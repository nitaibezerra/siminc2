<?php


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
$numdias = number_format( LIMITE_HORAS_ALERTA_TRAMITACAO / 24, 0, ",", "." );
monta_titulo( $titulo_modulo, 'Solicitações com mais de ' . $numdias . ' dias na mesma situação' );

$sql = "
	SELECT
		'<img align=\"absmiddle\" src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"abrirDadosPA(' || a.atiid || ')\" title=\"Abrir PA\"/>' as acao,
		a.atidescricao,
		a.atinumeropi,
		aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod as codppa,
		coalesce( a.estuf, e.esfdsc, ' - ' ) as estado,
		ue.uexdsc as unidadeexecutora,
		TO_CHAR( atidatacadastro, 'DD/MM/YYYY') as atidatacadastro,
		usu.usunome,
		ed.esddsc,
	(SELECT case when ( now() - hdoc.htddata ) > interval '" . LIMITE_HORAS_ALERTA_TRAMITACAO . " hours' 
			then '' || to_char(hdoc.htddata, 'DD/MM/YYYY') || '' 
			else to_char(hdoc.htddata, 'DD/MM/YYYY') 
		end 
		FROM workflow.historicodocumento hdoc 
		WHERE hdoc.docid = a.docid ORDER BY hstid DESC LIMIT 1 )as datasituacao,
	(SELECT usu.usunome
		FROM workflow.historicodocumento hdoc 
		JOIN seguranca.usuario usu on usu.usucpf = hdoc.usucpf
		WHERE hdoc.docid = a.docid ORDER BY hstid DESC LIMIT 1 )as cpfsituacao
	FROM pde.atividade a
	LEFT JOIN monitora.acao aca on aca.acaid = a.acaid
	LEFT JOIN planointerno.esfera e ON e.esfid = a.esfid
	LEFT JOIN planointerno.unidadeexecutora ue ON ue.uexid = a.uexid
	LEFT JOIN seguranca.usuario usu ON usu.usucpf = a.usucpf
	LEFT JOIN workflow.documento d ON a.docid = d.docid 
	LEFT JOIN workflow.estadodocumento ed ON coalesce(d.esdid, " . WORKFLOW_EM_ELABORACAO . ") = ed.esdid 
	WHERE
		a.atiidpai is null and
		a.atistatus = 'A' and
		a.atiid <> " . PROJETO_PDE . "
		AND ed.esdid NOT IN ( " . IGNORAR_ESTADOS_ALERTA_TRAMITACAO . " )
	ORDER BY
		( SELECT hdoc.htddata FROM workflow.historicodocumento hdoc WHERE hdoc.docid = a.docid ORDER BY hstid DESC LIMIT 1 ) ASC
";

$cabecalho = array("Ação", "Título", "PI", "Funcional Programática", "UF", "Unid. Executora", "Data Cadastro", "Quem Cadastrou", "Situação Atual", "Data Situação", "Quem fez");
$db->monta_lista_simples( $sql, $cabecalho, 1000, 20 );

?>