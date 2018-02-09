<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

//error_reporting(-1);

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';


$db = new cls_banco();

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$semana_ini = date("Y-m-d");
$semana_fim = $db->pegaUm("SELECT date '".$semana_ini."' + integer '7' as data");


$sql = "SELECT DISTINCT
								ent.entid,
								ent.entnome,
								usu.usuemail
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid 
							LEFT JOIN
								seguranca.usuario usu ON usu.usucpf = ent.entnumcpfcnpj
							WHERE
								cle.tpvid in (1,2,3)
							ORDER BY
								ent.entnome";

$pessoas = $db->carregar($sql);

if($pessoas[0]) {
	foreach($pessoas as $p) {

        $registroExiste = gravarLogArquivo($p['entid']);
        if($registroExiste){
            continue;
        }

		// instancia a classe de relatório
		$rel = new montaRelatorio();
		
		// monta o sql, agrupador e coluna do relatório
		$sql       = "SELECT DISTINCT
						icl.iclid || ' - ' ||icl.icldsc as itemdescricao,
						ati._atinumero ||' - '|| ati.atidescricao as atividadedescricao,
						ati._atinumero ||' - '|| ati.atidescricao as atividades,
						to_char(icl.iclprazo,'dd/mm/YYYY') as itemprazo,
						
						CASE 
							WHEN val1.vldsituacao = TRUE 
								THEN 'Execução validada.'|| coalesce(val1.vldobservacao,' ')
				          	WHEN val1.vldsituacao = FALSE 
				          		THEN 'Execução invalidada.'|| coalesce(val1.vldobservacao,' ')
				          	ELSE ' Execução não realizada. ' 
				        END as executado,
						CASE 
							WHEN val2.vldsituacao = TRUE 
								THEN ' Validação validada. '|| CASE WHEN val2.vldobservacao !='' THEN 'Observação:'||val2.vldobservacao ELSE '' END
				          	WHEN val2.vldsituacao = FALSE 
				          		THEN ' Validação invalidada. '|| CASE WHEN val2.vldobservacao !='' THEN 'Observação:'||val2.vldobservacao ELSE '' END
				          	ELSE ' Validação não realizada. ' 
				        END as validado,
				     	CASE 
				     		WHEN val3.vldsituacao = TRUE 
				     			THEN ' Certificação validada. '|| CASE WHEN val3.vldobservacao !='' THEN 'Observação:'||val3.vldobservacao ELSE '' END
							WHEN val3.vldsituacao = FALSE 
								THEN ' Certificação invalidada. '|| CASE WHEN val3.vldobservacao !='' THEN 'Observação:'||val3.vldobservacao ELSE '' END
				          	ELSE ' Certificação não realizada. ' 
				        END as certificado,
				        
						CASE WHEN ati2.atitipoenem = 'S' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
							 WHEN ati3.atitipoenem = 'S' THEN ati3._atinumero ||' - '|| ati3.atidescricao 
							 WHEN ati4.atitipoenem = 'S' THEN ati4._atinumero ||' - '|| ati4.atidescricao
							 WHEN ati5.atitipoenem = 'S' THEN ati5._atinumero ||' - '|| ati5.atidescricao
							 WHEN ati6.atitipoenem = 'S' THEN ati6._atinumero ||' - '|| ati6.atidescricao
							ELSE 'Não possuem sub-processos'
					    END as subprocessos,
						CASE WHEN ati2.atitipoenem = 'P' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
							 WHEN ati3.atitipoenem = 'P' THEN ati3._atinumero ||' - '|| ati3.atidescricao
							 WHEN ati4.atitipoenem = 'P' THEN ati4._atinumero ||' - '|| ati4.atidescricao
							 WHEN ati5.atitipoenem = 'P' THEN ati5._atinumero ||' - '|| ati5.atidescricao
							 WHEN ati6.atitipoenem = 'P' THEN ati6._atinumero ||' - '|| ati6.atidescricao
							 ELSE 'Não existe'
						END as processos,
						CASE 
							 WHEN ati2.atitipoenem = 'E' THEN ati2._atinumero ||' - '|| ati2.atidescricao
							 WHEN ati3.atitipoenem = 'E' THEN ati3._atinumero ||' - '|| ati3.atidescricao
							 WHEN ati4.atitipoenem = 'E' THEN ati4._atinumero ||' - '|| ati4.atidescricao
							 WHEN ati5.atitipoenem = 'E' THEN ati5._atinumero ||' - '|| ati5.atidescricao
							 WHEN ati6.atitipoenem = 'E' THEN ati6._atinumero ||' - '|| ati6.atidescricao
							 ELSE 'Não existe'
						END as etapas,
				     	CASE WHEN icl.iclcritico=TRUE THEN 'Sim' ELSE 'Não' END as itemcritico,
				     	
				     	CASE WHEN val1.vldid IS NULL 	 AND val2.vldid IS NULL    AND val3.vldid IS NULL AND ch1.entid IS NOT NULL       THEN 'Não'
				          	 WHEN val1.vldid IS NOT NULL AND et1.etcid IS NOT NULL AND et1.etcopcaoevidencia=TRUE  THEN 'Sim
		 Com evidências ('||et1.etcevidencia||') 
		'||to_char(val1.vlddata,'DD/MM/YYYY') 
				          	 WHEN val1.vldid IS NOT NULL AND et1.etcid IS NOT NULL AND et1.etcopcaoevidencia=FALSE THEN 'Sim
		 Sem evidências 
		'||to_char(val1.vlddata,'DD/MM/YYYY') 
				     	END as execucao,
				     	CASE WHEN val2.vldid IS NULL 	 AND ch2.entid IS NOT NULL THEN 'Não'
				          	 WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND et2.etcid IS NOT NULL AND et2.etcopcaoevidencia=TRUE THEN 'Sim
		 Com evidências ('||coalesce(et2.etcevidencia,'-')||')
		'||to_char(val2.vlddata,'DD/MM/YYYY')
				          	 WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND et2.etcid IS NOT NULL AND et2.etcopcaoevidencia=FALSE THEN 'Sim
		 Sem evidências
		'||to_char(val2.vlddata,'DD/MM/YYYY')
				     	END as validacao,
				     	CASE WHEN val3.vldid IS NULL 	 AND ch3.entid IS NOT NULL THEN 'Não'
				         	 WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcid IS NOT NULL AND et3.etcopcaoevidencia=TRUE THEN 'Sim
		 Com evidências ('||et3.etcevidencia||')
		'||to_char(val3.vlddata,'DD/MM/YYYY')
				          	 WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcid IS NOT NULL AND et3.etcopcaoevidencia=FALSE THEN 'Sim
		 Sem evidências
		'||to_char(val3.vlddata,'DD/MM/YYYY')
				     	END as certificacao,
				     	
				     	CASE WHEN val1.vldid IS NULL 	 AND val2.vldid IS NULL AND val3.vldid IS NULL THEN 'Não'
				          	 WHEN val1.vldid IS NOT NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND et1.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
				         	 WHEN val1.vldid IS NOT NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND et1.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
				     	END as execucao_agrupador,
				     	CASE WHEN en1.entnome IS NULL THEN 'Sem executor(es)' ELSE en1.entnome || ' ' || case when trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) END 
				     	END as executores,
				
				     	CASE WHEN val2.vldid IS NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL THEN 'Não'
				          	WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL AND et2.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
				          	WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL AND et2.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
				     	END as validacao_agrupador,
				     	CASE WHEN en2.entnome IS NULL THEN 'Sem validador(es)' ELSE en2.entnome || ' ' || case when trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) END 
				     	END as validadores,
				     	
				     	CASE WHEN val3.vldid IS NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL THEN 'Não'
				          	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
				          	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
				     	END as certificacao_agrupador,
				    	CASE WHEN en3.entnome IS NULL
				        	THEN 'Sem certificador(es)'
				        	ELSE coalesce(en3.entnome,' ') || ' ' || case when trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) END 
				    	END as certificadores 
					FROM 
						pde.itemchecklist icl 
					INNER JOIN 
						pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A'
					LEFT JOIN 
						pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
					LEFT JOIN 
						pde.atividade ati3 ON ati3.atiid = ati2.atiidpai AND ati3.atistatus = 'A'
					LEFT JOIN 
						pde.atividade ati4 ON ati4.atiid = ati3.atiidpai AND ati4.atistatus = 'A'
					LEFT JOIN 
						pde.atividade ati5 ON ati5.atiid = ati4.atiidpai AND ati5.atistatus = 'A'
					LEFT JOIN 
						pde.atividade ati6 ON ati6.atiid = ati5.atiidpai AND ati6.atistatus = 'A'
					LEFT JOIN 
						pde.etapascontrole et1 ON et1.iclid = icl.iclid AND et1.tpvid = 1 
					LEFT JOIN 
						pde.validacao val1 ON val1.iclid = icl.iclid AND val1.tpvid = 1 
					LEFT JOIN 
						pde.checklistentidade ch1 ON ch1.iclid = icl.iclid AND ch1.tpvid = 1
					LEFT JOIN 
						entidade.entidade en1 ON en1.entid = ch1.entid AND en1.entstatus = 'A'
					LEFT JOIN 
						pde.etapascontrole et2 ON et2.iclid = icl.iclid AND et2.tpvid = 2 
					LEFT JOIN 
						pde.validacao val2 ON val2.iclid = icl.iclid AND val2.tpvid = 2 
					LEFT JOIN 
						pde.checklistentidade ch2 ON ch2.iclid = icl.iclid AND ch2.tpvid = 2
					LEFT JOIN 
						entidade.entidade en2 ON en2.entid = ch2.entid AND en2.entstatus = 'A'
					LEFT JOIN 
						pde.etapascontrole et3 ON et3.iclid = icl.iclid AND et3.tpvid = 3
					LEFT JOIN 
						pde.validacao val3 ON val3.iclid = icl.iclid AND val3.tpvid = 3 
					LEFT JOIN 
						pde.checklistentidade ch3 ON ch3.iclid = icl.iclid AND ch3.tpvid = 3
					LEFT JOIN 
						entidade.entidade en3 ON en3.entid = ch3.entid AND en3.entstatus = 'A' 
					WHERE ( en1.entid IN('".$p['entid']."') OR 
						  	en2.entid IN('".$p['entid']."') OR 
							en3.entid IN('".$p['entid']."') ) 
							AND (
							(val1.vldid IS NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND ch1.entid IS NOT NULL AND et1.etcid IS NOT NULL AND ch1.entid IN('".$p['entid']."')) OR  
							(val2.vldid IS NULL AND ch2.entid IS NOT NULL AND et2.etcid IS NOT NULL  AND ch2.entid IN('".$p['entid']."')) OR  
							(val3.vldid IS NULL AND ch3.entid IS NOT NULL AND et3.etcid IS NOT NULL  AND ch3.entid  IN('".$p['entid']."')) ) 
							AND (
							( icl.iclprazo >= '".$semana_ini."' AND icl.iclprazo <= '".$semana_fim."' )
							)
					ORDER BY atividades";
		
		$agrupador = array("agrupador" => array(0 => array("campo"=>"atividadedescricao","label"=>"Atividades"),
								   				1 => array("campo"=>"itemdescricao","label"=>"Descrição do item")),
			  			   "agrupadoColuna" => array("itemprazo","execucao","executores","atividadedescricao","validacao","validadores","certificacao","certificadores"));
								   				
		
		$coluna    = array(0 => array("campo"=>"atividadedescricao","label"=>"Descrição da atividade","blockAgp"=>"","type"=>"string"),
						  1 => array("campo"=>"itemprazo","label"=>"Prazo do item","blockAgp"=>"","type"=>"string"),
						  2 => array("campo"=>"execucao","label"=>"Execução","blockAgp"=>"","type"=>"string"),
						  3 => array("campo"=>"executores","label"=>"Executor(es)","blockAgp"=>"","type"=>"string"),
						  4 => array("campo"=>"validacao","label"=>"Validação","blockAgp"=>"","type"=>"string"),
						  5 => array("campo"=>"validadores","label"=>"Validador(es)","blockAgp"=>"","type"=>"string"),
						  6 => array("campo"=>"certificacao","label"=>"Certificação","blockAgp"=>"","type"=>"string"),
						  7 => array("campo"=>"certificadores","label"=>"Certificador(es)","blockAgp"=>"","type"=>"string"));
		
		$dados 	   = $db->carregar( $sql );

		if($dados[0]) {
		
			$rel->setAgrupador($agrupador, $dados); 
			$rel->setColuna($coluna);
			$rel->setTolizadorLinha(false);
			$rel->setEspandir(true);
			$rel->setMonstrarTolizadorNivel(true);
			
			$corpo = $rel->getRelatorio();
			
			$html = str_replace(array("../imagens/"),
								array("http://simec.mec.gov.br/imagens/"),
								'<html>
								<center>Período: '.formata_data($semana_ini).' à '.formata_data($semana_fim).'</center>
								<center><table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">'
								.'	<tr bgcolor="#ffffff">' 	
								.'		<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>'			
								.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'				
								.'			'. NOME_SISTEMA. '<br/>'				
								.'			MEC / SE - Secretaria Executiva <br />'
								.'		</td>'
								.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'					
								.'			Hora da Impressão:' . date( 'd/m/Y - H:i:s' ) . '<br />'					
								.'		</td>'					
								.'	</tr><tr>'
								.'		<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">'
								.'			<b><font style="font-size:14px;">Relátorio de Checklist</font></b>'
								.'		</td>'
								.'	</tr>'					
								.'</table>'					
								.'</center>
								'.$corpo.'
								</html>');
								
			require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
			require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= "Relatório de Checklist";
			$mensagem->From 		= "no-reply@mec.gov.br";
			$mensagem->AddBCC( $_SESSION['email_sistema'], "Vitor Sad" );

            //$mensagem->AddAddress( $_SESSION['email_sistema'], "Vitor Sad" );

			if($p['usuemail']) {
				
				$mensagem->AddAddress( $p['usuemail'], $p['entnome'] );
								
			} else {
				
				$emails = $db->carregar("select distinct e.entid, entnome, usuemail from pde.usuarioresponsabilidade ur
										inner join seguranca.usuario u ON u.usucpf = ur.usucpf
										inner join entidade.entidade e ON e.entnumcpfcnpj = ur.usucpf
										where ur.entid = '".$p['entid']."'");
				if($emails[0]) {
					foreach($emails as $em) {
						$mensagem->AddAddress( $em['usuemail'], $em['entnome'] );
					}
				}
			}

			$mensagem->Subject = "Atividades de checklist: ".$p['entnome']."(".formata_data($semana_ini)." à ".formata_data($semana_fim).")";
			$mensagem->Body = $html;
			
			$mensagem->IsHTML( true );
			echo "resp.".$mensagem->Send()."<br>";
		}
	}
}

/**
 * Grava Log de erro
 *
 * @param Text $sMessage
 * @return void
 */
function gravarLogArquivo($entid)
{
    $errdata = date ( 'Y-m-d' );

    $nomeArquivo = 'log_aviso_enem_checklist_' . date("Ymd") . '.log';
    $pathRaiz = APPRAIZ . 'arquivos/log_erro/';

    if(!is_dir($pathRaiz)){
        mkdir($pathRaiz, 0777);
    }


    $arquivo = fopen($pathRaiz . $nomeArquivo, "a+");

    $registroExiste = false;
    while(!feof($arquivo)) {
        // lê uma linha do arquivo
        $linha = fgets($arquivo, 4096);
        if(false !== strpos($linha, "{$errdata};{$entid}")){
            $registroExiste = true;
            break;
        }
    }

    if(!$registroExiste){
        $log = "{$errdata};{$entid}\r\n";
        fwrite($arquivo, $log);
    }

    fclose($arquivo);

    return $registroExiste;
}

echo "fim";
?>