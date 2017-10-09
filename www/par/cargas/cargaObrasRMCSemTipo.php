<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/par/_funcoesPar.php";
include_once APPRAIZ . "www/par/_funcoes.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '';
$_SESSION['usucpf'] 		= '';

$db = new cls_banco();

$sql = "select
			p.preid,
		    p.predescricao,
		    p.muncod,
		    p.docid,
		    p.ptoid,
		    sr.ptoidsolicitado,
		    e.esddsc,
			(select count(preid) from obras.preobra where preidpai = p.preid) as totalobrapai
		from obras.preobra p
			inner join workflow.documento d on d.docid = p.docid
		    inner join workflow.estadodocumento e on e.esdid = d.esdid
		    inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid
		where
			/*d.esdid = 1486
		    and*/ (e.esddsc not ilike '%solicitação%' and e.esddsc ilike '%RMC%')
		    and p.ptoid not in (73, 74)";
		    
$arrObrasMi = $db->carregar($sql);
$arrObrasMi = $arrObrasMi ? $arrObrasMi : array();

foreach ($arrObrasMi as $v) {
	$sql = "SELECT
				pre.predescricao,
				pto.ptodescricao,
				ent.entemail,
				obr.obrid
			FROM
				obras.preobra pre
			INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid
			INNER JOIN obras2.obras						obr ON obr.preid = pre.preid
			INNER JOIN par.instrumentounidade 			inu ON (inu.muncod = pre.muncodpar AND pre.tooid = 1) OR (inu.estuf = pre.estufpar AND pre.tooid <> 1)
			INNER JOIN par.instrumentounidadeentidade	iue ON iue.inuid = inu.inuid
			LEFT  JOIN entidade.entidade				ent ON ent.entnumcpfcnpj = iue.iuecnpj AND ent.entemail IS NOT NULL
			WHERE
				pre.preid = {$v['preid']}";
	
	$arrDados = $db->pegaLinha( $sql );
	
	if( $v['totalobrapai'] < 1 ){
	    include_once APPRAIZ . "par/classes/modelo/PreObra.class.inc";
		$objPreObra = new PreObra( $v['preid'] );
		$novoPreid = $objPreObra->criarBkp();
	}
	
	$ptoid = $v['ptoidsolicitado'];
		
	$sql = "UPDATE obras.preobra SET ptoid = ".($ptoid ? $ptoid : 'null')." WHERE preid = {$v['preid']};";
	$db->executar( $sql );
	$db->commit();
	
	$sql = "SELECT
				obrid,
				doc.esdid,
				doc.docid
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = {$v['preid']}
				AND obrstatus = 'A'";
	
	$arObra = $db->pegaLinha( $sql );
	
	if( $arObra['obrid'] && $arObra['esdid'] != 768 ){
	
		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = 768
					AND esdidorigem = {$arObra['esdid']}";
		
		$aedid = $db->pegaUm($sql);
		
		if( $aedid == '' ){
		$sql = "INSERT INTO workflow.acaoestadodoc
					(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
					esdsncomentario, aedvisivel, aedcodicaonegativa)
				VALUES
					({$arObra['esdid']}, 768, 'Enviar para reformulação', 'A', 'Enviada para reformulação',
					true, false, false )
				RETURNING
					aedid";
		
			$aedid = $db->pegaUm($sql);
		}
		
		include_once APPRAIZ . 'includes/workflow.php';
		
		$teste = wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por wf_pos_refurmulaPreObra_miparaconvencional preid = '.$v['preid'], array( 'docid' => $arObra['docid'] ) );
		$db->commit();
	}
	
	$texto = '
			<html>
				<head>
					<title></title>
				</head>
				<body>
					<table style="width: 100%;">
						<thead>
							<tr>
								<td style="text-align: center;">
									<p><img  src="http://simec.mec.gov.br/imagens/brasao.gif" width="70"/><br/>
									<b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
									FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO<br/>
									DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS<br/>
									COORDENAÇÃO GERAL DE INFRAESTRUTURA EDUCACIONAL<br/> 
									SBS Quadra 02 - Bloco F - 14º andar - Edifício FNDE - CEP -70070-929<br/>
								</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td style="line-height: 15px;">
								</td>
							</tr>
							<tr>
								<td style="line-height: 15px; text-align:justify">
									<p>Sr(a). Gestor,<br> a obra ('.$arrDados['obrid'].') Construção de Creche '.$arrDados['predescricao'].' está aberta para reformulação, possibilitando a alteração do projeto para construção em metodologia convencional e posterior realização do processo licitatório para execução da mesma.</p>
									                                                        
									<p>Para obter informações de como preencher o sistema, acesse o manual disponibilizado na página inicial do SIMEC-módulo PAR.</b>
								</td>
							</tr>
							<tr>
								<td style="padding: 10px 0 0 0;">
									Atenciosamente,
								</td>
							</tr>
							<tr>
								<td style="text-align: center; padding: 10px 0 0 0;">
									<img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="http://simec.mec.gov.br/imagens/obras/assinatura-fabio.png" />
									<br />
									FÁBIO LÚCIO DE A. CARDOSO<br>
									Coordenador-Geral de Infraestrutura Educacional - CGEST<br>
									Diretoria de Gestão, Articulação e Projetos Educacionais - DIGAP<br>
									Fundo Nacional de Desenvolvimento da Educação-FNDE<br>
								</td>
							</tr>
						</tbody>
					</table>
				</body>
			</html>';
	
	
	$assunto  = "Reformulação da obra ({$arrDados['obrid']}) Construção de Creche Metodologias Inovadoras";
	
	$email = Array($arrDados['entemail']);
	
	if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
		$email = array($_SESSION['email_sistema']);
	}
	enviar_email(array('nome'=>SIGLA_SISTEMA. ' - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $texto, $cc, $cco );
	
	
}