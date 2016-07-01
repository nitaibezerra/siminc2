<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

$sql = 'SELECT "NU_CGC_FAVORECIDO",  replace("VL_EMPENHO", \',\', \'.\') as vlempenho , po.pronumeroprocesso, "CO_ESPECIE_EMPENHO", "CO_PLANO_INTERNO_APROV", "CO_ESFERA_ORCAMENTARIA_SOLIC", "CO_PTRES_SOLIC",
"CO_FONTE_RECURSO_SOLIC", "CO_NATUREZA_DESPESA_SOLIC", "CO_CENTRO_GESTAO_APROV", "CO_OBSERVACAO", "CO_TIPO_EMPENHO",  "CO_DESCRICAO_EMPENHO",
"CO_PROGRAMA_FNDE", "NU_ID_SISTEMA", "NU_SEQ_MOV_NE", po.proid
FROM carga.empenhosigef es 
INNER JOIN par.processoobra po ON po.pronumeroprocesso = es."NU_PROCESSO" and po.prostatus = \'A\' ';

$processos = $db->carregar($sql);

foreach($processos as $dados){

	$sql = "INSERT INTO par.empenho(
            empcnpj,
            empvalorempenho,
            empnumeroprocesso,
            empcodigoespecie,
            empcodigopi,
            empcodigoesfera,
            empcodigoptres,
            empfonterecurso,
            empcodigonatdespesa,
            empcentrogestaosolic,
            empcodigoobs,
            empcodigotipo,
            empdescricao,
            empgestaoeminente,
            empunidgestoraeminente,
            empprogramafnde,
            empnumerosistema,
            usucpf,
            empprotocolo,
            empsituacao
            )
		    VALUES (".$dados['NU_CGC_FAVORECIDO'].",
		    		".$dados['vlempenho'].",
		            ".$dados['pronumeroprocesso'].",
		            ".$dados['CO_ESPECIE_EMPENHO'].",
		           '".$dados['CO_PLANO_INTERNO_APROV']."',
		            ".$dados['CO_ESFERA_ORCAMENTARIA_SOLIC'].",
		            ".$dados['CO_PTRES_SOLIC'].",
		            ".$dados['CO_FONTE_RECURSO_SOLIC'].",
		            ".$dados['CO_NATUREZA_DESPESA_SOLIC'].",
		            ".$dados['CO_CENTRO_GESTAO_APROV'].",
		            ".$dados['CO_OBSERVACAO'].",
		            ".$dados['CO_TIPO_EMPENHO'].",
		            ".$dados['CO_DESCRICAO_EMPENHO'].",
		            15253,
		            153173,
		            '".$dados['CO_PROGRAMA_FNDE']."',
		            ".$dados['NU_ID_SISTEMA'].",
		            '',
		            ".$dados['NU_SEQ_MOV_NE'].",
		            '8 - SOLICITAÇÃO APROVADA'
		            ) RETURNING empid;";

			$empid = $db->pegaUm($sql);
			
			$sql = "INSERT INTO par.historicoempenho(
            		usucpf, empid, hepdata, empsituacao)
    				VALUES ('', '".$empid."', NOW(), '8 - SOLICITAÇÃO APROVADA');";

			$db->executar($sql);
						
			$sql = 'select o.preid,  replace(empenhoporobra, \',\', \'.\') as vlrempenhoporobra, "Processo", "UF", case when "% do empenho" = \'0,3\' then 30 else 40 end as porcentagem 
					from carga.obrasfranciscos o
					inner join par.processoobraspaccomposicao pop on pop.preid::integer = o.preid::integer and pop.pocstatus = \'A\'
					where "Processo" = \''.$dados['pronumeroprocesso']."'";
			
			
			$obras = $db->carregar($sql);
			
			foreach($obras as $dadosObra)
			{
				$sql = "INSERT INTO par.empenhoobra(
            				preid, empid, eobpercentualemp, eobvalorempenho, eobpercentualemp2)
    						VALUES ('".$dadosObra['preid']."', '".$empid."', '".$dadosObra['porcentagem']."', '".$dadosObra["vlrempenhoporobra"]."', '".round($dadosObra['porcentagem'])."');";

					$db->executar($sql);
					
					
			}
			$db->commit();
			echo "Inserido historico de empenho para o processo".$dados['pronumeroprocesso']." <br>";
}


?>