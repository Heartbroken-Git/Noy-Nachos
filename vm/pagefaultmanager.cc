    /*! \file pagefaultmanager.cc
    Routines for the page fault managerPage Fault Manager
    */
    //
    //  Copyright (c) 1999-2000 INSA de Rennes.
    //  All rights reserved.
    //  See copyright_insa.h for copyright notice and limitation
    //  of liability and disclaimer of warranty provisions.
    //

    #include "kernel/thread.h"
    #include "vm/swapManager.h"
    #include "vm/physMem.h"
    #include "vm/pagefaultmanager.h"

    PageFaultManager::PageFaultManager() {


    }

    // PageFaultManager::~PageFaultManager()
    /*! Nothing for now
    */
    PageFaultManager::~PageFaultManager() {
    }

    // ExceptionType PageFault(uint32_t virtualPage)
    /*!
    //	This method is called by the Memory Management Unit when there is a
    //      page fault. This method loads the page from :
    //      - read-only sections (text,rodata) $\Rightarrow$ executive
    //        file
    //      - read/write sections (data,...) $\Rightarrow$ executive
    //        file (1st time only), or swap file
    //      - anonymous mappings (stack/bss) $\Rightarrow$ new
    //        page from the MemoryManager (1st time only), or swap file
    //
    //	\param virtualPage the virtual page subject to the page fault
    //	  (supposed to be between 0 and the
    //        size of the address space, and supposed to correspond to a
    //        page mapped to something [code/data/bss/...])
    //	\return the exception (generally the NO_EXCEPTION constant)
    */
    ExceptionType PageFaultManager::PageFault(uint32_t virtualPage)
    {


      #ifdef ETUDIANTS_TP

      int nb_phys_page = g_cfg->NumPhysPages;
      int page_size = g_cfg->PageSize;
      int swap =g_machine->mmu->translationTable->getBitSwap(virtualPage);
      int addr = g_machine->mmu->translationTable->getAddrDisk(virtualPage);
      char tmp[page_size];
      if(swap ==  1){// Page is in the swap area

        while(addr == -1){// waiting for the thief
           g_current_thread->Yield();
         }

         g_swap_manager->GetPageSwap(addr,tmp);//load page in temp buffer

         //get new physical page number
         int phys_page_number = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace,virtualPage);



         //copy the temp buff into the new phys page
         memcpy(&(g_machine->mainMemory[g_machine->mmu->translationTable->getPhysicalPage(phys_page_number)*page_size]),&tmp, page_size);

          // link the virtual page to the new physical page
          g_machine->mmu->translationTable->setPhysicalPage(virtualPage,phys_page_number);
          g_machine->mmu->translationTable->setBitValid(virtualPage);
          return ((ExceptionType)0);

      }else{
          int phys_page = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace,virtualPage);

          if(addr == -1){


              //fill 0 i tmpp buffer
              bzero(tmp,page_size);

              g_machine->mmu->translationTable->setPhysicalPage(virtualPage,phys_page);
              memcpy(&(g_machine->mainMemory[g_machine->mmu->translationTable->getPhysicalPage(phys_page)*page_size]),&tmp, page_size);
              g_machine->mmu->translationTable->setBitValid(virtualPage);
              g_machine->mmu->translationTable->setPhysicalPage(virtualPage,phys_page);
              g_machine->mmu->translationTable->setBitSwap(virtualPage);

              return ((ExceptionType)0);


          }else{

              OpenFile* mapped_file = g_current_thread->GetProcessOwner()->addrspace->findMappedFile(virtualPage);
              if (mapped_file != NULL) {
                  mapped_file -> ReadAt(tmp,page_size, addr);
              } else {
                  g_current_thread -> GetProcessOwner() -> exec_file -> ReadAt(tmp,page_size, addr);
              }

              g_machine->mmu->translationTable->setPhysicalPage(virtualPage,phys_page);
              memcpy(&(g_machine->mainMemory[g_machine->mmu->translationTable->getPhysicalPage(phys_page)*page_size]),&tmp, page_size);
              g_machine->mmu->translationTable->setBitValid(virtualPage);
              g_machine->mmu->translationTable->setPhysicalPage(virtualPage,phys_page);

              return ((ExceptionType)0);

          }




      }


      return ((ExceptionType)0);
      #endif

      #ifndef ETUDIANTS_TP

      printf("**** Warning: page fault manager is not implemented yet\n");
        exit(-1);
        return ((ExceptionType)0);

        #endif
    }
