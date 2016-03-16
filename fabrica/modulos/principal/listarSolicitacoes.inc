<?php

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if ($_REQUEST['manterPesquisa'] == null || $_REQUEST['manterPesquisa'] !=true){
	unset($_SESSION['REQUEST']);
}

if (!empty($_POST)){
	$_SESSION['REQUEST']=$_REQUEST;
}else{
	if (!empty($_SESSION['REQUEST'])){
		$_REQUEST =$_SESSION['REQUEST'];
	}
}

//trata combo_popup para paginação e ordenação por coluna
		//campo Requisitante
if(!$_POST['numero'] && !$_POST['ordemlista'])
{
    if($_POST['usunomerequisitante'][0]){
        for( $i = 0; $i < count( $_POST['usunomerequisitante']);  $i++ ){
            $usunomerequisitantex .= "|" . $_POST['usunomerequisitante'][$i];
        }
        $_SESSION['usunomerequisitanteaux'] = substr($usunomerequisitantex, 1);
    }
    else{
        $_SESSION['usunomerequisitanteaux'] = "";
    }
}

if($_SESSION['usunomerequisitanteaux']){
    $usunomerequisitantex = str_replace("|","','",$_SESSION['usunomerequisitanteaux']);
    //parametro para query
    $sqlx = "SELECT
                u.usunome AS codigo,
                u.usunome AS descricao
            FROM
                seguranca.usuario u
            Where u.usunome in ('".$usunomerequisitantex."')
            GROUP BY u.usunome
            order by u.usunome";
    $_POST['usunomerequisitante'] = $db->carregarColuna($sqlx);
    //preenche combopopup
    $usunomerequisitante = $db->carregar($sqlx);
}

//campo Fiscais
if(!$_POST['numero'] && !$_POST['ordemlista']){
    if($_POST['usufiscal'][0]){
        for($i=0; $i<count($_POST['usufiscal']);  $i++){
            $usufiscalx .= "|" . $_POST['usufiscal'][$i];
        }
        $_SESSION['usufiscalaux'] = substr($usufiscalx, 1);
    }
    else{
        $_SESSION['usufiscalaux'] = "";
    }
}
if($_SESSION['usufiscalaux']){
    $usufiscalx = str_replace("|","','",$_SESSION['usufiscalaux']);
    //parametro para query
    $sqlx = "SELECT
                u.usucpf AS codigo,
                u.usunome AS descricao
            FROM
                seguranca.usuario u
            Where u.usucpf in ('".$usufiscalx."')
            GROUP BY u.usucpf, u.usunome
            order by u.usunome";
    $_POST['usufiscal'] = $db->carregarColuna($sqlx);
    //preenche combopopup
    $usufiscal = $db->carregar($sqlx);
}

//campo sistemas
if(!$_POST['numero'] && !$_POST['ordemlista']){
    if($_POST['sidid'][0]){
        for($i=0; $i<count($_POST['sidid']);  $i++){
            $sididx .= "|" . $_POST['sidid'][$i];
        }
        $_SESSION['sididaux'] = substr($sididx, 1);
    }
    else{
        $_SESSION['sididaux'] = "";
    }
}
if($_SESSION['sididaux']){
    $sididx = str_replace("|","','",$_SESSION['sididaux']);
    //parametro para query
    $sqlx = "select
                sidid as codigo,
                siddescricao as descricao
            from
                demandas.sistemadetalhe
            Where sidid in ('".$sididx."')
            order by siddescricao";
    $_POST['sidid'] = $db->carregarColuna($sqlx);
    //preenche combopopup
    $sidid = $db->carregar($sqlx);
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="./js/jquery.dataTables.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script type="text/javascript">
    
$(document).ready(function(){
    $('#numeroSolicitacao').focus();
});
    

function limparCampos(){
	$('input:not(:submit,:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
	$('#radioSituacao').click();
	//$('input[name="situacaoPausada[]"]').attr('checked',false);
}

function submeteFormulario()
{
	$('#usunomerequisitante option').each(function()
	{
		$(this).attr('selected',true);
	});
	$('#usufiscal option').each(function()
	{
		$(this).attr('selected',true);
	});
	$('#sidid option').each(function()
	{
		$(this).attr('selected',true);
	});
	$('#form_listar_contratos').submit();
}


</script>
<?php

$titulo_modulo = "Listar Solicitações";
monta_titulo($titulo_modulo, '');



//recupera perfis de usuario
$pfls = arrayPerfil();

?>

<form name="formulario" id="form_listar_contratos" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Número da solicitação</td>
			<td><input id="numeroSolicitacao" class="normal" type="text" name="scsid" value="<?php echo $_REQUEST['scsid']; ?>"></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Unidade do requisitante</td>
			<td><?php

				$sql = "SELECT ungcod as codigo, ungcod || ' - ' || ungdsc as descricao FROM public.unidadegestora ORDER BY ungdsc";
				$db->monta_combo('ungcod', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'ungcod', '', $_REQUEST['ungcod']);
		  ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Requisitante</td>
			<td>
				<?php
				$sql_combo = "SELECT
								usu.usunome as codigo,
								usu.usunome as descricao
							 FROM
							 	seguranca.usuario usu
							 WHERE
							 	usu.usucpf in (select usucpfrequisitante from fabrica.solicitacaoservico) order by usu.usunome";
				combo_popup( 'usunomerequisitante', $sql_combo, 'Selecione o(s) requisitante(s)', '400x400', 0, array(), '', 'S', true, true, 5, 400, null, null, null, null, null, true, false, null, true);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Fiscal(is) Responsável(is)</td>
			<td>
				<?php
				$sql_combo = "SELECT
								usu.usucpf as codigo,
								usu.usunome as descricao
							 FROM
							 	seguranca.usuario usu
							 WHERE
							 	usu.usucpf in (select usucpf from fabrica.fiscalsolicitacao) order by usu.usunome";
				combo_popup( 'usufiscal', $sql_combo, 'Selecione o(s) fiscal(is)', '400x400', 0, array(), '', 'S', true, true, 5, 400, null, null, null, null, null, true, false, null, true);
				?>
			</td>
		</tr>
		<tr>
                    <td class="SubtituloDireita" width="25%">C&eacute;lula Respons&aacute;vel </td>
                    <td>
                        <?php
                        
                        if($_REQUEST['celid']) $celid = $_REQUEST['celid'];
                        
                        $sql = "SELECT distinct cel.celid as codigo, cel.celnome as descricao
                                FROM demandas.celula cel 
                                INNER JOIN demandas.sistemacelula sisce 
                                    ON cel.celid = sisce.celid 
                                INNER JOIN demandas.sistemadetalhe sisde 
                                    ON sisde.sidid = sisce.sidid 
                                INNER JOIN fabrica.solicitacaoservico ss 
                                    ON sisde.sidid = ss.sidid 
                                ORDER BY cel.celnome";
                                $db->monta_combo('celid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'celid', '');
                        ?>
                    </td>
		</tr>

		<tr>
			<td class="SubtituloDireita" width="25%">Solicitação</td>
			<td><input class="normal" id="scsnecessidade" type="text" name="scsnecessidade" value="<?php echo $_REQUEST['scsnecessidade']; ?>" size="60"></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Justificativa</td>
			<td><input class="normal" id="scsjustificativa" type="text" name="scsjustificativa" value="<?php echo $_REQUEST['scsjustificativa']; ?>" size="60"></td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">Data de Previs&atilde;o de T&eacute;rmino</td>
			<td><?php
					echo campo_data2('scsprevatendimento','N', 'S', 'Expectativa de atendimento', 'S', '', '', formata_data_sql($_REQUEST['scsprevatendimento']) );
				?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Período de abertura</td>
			<td><?php
					echo campo_data2('dtini','N', 'S', 'Data início', 'S', '', '', formata_data_sql($_REQUEST['dtini']) );
					echo " à ";
					echo campo_data2('dtfim','N', 'S', 'Data Fim', 'S', '', '', formata_data_sql($_REQUEST['dtfim']) );
				?></td>
		</tr>

		<tr>
			<td class="SubtituloDireita" width="25%">Situação</td>
			<td>
			<?php
					if($_REQUEST['esdid']) $esdid = $_REQUEST['esdid'];

					$sql = "SELECT esd.esdid as codigo,esd.esddsc as descricao
							FROM workflow.estadodocumento esd
							WHERE tpdid in (select tpdid from workflow.tipodocumento where tpdid = ".TIPO_SITUACAO_SOLICITACAO." and sisid = ".SISID_FABRICA.")
                            AND esd.esdstatus = 'A'
							order by esd.esdordem";
                    
				  	$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdid', '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Sistema</td>
			<td>
				<?php
					$sql_combo = "SELECT sidid as codigo, siddescricao as descricao
                                  FROM demandas.sistemadetalhe
                                  WHERE sidstatus = 'A'
                                  ORDER BY siddescricao";
					$wherePesq = array( array(
                        "codigo" 	=> "siddescricao",
                        "descricao" => "Sistema:",
                        "numeric"	=> false
                    ));
					combo_popup( 'sidid', $sql_combo, 'Selecione o(s) Sistema(s)', '400x400', 0, array(), '', 'S', true, true, 5, 400, null, null, null, $wherePesq, null, true, false, null, true);
				?>
			</td>
		</tr>
		<tr>
			<td></td>
			<td >
				<input type="submit" value="Pesquisar" onclick="submeteFormulario();">
				&nbsp;
				<input type="button" value="Limpar" id="limpar" onclick="limparCampos();">
			</td>
		</tr>
	</table>
	<input type="hidden" name="pesquisar" value="pesquisar">
</form>

<?php

//permissao para EXCLUSÃO
$btnExcluir = '0';
$joinCelula = '';
$arrWhere   = array( "scsstatus = 'A'" );

if( isset($_REQUEST['pesquisar']) )
{

	if($_POST['usunomerequisitante'][0] != "" && $_POST['usunomerequisitante'] != "Array")
	{
		$_POST['usunomerequisitante'] = "'" . implode("','",$_POST['usunomerequisitante']) . "'";
		$arrWhere[] =  " u.usunome IN (".$_POST['usunomerequisitante'].") ";
	}
    
	if($_POST['usufiscal'][0] != "" && $_POST['usufiscal'] != "Array")
	{
		$_POST['usufiscal'] = "'" . implode("','",$_POST['usufiscal']) . "'";
		$arrWhere[] =  "s.scsid IN (select scsid from fabrica.fiscalsolicitacao where usucpf in (".$_POST['usufiscal'].") ) ";
	}
    
	if($_POST['sidid'][0] != "" && $_POST['sidid'] != "Array")
	{
		$_POST['sidid'] = "'" . implode("','",$_POST['sidid']) . "'";
		$arrWhere[] =  "s.sidid IN (".$_POST['sidid'].") ";
	}

	if($_REQUEST['esdid'])
	{
		$arrWhere[] =  "ed.esdid = ".$_REQUEST['esdid']." ";
	}

    if($_REQUEST['scsnecessidade']){
		$arrWhere[] =  "s.scsnecessidade ILIKE '%{$_REQUEST['scsnecessidade']}%' ";

	}
	
    if($_REQUEST['ungcod']){
		$arrWhere[] =  "u.ungcod = '{$_REQUEST['ungcod']}' ";

	}
	
    if($_REQUEST['scsjustificativa']){
		$arrWhere[] =  "s.scsjustificativa ILIKE '%{$_REQUEST['scsjustificativa']}%' ";

	}
	
    if($_REQUEST['scsprevatendimento']){
		$arrWhere[] =  "( to_char(a.ansprevtermino,'YYYY-MM-DD') = '".formata_data_sql($_REQUEST['scsprevatendimento'])."' 
                    OR 
                    to_char(os.odsdtprevtermino, 'YYYY-MM-DD') = '".formata_data_sql($_REQUEST['scsprevatendimento'])."'  
                 ) ";

	}
    
	if($_REQUEST['dtini'] && $_REQUEST['dtfim'])
    {
		$arrWhere[] =  "s.dataabertura >= '".formata_data_sql($_REQUEST['dtini'])."' ";
		$arrWhere[] =  "s.dataabertura <= '".formata_data_sql($_REQUEST['dtfim'])."' ";
	}

	if($_REQUEST['prgcod'])
    {
		$arrWhere[] =  "s.prgcod = '{$_REQUEST['prgcod']}' ";
	}
    
	if(is_numeric($_REQUEST['scsid']))
    {
		$arrWhere[] =  "s.scsid = '{$_REQUEST['scsid']}' ";
	}
    
    if(!empty( $_REQUEST['celid'] ))
    {
        $joinCelula = 'LEFT JOIN demandas.sistemacelula sisce 
                            ON sid.sidid = sisce.sidid 
                        LEFT JOIN demandas.celula cel
                            ON sisce.celid = cel.celid';
		$arrWhere[] =  "cel.celid = {$_REQUEST['celid']} ";
	}
}

//filtro PERFIL_REQUISITANTE
if(in_array(PERFIL_REQUISITANTE,$pfls) && !in_array(PERFIL_SUPER_USUARIO,$pfls) && !in_array(PERFIL_ADMINISTRADOR, $pfls)){
	$arrWhere[] =  "(s.usucpfrequisitante = '{$_SESSION['usucpf']}' OR s.usucpforigem = '{$_SESSION['usucpf']}') ";
}

if(in_array(PERFIL_ESPECIALISTA_SQUADRA,$pfls) && !in_array(PERFIL_SUPER_USUARIO,$pfls) && !in_array(PERFIL_ADMINISTRADOR, $pfls)){
	$entids = $db->carregarColuna("select entid from fabrica.usuarioresponsabilidade where rpustatus = 'A' and usucpf = '{$_SESSION['usucpf']}'");
	if($entids){
		$ctrids = $db->carregarColuna("select ctrid from fabrica.contrato where entidcontratado in (".implode(',', $entids).")");
		$arrWhere[] =  "( a.ctrid in (".implode(',', $ctrids).") ) ";
	}
}


$sql = "SELECT '<center>
				        ' || CASE d.esdid
						WHEN ".WF_ESTADO_ELABORACAO." THEN
							CASE d.esdid
								WHEN ".WF_ESTADO_ELABORACAO." THEN '<img src=\"/imagens/alterar.gif\" border=0 title=\"Em elaboração\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'\'\">' ELSE '<img src=\"/imagens/alterar_01.gif\" border=0 title=\"Em elaboração\">'
							END
				        WHEN ".WF_ESTADO_PRE_ANALISE." THEN
							CASE s.scsid
								WHEN NULL THEN '<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Pré-análise\">' ELSE '<img src=\"/imagens/consultar.gif\" border=0 title=\"Pré-análise\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'\'\">'
							END
				        WHEN ".WF_ESTADO_ANALISE." THEN
							CASE s.scsid
								WHEN NULL THEN '<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Em analise\">' ELSE '<img src=\"/imagens/consultar.gif\" border=0 title=\"Em analise\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'\'\">'
							END
						WHEN ".WF_ESTADO_DETALHAMENTO." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/editar_nome_desabilitada.gif\" border=0 title=\"Em detalhamento\">' ELSE '<img src=\"/imagens/editar_nome.gif\" border=0 title=\"Em detalhamento\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						WHEN ".WF_ESTADO_OS_PAUSA." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/editar_nome_desabilitada.gif\" border=0 title=\"Em detalhamento\">' ELSE '<img src=\"/imagens/editar_nome.gif\" border=0 title=\"Em detalhamento\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						WHEN ".WF_ESTADO_SS_PAUSA." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/editar_nome_desabilitada.gif\" border=0 title=\"Em detalhamento\">' ELSE '<img src=\"/imagens/editar_nome.gif\" border=0 title=\"Em detalhamento\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END						
							
						WHEN ".WF_ESTADO_AVALIACAO." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/editar_nome_desabilitada.gif\" border=0 title=\"Em avaliação\">' ELSE '<img src=\"/imagens/editar_nome.gif\" border=0 title=\"Em avaliação\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						WHEN ".WF_ESTADO_APROVACAO." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/editar_nome_desabilitada.gif\" border=0 title=\"Em aprovação\">' ELSE '<img src=\"/imagens/editar_nome.gif\" border=0 title=\"Em aprovação\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						WHEN ".WF_ESTADO_EXECUCAO." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/editar_nome_desabilitada.gif\" border=0 title=\"Em execução\">' ELSE '<img src=\"/imagens/editar_nome.gif\" border=0 title=\"Em execução\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						WHEN ".WF_ESTADO_FINALIZADA." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Finalizada\">' ELSE '<img src=\"/imagens/consultar.gif\" border=0 title=\"Finalizada\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						WHEN ".WF_ESTADO_AGUARDANDO_PAGAMENTO." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Aguardando Pagamento\">' ELSE '<img src=\"/imagens/consultar.gif\" border=0 title=\"Aguardando Pagamento\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						WHEN ".WF_ESTADO_CANCELADA_SEM_CUSTO." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Cancelada Sem Custo\">' ELSE '<img src=\"/imagens/consultar.gif\" border=0 title=\"Cancelada Sem Custo\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						WHEN ".WF_ESTADO_CANCELADA_COM_CUSTO." THEN
							CASE a.ansid
								WHEN NULL THEN '<img src=\"/imagens/consultar_desabilitado.gif\" border=0 title=\"Cancelada Com Custo\">' ELSE '<img src=\"/imagens/consultar.gif\" border=0 title=\"Cancelada Com Custo\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid||'\'\">'
							END
						END
						||
						CASE WHEN $btnExcluir = '1' THEN
								' <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\" border=\"0\" onclick=\"Excluir(\'fabrica.php?modulo=principal/listarSolicitacoes&acao=A&requisicao=removerSolicitacaoServico&scsid='||s.scsid||'\',\'Deseja realmente excluir a solicitação?\');\" /></center>'
							 ELSE
							 	'</center>'
						END,

                    '<span style= \"display:none;\">'||lpad(s.scsid::text, 8, '0')||'</span><div style=\"white-space: nowrap; color: #0066CC;\">'
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_ELABORACAO.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_PRE_ANALISE.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid|| '\'\">'||s.scsid||'</p>' ELSE '' END)                        
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_ANALISE.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_DETALHAMENTO.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_OS_PAUSA.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_SS_PAUSA.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_AVALIACAO.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_APROVACAO.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_EXECUCAO.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_FINALIZADA.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_AGUARDANDO_PAGAMENTO.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_CANCELADA_SEM_CUSTO.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                        || (CASE WHEN (d.esdid = ".WF_ESTADO_CANCELADA_COM_CUSTO.") THEN '<p title=\"Editar S.S.\" style=cursor:pointer; onclick=\"window.location.href=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='||s.scsid||'&ansid='||a.ansid|| '\'\">'||s.scsid||'</p>' ELSE '' END)
                    || '</div>' AS scsid,
               s.scsnecessidade,
			   sid.siddescricao,
			   CASE WHEN s.usucpforigem=s.usucpfrequisitante THEN u.usunome ELSE u.usunome||' >> '||COALESCE((SELECT usunome FROM seguranca.usuario WHERE usucpf=s.usucpforigem),'NÃO CADASTRADO') END,
			   '<span style= \"display:none;\">' || s.dataabertura  ||'</span>' || to_char(s.dataabertura,'dd/mm/YYYY') as dataabertura,
			   --to_char(s.scsprevatendimento,'dd/mm/YYYY') as scsprevatendimento,
			    '<span style= \"display:none;\">' || to_char(a.ansprevtermino,'YYYY-MM-DD')  ||'</span>' ||
			   (CASE ed.esddsc
			 		WHEN 'Finalizada'
			 		THEN
			 			'<font title=\"\">' || to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '<font>'
			 			
			 		WHEN 'Em Análise'
			 		THEN
				 		(CASE WHEN a.ansprevtermino is not null THEN
								(CASE WHEN to_char(a.ansprevtermino::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
						 			THEN '<font color=\"#FBB917\" title=\"\">' || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
						 			ELSE
										(CASE WHEN a.ansprevtermino < CURRENT_DATE THEN
													'<font color=\"red\" title=\"\">' || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
								 			  ELSE
								 			  		'<font color=\"green\" title=\"\">'  || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
								 		END)
						 		END)
						END)
						
					WHEN 'Em Revisão'
			 		THEN
				 		(CASE WHEN a.ansprevtermino is not null THEN
								(CASE WHEN to_char(a.ansprevtermino::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
						 			THEN '<font color=\"#FBB917\" title=\"\">' || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
						 			ELSE
										(CASE WHEN a.ansprevtermino < CURRENT_DATE THEN
													'<font color=\"red\" title=\"\">' || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
								 			  ELSE
								 			  		'<font color=\"green\" title=\"\">'  || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
								 		END)
						 		END)
						END)
						
					WHEN 'Em Detalhamento'
			 		THEN
				 		(CASE WHEN a.ansprevtermino is not null THEN
								(CASE WHEN to_char(a.ansprevtermino::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
						 			THEN '<font color=\"#FBB917\" title=\"\">' || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
						 			ELSE
										(CASE WHEN a.ansprevtermino < CURRENT_DATE THEN
													'<font color=\"red\" title=\"\">' || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
								 			  ELSE
								 			  		'<font color=\"green\" title=\"\">'  || to_char(a.ansprevtermino, 'DD/MM/YYYY') || '</font>'
								 		END)
						 		END)
						END)
						
				   ELSE
					   (CASE WHEN os.odsdtprevtermino is not null THEN
									(CASE WHEN to_char(os.odsdtprevtermino::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
							 			THEN '<font color=\"#FBB917\" title=\"\">' || to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '</font>'
							 			ELSE
											(CASE WHEN os.odsdtprevtermino < CURRENT_DATE THEN
														'<font color=\"red\" title=\"\">' || to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '</font>'
									 			  ELSE
									 			  		'<font color=\"green\" title=\"\">'  || to_char(os.odsdtprevtermino, 'DD/MM/YYYY') || '</font>'
									 		END)
							 		END)
						END)
				END) as dataprev,
			   ed.esddsc
			   
		FROM fabrica.solicitacaoservico s
		LEFT JOIN (SELECT MAX(odsdtprevtermino) as odsdtprevtermino, scsid, tosid FROM fabrica.ordemservico where fabrica.ordemservico.scsid = scsid GROUP BY scsid, tosid) as os ON s.scsid = os.scsid and os.tosid = 1
		LEFT JOIN fabrica.analisesolicitacao a ON a.scsid=s.scsid
		LEFT JOIN seguranca.usuario u ON u.usucpf=s.usucpfrequisitante
		LEFT JOIN workflow.documento d ON d.docid=s.docid
		LEFT JOIN workflow.estadodocumento ed ON ed.esdid=d.esdid
		LEFT JOIN demandas.sistemadetalhe sid ON sid.sidid=s.sidid        
        {$joinCelula}
        LEFT JOIN
                 ( 
                    SELECT obs.scsid,  hps.hpstipo , obs.obsdata as datapausa
                    FROM fabrica.historicopausaservico hps
                    INNER JOIN fabrica.observacoes obs
                        ON hps.obsid = obs.obsid
                    INNER JOIN 
                        ( 
                            SELECT obs.scsid, MAX( hps.hpsid ) AS id
                            FROM fabrica.historicopausaservico hps
                            INNER JOIN fabrica.observacoes obs
                                ON hps.obsid = obs.obsid 
                            GROUP BY obs.scsid 
                        ) as historico
                        ON  hps.hpsid = historico.id) AS situacao
                    ON s.scsid = situacao.scsid
          WHERE ". implode( ' AND ' , $arrWhere ) ."
          ORDER BY s.scsid DESC";

//ver($sql);

$cabecalho     = array("Ações", "Nº", "Necessidade", "Sistema", "Nome", "Data abertura", "Data de Previsão de Término", "Situação");
$tamanho       = array("5%","5%","35%","15%","15%","5%","10%","10%");
$alinhamento   = array("center","center","left","center","center","center","center","center");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2,"",$tamanho,$alinhamento);