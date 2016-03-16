<?php
ini_set("memory_limit", "5120M");
set_time_limit(30000);

global $db;

$sql = "SELECT DISTINCT
    		CASE 
			     WHEN ati2.atitipoenem = 'E' THEN ati2._atinumero ||' - '|| ati2.atidescricao
			     WHEN ati3.atitipoenem = 'E' THEN ati3._atinumero ||' - '|| ati3.atidescricao
			     WHEN ati4.atitipoenem = 'E' THEN ati4._atinumero ||' - '|| ati4.atidescricao
			     WHEN ati5.atitipoenem = 'E' THEN ati5._atinumero ||' - '|| ati5.atidescricao
			     WHEN ati6.atitipoenem = 'E' THEN ati6._atinumero ||' - '|| ati6.atidescricao
			     ELSE 'Não existe'
		    END as etapas,
   			CASE 
			     WHEN ati2.atitipoenem = 'E' THEN to_char(ati2.atidatainicio,'DD/MM/YYYY')
			     WHEN ati3.atitipoenem = 'E' THEN to_char(ati3.atidatainicio,'DD/MM/YYYY')
			     WHEN ati4.atitipoenem = 'E' THEN to_char(ati4.atidatainicio,'DD/MM/YYYY')
			     WHEN ati5.atitipoenem = 'E' THEN to_char(ati5.atidatainicio,'DD/MM/YYYY')
			     WHEN ati6.atitipoenem = 'E' THEN to_char(ati6.atidatainicio,'DD/MM/YYYY')
			     ELSE 'Não existe'
		    END as datainicioetapa,
    		CASE 
			     WHEN ati2.atitipoenem = 'E' THEN to_char(ati2.atidatafim,'DD/MM/YYYY')
			     WHEN ati3.atitipoenem = 'E' THEN to_char(ati3.atidatafim,'DD/MM/YYYY')
			     WHEN ati4.atitipoenem = 'E' THEN to_char(ati4.atidatafim,'DD/MM/YYYY')
			     WHEN ati5.atitipoenem = 'E' THEN to_char(ati5.atidatafim,'DD/MM/YYYY')
			     WHEN ati6.atitipoenem = 'E' THEN to_char(ati6.atidatafim,'DD/MM/YYYY')
			     ELSE 'Não existe'
			END as datafimetapa,
    		CASE 
    			 WHEN ati2.atitipoenem = 'P' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
			     WHEN ati3.atitipoenem = 'P' THEN ati3._atinumero ||' - '|| ati3.atidescricao
			     WHEN ati4.atitipoenem = 'P' THEN ati4._atinumero ||' - '|| ati4.atidescricao
			     WHEN ati5.atitipoenem = 'P' THEN ati5._atinumero ||' - '|| ati5.atidescricao
			     WHEN ati6.atitipoenem = 'P' THEN ati6._atinumero ||' - '|| ati6.atidescricao
			     ELSE 'Não existe'
		    END as processos,
		    CASE 
		    	 WHEN ati2.atitipoenem = 'P' THEN to_char(ati2.atidatainicio,'DD/MM/YYYY')
     			 WHEN ati3.atitipoenem = 'P' THEN to_char(ati3.atidatainicio,'DD/MM/YYYY')
			     WHEN ati4.atitipoenem = 'P' THEN to_char(ati4.atidatainicio,'DD/MM/YYYY')
      			 WHEN ati5.atitipoenem = 'P' THEN to_char(ati5.atidatainicio,'DD/MM/YYYY')
      			 WHEN ati6.atitipoenem = 'P' THEN to_char(ati6.atidatainicio,'DD/MM/YYYY')
		      	 ELSE 'Não existe'
		    END as datainicioprocesso,
		    CASE 
		    	 WHEN ati2.atitipoenem = 'P' THEN to_char(ati2.atidatafim,'DD/MM/YYYY')
		      	 WHEN ati3.atitipoenem = 'P' THEN to_char(ati3.atidatafim,'DD/MM/YYYY')
				 WHEN ati4.atitipoenem = 'P' THEN to_char(ati4.atidatafim,'DD/MM/YYYY')
				 WHEN ati5.atitipoenem = 'P' THEN to_char(ati5.atidatafim,'DD/MM/YYYY')
				 WHEN ati6.atitipoenem = 'P' THEN to_char(ati6.atidatafim,'DD/MM/YYYY')
				 ELSE 'Não existe'
			END as datafimprocesso,
		    CASE 
		    	 WHEN ati2.atitipoenem = 'S' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
		      	 WHEN ati3.atitipoenem = 'S' THEN ati3._atinumero ||' - '|| ati3.atidescricao 
			     WHEN ati4.atitipoenem = 'S' THEN ati4._atinumero ||' - '|| ati4.atidescricao
		      	 WHEN ati5.atitipoenem = 'S' THEN ati5._atinumero ||' - '|| ati5.atidescricao
		      	 WHEN ati6.atitipoenem = 'S' THEN ati6._atinumero ||' - '|| ati6.atidescricao
		     	 ELSE 'Não possuem sub-processos'
		       	 END as subprocessos,
		    CASE 
		    	 WHEN ati2.atitipoenem = 'S' THEN to_char(ati2.atidatainicio,'DD/MM/YYYY')
		      	 WHEN ati3.atitipoenem = 'S' THEN to_char(ati3.atidatainicio,'DD/MM/YYYY')
		      	 WHEN ati4.atitipoenem = 'S' THEN to_char(ati4.atidatainicio,'DD/MM/YYYY')
		      	 WHEN ati5.atitipoenem = 'S' THEN to_char(ati5.atidatainicio,'DD/MM/YYYY')
		      	 WHEN ati6.atitipoenem = 'S' THEN to_char(ati6.atidatainicio,'DD/MM/YYYY')
		     	 ELSE 'Não possuem sub-processos'
		    END as datainiciosubprocesso,
		    CASE 
		    	 WHEN ati2.atitipoenem = 'S' THEN to_char(ati2.atidatafim,'DD/MM/YYYY')
		      	 WHEN ati3.atitipoenem = 'S' THEN to_char(ati3.atidatafim,'DD/MM/YYYY')
		      	 WHEN ati4.atitipoenem = 'S' THEN to_char(ati4.atidatafim,'DD/MM/YYYY')
		     	 WHEN ati5.atitipoenem = 'S' THEN to_char(ati5.atidatafim,'DD/MM/YYYY')
		      	 WHEN ati6.atitipoenem = 'S' THEN to_char(ati6.atidatafim,'DD/MM/YYYY')
		     	 ELSE 'Não possuem sub-processos'
		       	 END as datafimsubprocesso,
    		CASE 
    			 WHEN icl.iclid is not null THEN icl.iclid || ' - ' ||icl.icldsc else 'Sem checklist' end as itemdescricao,
    		ati._atinumero ||' - '|| ati.atidescricao as atividadedescricao,
   			ati._atinumero ||' - '|| ati.atidescricao as atividades,
   			to_char(ati.atidatainicio,'DD/MM/YYYY') as datainicioatividade,
    		to_char(ati.atidatafim,'DD/MM/YYYY') as datafimatividade,
   			to_char(icl.iclprazo,'dd/mm/YYYY') as itemprazo,
		    CASE 
     			WHEN val1.vldsituacao = TRUE THEN 'Execução validada.'|| coalesce(val1.vldobservacao,' ')
           		WHEN val1.vldsituacao = FALSE THEN 'Execução invalidada.'|| coalesce(val1.vldobservacao,' ')
             	ELSE ' Execução não realizada. ' 
          	END as executado,
    		CASE 
    			WHEN val2.vldsituacao = TRUE THEN ' Validação validada. '|| CASE WHEN val2.vldobservacao !='' THEN 'Observação:'||val2.vldobservacao ELSE '' END
            	WHEN val2.vldsituacao = FALSE THEN ' Validação invalidada. '|| CASE WHEN val2.vldobservacao !='' THEN 'Observação:'||val2.vldobservacao ELSE '' END
             ELSE ' Validação não realizada. ' 
          	END as validado,
        	CASE 
         		WHEN val3.vldsituacao = TRUE THEN ' Certificação validada. '|| CASE WHEN val3.vldobservacao !='' THEN 'Observação:'||val3.vldobservacao ELSE '' END
     			WHEN val3.vldsituacao = FALSE THEN ' Certificação invalidada. '|| CASE WHEN val3.vldobservacao !='' THEN 'Observação:'||val3.vldobservacao ELSE '' END
             	ELSE ' Certificação não realizada. ' 
          	END as certificado,
	        CASE 
	        	WHEN icl.iclcritico = TRUE THEN 'Sim' ELSE 'Não' END as itemcritico,
        	CASE 
        		WHEN val1.vldid IS NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND ch1.entid IS NOT NULL THEN 'Não'
      			WHEN d.esdid = 281 and ( hd.aedid = 721 or hd.aedid is null ) THEN 'Não'
              	WHEN val1.vldid IS NOT NULL AND et1.etcid IS NOT NULL AND et1.etcopcaoevidencia=TRUE THEN 'Sim Com evidências ('||et1.etcevidencia||') '||to_char(val1.vlddata,'DD/MM/YYYY') 
              	WHEN val1.vldid IS NOT NULL AND et1.etcid IS NOT NULL AND et1.etcopcaoevidencia=FALSE THEN 'Sim Sem evidências '||to_char(val1.vlddata,'DD/MM/YYYY') 
   	 		END as execucao,
        	CASE 
        		WHEN val2.vldid IS NULL AND ch2.entid IS NOT NULL THEN 'Não'
              	WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND et2.etcid IS NOT NULL AND et2.etcopcaoevidencia=TRUE THEN 'Sim Com evidências ('||coalesce(et2.etcevidencia,'-')||')'||to_char(val2.vlddata,'DD/MM/YYYY')
              	WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND et2.etcid IS NOT NULL AND et2.etcopcaoevidencia=FALSE THEN 'Sim Sem evidências'||to_char(val2.vlddata,'DD/MM/YYYY')
        	END as validacao,
        	CASE 
        		WHEN val3.vldid IS NULL AND ch3.entid IS NOT NULL THEN 'Não'
             	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcid IS NOT NULL AND ESDID != 284 THEN 'Não'
             	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcid IS NOT NULL AND et3.etcopcaoevidencia=TRUE AND ESDID = 284 THEN 'Sim Com evidências ('||et3.etcevidencia||')'||to_char(val3.vlddata,'DD/MM/YYYY')
              	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcid IS NOT NULL AND et3.etcopcaoevidencia=FALSE AND ESDID = 284 THEN 'Sim Sem evidências '||to_char(val3.vlddata,'DD/MM/YYYY')
        	END as certificacao,
	        CASE 
	        	WHEN val1.vldid IS NULL   AND val2.vldid IS NULL AND val3.vldid IS NULL THEN 'Não'
              	WHEN val1.vldid IS NOT NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND et1.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
             	WHEN val1.vldid IS NOT NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND et1.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
        	END as execucao_agrupador,
        	CASE 
        		WHEN en1.entnome IS NULL THEN 'Sem executor(es)' ELSE en1.entnome || ' ' || case when trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) END 
        	END as executores,
	        CASE 
	        	WHEN val2.vldid IS NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL THEN 'Não'
             	WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL AND et2.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
             	WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL AND et2.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
        	END as validacao_agrupador,
        	CASE 
        		WHEN en2.entnome IS NULL THEN 'Sem validador(es)' ELSE en2.entnome || ' ' || case when trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) END 
        	END as validadores,
	        CASE 
	        	WHEN val3.vldid IS NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL THEN 'Não'
             	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
             	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
        	END as certificacao_agrupador,
       		CASE 
       			WHEN en3.entnome IS NULL THEN 'Sem certificador(es)'
           		ELSE coalesce(en3.entnome,' ') || ' ' || case when trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) END 
       		END as certificadores 
   		FROM pde.atividade ati 
   		LEFT JOIN pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
   		LEFT JOIN pde.atividade ati3 ON ati3.atiid = ati2.atiidpai AND ati3.atistatus = 'A'
   		LEFT JOIN pde.atividade ati4 ON ati4.atiid = ati3.atiidpai AND ati4.atistatus = 'A'
  		LEFT JOIN pde.atividade ati5 ON ati5.atiid = ati4.atiidpai AND ati5.atistatus = 'A'
   		LEFT JOIN pde.atividade ati6 ON ati6.atiid = ati5.atiidpai AND ati6.atistatus = 'A'
   		LEFT JOIN pde.itemchecklist icl ON ati.atiid = icl.atiid 
   		LEFT JOIN workflow.documento d ON d.docid = icl.docid 
   		LEFT JOIN workflow.historicodocumento hd ON hd.hstid = d.hstid
  	 	LEFT JOIN workflow.historicodocumento hd2 ON hd2.docid = d.docid
   		LEFT JOIN pde.etapascontrole et1 ON et1.iclid = icl.iclid AND et1.tpvid = 1 
   		LEFT JOIN pde.validacao val1 ON val1.iclid = icl.iclid AND val1.tpvid = 1 
   		LEFT JOIN pde.checklistentidade ch1 ON ch1.iclid = icl.iclid AND ch1.tpvid = 1
   		LEFT JOIN entidade.entidade en1 ON en1.entid = ch1.entid AND en1.entstatus = 'A'
   		LEFT JOIN pde.etapascontrole et2 ON et2.iclid = icl.iclid AND et2.tpvid = 2 
   		LEFT JOIN pde.validacao val2 ON val2.iclid = icl.iclid AND val2.tpvid = 2 
   		LEFT JOIN pde.checklistentidade ch2 ON ch2.iclid = icl.iclid AND ch2.tpvid = 2
   		LEFT JOIN entidade.entidade en2 ON en2.entid = ch2.entid AND en2.entstatus = 'A'
   		LEFT JOIN pde.etapascontrole et3 ON et3.iclid = icl.iclid AND et3.tpvid = 3
   		LEFT JOIN pde.validacao val3 ON val3.iclid = icl.iclid AND val3.tpvid = 3 
   		LEFT JOIN pde.checklistentidade ch3 ON ch3.iclid = icl.iclid AND ch3.tpvid = 3
   		LEFT JOIN entidade.entidade en3 ON en3.entid = ch3.entid AND en3.entstatus = 'A' 
   		WHERE ati.atistatus = 'A' AND ( ( ati3.atitipoenem = 'E' AND ati3.atiid  IN  ('127343','124019','122601','122694','122778','122977','123004') ) OR
   		( ati4.atitipoenem = 'E' AND ati4.atiid  IN  ('127343','124019','122601','122694','122778','122977','123004') ) ) 
  		ORDER BY etapas,processos,atividades";
<<<<<<< .mine

$dadosExcel = $db->carregar($sql);

=======
		
$dadosExcel = $db->carregar($sql);
if(empty($dadosExcel)){
	echo "<script type="text/javascript" language="javasscript">
			alert('Nenhum registro encontrado!');
		 </script>";
} else {
	$cabecalho = array("Etapas","Data Ínicio Etapas","Data Fim Etapas","Processos","Data Ínicio Processos","Data Fim Processos","Subprocessos","Data Ínicio Subprocessos","Data Fim Subprocessos","Item","Atividade","Atividades","","","Item Prazo","Executado","Validado","Certificado","Item Crítico","Execução","Validação","Cetificação","Execução Agrupador",
	"Executores","Validação Agrupador","Validadores","Certificação Agrupador","Certificadores");
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$db->sql_to_excel($dadosExcel, 'Relatorio_Geral', $cabecalho);
}
>>>>>>> .r52160
if(empty($dadosExcel)){
	echo "<script type='text/javascript' language='javasscript'>
			alert('Nenhum registro encontrado!');
		 </script>";
} else {
	$cabecalho = array("Etapas","Data Ínicio Etapas","Data Fim Etapas","Processos","Data Ínicio Processos","Data Fim Processos","Subprocessos","Data Ínicio Subprocessos","Data Fim Subprocessos","Item","Atividade","Atividades","","","Item Prazo","Executado","Validado","Certificado","Item Crítico","Execução","Validação","Cetificação","Execução Agrupador",
	"Executores","Validação Agrupador","Validadores","Certificação Agrupador","Certificadores");
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$db->sql_to_excel($dadosExcel, 'Relatorio_Geral', $cabecalho);
}
?>