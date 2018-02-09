<?php

function checklist_monta_coluna_relatorio(){

	$coluna = array();

	array_push( $coluna, array("campo" 	  => "atividadedescricao",
					   		   "label" 	  => "Descrição da atividade",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );

	array_push( $coluna, array("campo" 	  => "itemprazo",
					   		   "label" 	  => "Prazo do item",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );

	array_push( $coluna, array("campo" 	  => "execucao",
					   		   "label" 	  => "Execução",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );

	array_push( $coluna, array("campo" 	  => "executores",
					   		   "label" 	  => "Executor(es)",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );

	array_push( $coluna, array("campo" 	  => "validacao",
					   		   "label" 	  => "Validação",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );

	array_push( $coluna, array("campo" 	  => "validadores",
					   		   "label" 	  => "Validador(es)",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );

	array_push( $coluna, array("campo" 	  => "certificacao",
					   		   "label" 	  => "Certificação",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );

	array_push( $coluna, array("campo" 	  => "certificadores",
					   		   "label" 	  => "Certificador(es)",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );

	return $coluna;

}


function checklist_monta_agp_relatorio(){

	$agrupador = $_REQUEST['agrupadores'];

	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("itemprazo","execucao","executores","atividadedescricao",
										  "validacao","validadores","certificacao","certificadores")
				);

//	array_push( $agp['agrupador'], array("campo" 	  => "executores",
//					   		   			 "label" 	  => "Executor(es)") );
//	array_push( $agp['agrupador'], array("campo" 	  => "validadores",
//					   		   			 "label" 	  => "Validador(es)") );
//	array_push( $agp['agrupador'], array("campo" 	  => "certificadores",
//					   		   		     "label" 	  => "Certificador(es)") );
	array_push( $agp['agrupador'], array("campo" 	  => "atividadedescricao",
								   		 "label" 	  => "Atividades") );
	array_push( $agp['agrupador'], array("campo" 	  => "itemdescricao",
		 					   		     "label" 	  => "Descrição do item") );


	return $agp;

}


function checklist_monta_sql_relatorio( $pessoa ){

	$where = array();

	// $pessoas
	array_push($where, "( en1.entid IN ('" . $pessoa . "')
						  OR en2.entid IN ('" . $pessoa . "')
						  OR en3.entid IN ('" . $pessoa . "') )");

	//intervalo de datas
	array_push($where, " ( icl.iclprazo >= '".date('Y-m-d')."' AND icl.iclprazo <= '".date('Y-m-d', strtotime('+1 week'))."' ) ");

	// monta o sql
	$sql = "SELECT DISTINCT
				'<input type=\"hidden\" id=\"'||icl.iclid||'\"/>'|| icl.iclid || ' - ' || icl.icldsc as itemdescricao,
				'<input type=\"hidden\" id=\"'||ati.atiid||'\"/>'||ati._atinumero ||' - '|| ati.atidescricao as atividadedescricao,
				'<input type=\"hidden\" id=\"'||ati.atiid||'\"/>'||ati._atinumero ||' - '|| ati.atidescricao as atividades,
				to_char(icl.iclprazo,'dd/mm/YYYY') as itemprazo,

				CASE WHEN val1.vldsituacao = TRUE THEN ' Execução validada. '|| CASE WHEN val1.vldobservacao !='' THEN 'Observação:'||val1.vldobservacao ELSE '' END
		          	WHEN val1.vldsituacao = FALSE THEN ' Execução invalidada. '|| CASE WHEN val1.vldobservacao !='' THEN 'Observação:'||val1.vldobservacao ELSE '' END
		          	ELSE ' Execução não realizada. '
		        END as executado,
				CASE WHEN val2.vldsituacao = TRUE THEN ' Validação validada. '|| CASE WHEN val2.vldobservacao !='' THEN 'Observação:'||val2.vldobservacao ELSE '' END
		          	WHEN val2.vldsituacao = FALSE THEN ' Validação invalidada. '|| CASE WHEN val2.vldobservacao !='' THEN 'Observação:'||val2.vldobservacao ELSE '' END
		          	ELSE ' Validação não realizada. '
		        END as validado,
		     	CASE WHEN val3.vldsituacao = TRUE THEN ' Certificação validada. '|| CASE WHEN val3.vldobservacao !='' THEN 'Observação:'||val3.vldobservacao ELSE '' END
					WHEN val3.vldsituacao = FALSE THEN ' Certificação invalidada. '|| CASE WHEN val3.vldobservacao !='' THEN 'Observação:'||val3.vldobservacao ELSE '' END
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

		     	CASE WHEN val1.vldid IS NULL 	 AND val2.vldid IS NULL AND val3.vldid IS NULL THEN 'Não'
		          	 WHEN val1.vldid IS NOT NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND et1.etcopcaoevidencia=TRUE THEN 'Sim Com evidências ('||et1.etcevidencia||')'
		          	 WHEN val1.vldid IS NOT NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND et1.etcopcaoevidencia=FALSE THEN 'Sim Sem evidências'
		     	END as execucao,
		     	CASE WHEN val2.vldid IS NULL 	 AND val1.vldid IS NOT NULL AND val3.vldid IS NULL THEN 'Não'
		          	 WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL AND et2.etcopcaoevidencia=TRUE THEN 'Sim Com evidências ('||et2.etcevidencia||')'
		          	 WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL AND et2.etcopcaoevidencia=FALSE THEN 'Sim Sem evidências'
		     	END as validacao,
		     	CASE WHEN val3.vldid IS NULL 	 AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL THEN 'Não'
		         	 WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcopcaoevidencia=TRUE THEN 'Sim Com evidências ('||et3.etcevidencia||')'
		          	 WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcopcaoevidencia=FALSE THEN 'Sim Sem evidências'
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
			".(($where)?"WHERE ".implode(" AND ",$where):"")."
			ORDER BY
				executores, validadores, certificadores";
//ver($sql,1);
	return $sql;

}

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

//define( 'APPRAIZ', '/var/www/projetos/simec/' );

$_REQUEST['baselogin'] = "simec_espelho_producao";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';
//$_SESSION['usucpf'] = '00000000191';
//$_SESSION['mnuid'] = 6719;
//$_SESSION['sisid'] = 24;
//$_SESSION['usunivel'] = 1;
//$_SESSION['superuser'] = 1;
//$_SESSION['sisbaselogin'] = simec_espelho_producao;

// Inclui componente de simec
include_once "/var/www/simec/global/config.inc";
//include_once "/var/www/projetos/simec/global/config.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();

// Inclui componente de relatórios
include APPRAIZ . 'includes/classes/relatorio.class.inc';

//inclui componente de email e instancia a classe de email
include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';
$e = new EmailAgendado();

//monta agrupador e coluna
$agrupador = checklist_monta_agp_relatorio();
$coluna    = checklist_monta_coluna_relatorio();

$sql = "SELECT DISTINCT
			ent.entid
		FROM
			entidade.entidade ent
		INNER JOIN
			pde.checklistentidade cle ON cle.entid = ent.entid
		WHERE
			cle.tpvid in (1,2,3)
		ORDER BY
			1
		LIMIT
			30
		";

$pessoas = $db->carregarColuna($sql);
//ver($pessoas);
foreach($pessoas as $pessoa){

	$sql = "SELECT
				usuemail
			FROM
				entidade.entidade ent
			INNER JOIN seguranca.usuario usu ON usu.usucpf = ent.entnumcpfcnpj
			WHERE
				ent.entid in (".$pessoa.")";

	$dest = $db->pegaUm($sql);

	if($dest!=''){
		// monta o sql
		$sql       = checklist_monta_sql_relatorio( $pessoa ); //dbg($sql,1);
		$dados 	   = $db->carregar( $sql );

		if($dados[0]['itemdescricao']!=''){
			$rel = new montaRelatorio();
			$rel->setAgrupador($agrupador, $dados);
			$rel->setColuna($coluna);
			$rel->setTolizadorLinha(false);
			$rel->setEspandir(true);
			$rel->setMonstrarTolizadorNivel(true);

			$corpo = $rel->getRelatorio();

			$html = '<html>
							'./*$dest.*/'<br>
							<center>'.monta_cabecalho_relatorio( '95' )
							.'</center>'.$corpo.
					'</html>';
			$html = str_replace(Array('src="../imagens/brasao.gif"','../imagens/seta_filho.gif'),
								Array('src="simec.mec.gov.br/imagens/brasao.gif"','simec.mec.gov.br/imagens/seta_filho.gif'),$html);
//			echo $html;
//			die();
	//		echo $html;
			$e->limpaEmailsDestino();
			$e->setTitle('Pendências de checklist da semana no módulo ENEM do Sistema ' SIGLA_SISTEMA);
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA);
			$e->setEmailOrigem($_SESSION['email_sistema']);
			$e->setEmailsDestino(Array($_SESSION['email_sistema']));
//			$e->setEmailsDestino(Array($dest));
			$e->enviarEmails();
//			echo $dados[0]['itemdescricao'].' foi';
		}
	}
}

die();

?>