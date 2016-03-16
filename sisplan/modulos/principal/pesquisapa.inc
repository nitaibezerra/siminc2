<?
/**
 * Desenvolvedor: MESOTEC Informática LTDA
 * Analistas: Adonias Malosso <adonias@mesotec.com.br>, Davison Silva <davison@mesotec.com.br>
 * Programadores: Adonias Malosso <adonias@mesotec.com.br>, Halisson Gomides <halisson@mesotec.com.br>, Paulo Estevao <paulo@mesotec.com.br>
 * Baseado no SIMEC
 */

// direcionando o user pra uma pagina com os planejamentos se o exercicio for 2013 ou mais
if( (int) $_SESSION['exercicio'] >= 2013 ){
    echo "<script>
                window.location='sisplan.php?modulo=principal/consultarDetalhamento&acao=C';
          </script>";
}

unset( $_SESSION['REL_PESQUISAPA'] );

// SELECIONA PROJETO
if ( $_REQUEST['selecionar_projeto'] )
{
	/**
	* @TODO: verificar se o projeto existe
	*/
	$_SESSION['projeto'] = (integer) $_REQUEST['selecionar_projeto'];
	redirecionar( 'principal/atividade_/arvore', 'A' );
}
else
{
	$_SESSION['projeto'] = null;
}


////////////////   REMOVER PA   ////////////////
if ( $_REQUEST['remover'] == 'R' )
{
	$sql = "UPDATE pde.atividade SET atistatus = 'I' WHERE atiid = ".$_REQUEST['atiid'];
	$db->executar($sql);
	$db->commit();

	$sql = "SELECT count(*) as qtd FROM pde.atividade a JOIN workflow.historicodocumento hd on a.docid = hd.docid WHERE aedid = 55  AND a.atiid = ".$_REQUEST['atiid'];

	if ( $db->pegaUm($sql,0) >= 1 )
		toXmlMinc( $_REQUEST['atiid'], 'Atividade' );

}
////////////////REMOVER PA - FIM////////////////


$permissao_formulario = 'S';
$permissao_uex = 'S';


$uexid = null;

if (usuario_possui_perfil( PERFIL_GESTORPI ) ){
	$sql = "SELECT
				u.uexid
			FROM
				planointerno.unidadeexecutora u
			join
				sisplan.usuarioresponsabilidade ur on ur.uexid = u.uexid
			WHERE
				u.uexstatus = 'A'
				and ur.usucpf = '" . $_SESSION['usucpf'] . "'";

	$uexidArr = (array) $db->carregarColuna($sql);
	if ($uexidArr[0]){
		$uexidWhere = " a.uexid IN (" . ( implode(",", $uexidArr) ) . ")";
	}
}

if ( !$db->testa_superuser() && !usuario_possui_perfil( PERFIL_GESTORPI ) && !usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) && !usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) && !usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) && !usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) )
{
	$uexid = (int)$db->pegaUm( "SELECT uexid FROM seguranca.usuario WHERE usucpf = '" . $_SESSION['usucpf'] . "'" );
	$permissao_uex = ((!usuario_possui_perfil( PERFIL_GESTORPI ) && !usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) && !usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) && !usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) && !usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO )) ? 'N' : 'S');
}elseif ( $_POST['uexid'] && ( $db->testa_superuser() || usuario_possui_perfil( PERFIL_GESTORPI ) || usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) || usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) || usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) || usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) ){
	if (usuario_possui_perfil( PERFIL_GESTORPI )){
		if ( in_array($_POST['uexid'], $uexidArr) ){
			$uexid = (int) $_POST['uexid'];
		}
	}else{
		$uexid = (int) $_POST['uexid'];
	}
}



//dbg($_POST,1);
$where = array( "a.atianopi = '" . $_SESSION['exercicio'] . "'", "aca.prgano = '" . $_SESSION['exercicio'] . "'" );

if ($_REQUEST['atiid']){
	$atiid = $_REQUEST['atiid'];
	$where[] = "a.atiid = '" . $_REQUEST['atiid'] . "'";
}

if ($_REQUEST['atidescricao']){
	$atidescricao = $_REQUEST['atidescricao'];
	$where[] = "a.atidescricao ilike '%" . $_REQUEST['atidescricao'] . "%'";
}

if ($_REQUEST['atinumeropi']){
	$atinumeropi = $_REQUEST['atinumeropi'];
	$where[] = "a.atinumeropi = '" . $_REQUEST['atinumeropi'] . "'";
}

if ($_REQUEST['atiprotocolo']){
	$atiprotocolo = $_REQUEST['atiprotocolo'];
	$where[] = "a.atiprotocolo = '" . $_REQUEST['atiprotocolo'] . "'";
}

if ( $_REQUEST['data_inicio'] )
{
	$data_inicio = $_REQUEST['data_inicio'];
	$where[] = "TO_CHAR(a.atidatacadastro, 'DD/MM/YYYY') >= '" . $_REQUEST['data_inicio'] . "'";
}

if ( $_REQUEST['data_fim'] )
{
	$data_fim = $_REQUEST['data_fim'];
	$where[] = "TO_CHAR(a.atidatacadastro, 'DD/MM/YYYY') <= '" . $_REQUEST['data_fim'] . "'";
}

if ( $_REQUEST['situacao'] && $_REQUEST['situacao'] != 'T' )
{
	$atistatuspi = $_REQUEST['atistatuspi'];
	$where[] = "a.atistatuspi = '" . $_REQUEST['situacao'] . "'";
}

if ($_REQUEST['prgcod']){
	$prgcod = $_REQUEST['prgcod'];
	$where[] = "aca.prgcod = '" . $_REQUEST['prgcod'] . "'";
}

if ($_REQUEST['acacod']){
	$acacod = $_REQUEST['acacod'];
	$where[] = "aca.acacod = '" . $_REQUEST['acacod'] . "'";
}

if ($_REQUEST['unicod']){
	$unicod = $_REQUEST['unicod'];
	$where[] = "aca.unicod = '" . $_REQUEST['unicod'] . "'";
}

if ($_REQUEST['loccod']){
	$loccod = $_REQUEST['loccod'];
	$where[] = "aca.loccod = '" . $_REQUEST['loccod'] . "'";
}

if ($_REQUEST['procod']){
	$procod = $_REQUEST['procod'];
	$where[] = "aca.procod = '" . $_REQUEST['procod'] . "'";
}

if ($_REQUEST['esfid']){
	$esfid = $_REQUEST['esfid'];
	$where[] = "a.esfid = '" . $_REQUEST['esfid'] . "'";
}

if ($_REQUEST['paiid']){
	$paiid = $_REQUEST['paiid'];
	$where[] = "a.paiid = '" . $_REQUEST['paiid'] . "'";
}

if ($_REQUEST['estuf']){
	$estuf = $_REQUEST['estuf'];
	$where[] = "a.estuf = '" . $_REQUEST['estuf'] . "'";
}

if ($_REQUEST['estuf'] == 'DF' ){
	if ($_REQUEST['muncod']){
		$muncod = $_REQUEST['muncod'];
		$where[] = "a.rgaid= '" . $_REQUEST['muncod'] . "'";
	}
} else {
	if ($_REQUEST['muncod']){
		$muncod = $_REQUEST['muncod'];
		$where[] = "a.muncod = '" . $_REQUEST['muncod'] . "'";
	}
}

if ($_REQUEST['dfeid']){
	$dfeid = $_REQUEST['dfeid'];
	$where[] = "a.dfeid = '" . $_REQUEST['dfeid'] . "'";
}

if ($_REQUEST['iteid']){
	$iteid = $_REQUEST['iteid'];
	$where[] = "a.iteid = '" . $_REQUEST['iteid'] . "'";
}

if ($_REQUEST['preid']){
	$preid = $_REQUEST['preid'];
	$where[] = "a.preid = '" . $_REQUEST['preid'] . "'";
}

if ($_REQUEST['aesid']){
	$aesid = $_REQUEST['aesid'];
	$where[] = "a.aesid = '" . $_REQUEST['aesid'] . "'";
}

if ($_REQUEST['pddid']){
	$pddid = $_REQUEST['pddid'];
	$where[] = "a.pddid = '" . $_REQUEST['pddid'] . "'";
}

if ($_REQUEST['clsid']){
	$clsid = $_REQUEST['clsid'];
	$where[] = "a.clsid = '" . $_REQUEST['clsid'] . "'";
}

if ($_REQUEST['sbcid']){
	$sbcid = $_REQUEST['sbcid'];
	$where[] = "a.sbcid = '" . $_REQUEST['sbcid'] . "'";
}

if ($_REQUEST['areid']){
	$areid = $_REQUEST['areid'];
	$where[] = "a.areid = '" . $_REQUEST['areid'] . "'";
}

if ($_REQUEST['segid']){
	$segid = $_REQUEST['segid'];
	$where[] = "a.segid = '" . $_REQUEST['segid'] . "'";
}

/////////// SO ENTRA NESSE CAMPO SE FOR MARCADO PRONAC ///////////////////////
if ($_REQUEST['atipronacquest'] == "true")
{
	$where[] = "a.atipronacquest = 'true'";
	if ($_REQUEST['atipronac']){
		$atipronac = $_REQUEST['atipronac'];
		$where[] = "a.atipronac = '" . $_REQUEST['atipronac'] . "'";
	}

	if ($_REQUEST['tppid']){
		$tppid = $_REQUEST['tppid'];
		$where[] = "a.tppid = '" . $_REQUEST['tppid'] . "'";
	}

	if ($_REQUEST['tscid']){
		$tscid = $_REQUEST['tscid'];
		$where[] = "a.tscid = '" . $_REQUEST['tscid'] . "'";
	}
}
elseif ($_REQUEST['atipronacquest'] == "false")
{
	$where[] = "a.atipronacquest = 'false'";
}
/////////////////////////////////////////////////////////////////////////////////


/////////// SO ENTRA NESSE CAMPO SE FOR MARCADO EMENDA ///////////////////////
if ($_REQUEST['atiemenda'] == "true")
{
	$where[] = "a.atiemenda = 'true'";
	if ($_REQUEST['tauid']){
		$tauid = $_REQUEST['tauid'];
		$where[] = "a.tauid = '" . $_REQUEST['tauid'] . "'";
	}

	if ($_REQUEST['ppoid']){
		$ppoid = $_REQUEST['ppoid'];
		$where[] = "a.ppoid = '" . $_REQUEST['ppoid'] . "'";
	}

	if ($_REQUEST['aueid']){
		$aueid = $_REQUEST['aueid'];
		$where[] = "a.aueid = '" . $_REQUEST['aueid'] . "'";
	}
}
elseif ($_REQUEST['atiemenda'] == "false")
{
	$where[] = "a.atiemenda = 'false'";
}
/////////////////////////////////////////////////////////////////////////////////

//if ( $_REQUEST['esdid'] )
//{
//	$esdid = $_REQUEST['esdid'];
//	$where[] = "d.esdid = " . $_REQUEST['esdid'];
//}

// TODO: esdid
if ( $_REQUEST['esdid'][0] != "" )
{
	$where[] = "d.esdid IN ( " . implode(',', $_REQUEST['esdid']) . ") ";
}

if ( $_REQUEST['uexid'][0] != "" )
{
	$where[] = "ue.uexid IN ( " . implode(',', $_REQUEST['uexid']) . ") ";
}

if ( $_REQUEST['atirepresentante'] )
{
	$where[] = "a.atirepresentante ILIKE '%{$_REQUEST['atirepresentante']}%'";
}

$s_where = "";
if ( is_array( $where ) && count( $where ) )
{
	$s_where = implode( " AND ", $where ) . " AND ";
}

$sql = "
select
	a.atiid,
	a.atidescricao,
	a.atianopi,
	a.atinumeropi,
	a.atistatuspi,
	a.atipropameta,
	aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod as codppa,
	p.prodsc,

	u.unmdsc,
	coalesce( a.estuf, e.esfdsc, ' - ' ) as estado,
	ue.uexid,
	ue.uexcod || ' - ' || ue.uexsigla as unidadeexecutora,
	a.atiorcamentocusteio,
	a.atiorcamentocapital,
	a.atiorcamentocusteio + a.atiorcamentocapital as atiorcamentototal,
	TO_CHAR( atidatacadastro, 'DD/MM/YYYY') as atidatacadastro,
	usu.usucpf,
	usu.usunome,
	ed.esddsc,
	ed.esdid,
	coalesce(obra.qtdobras,0) as qtdobras
FROM pde.atividade a
LEFT JOIN monitora.acao aca on aca.acaid = a.acaid
LEFT JOIN public.produto p ON p.procod = aca.procod
LEFT JOIN public.unidademedida u ON u.unmcod = aca.unmcod
LEFT JOIN planointerno.esfera e ON e.esfid = a.esfid
LEFT JOIN planointerno.unidadeexecutora ue ON ue.uexid = a.uexid
LEFT JOIN seguranca.usuario usu ON usu.usucpf = a.usucpf
LEFT JOIN workflow.documento d ON a.docid = d.docid
LEFT JOIN workflow.estadodocumento ed ON d.esdid = ed.esdid
LEFT JOIN ( SELECT _atiprojeto, count(atiid) as qtdobras from pde.atividade where atiobra = true and atistatus = 'A' and atiid != " . PROJETO_PDE . " group by _atiprojeto) obra ON obra._atiprojeto = a.atiid
where
	" . $s_where . "
	a.atiidpai is null and
	a.atistatus = 'A' and
	a.atiid != " . PROJETO_PDE . "
order by
	a.atidescricao
";

//dbg($sql);

if ( $_REQUEST['submitPesquisar'] )
{
	$projetos = $db->carregar( $sql );
	$projetos = $projetos ? $projetos : array();
	foreach ( $projetos as $chave => $projeto ) {
		$sql = "select count(*) from pde.atividade where atiidpai = " . $projeto['atiid'] . " and atistatus = 'A'";
		$projetos[$chave]['possui_atividade'] = (boolean) $db->pegaUm( $sql );
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, 'Clique no nome do projeto para iniciar o trabalho.' );
?>
<script language="javascript" type="text/javascript" src="../includes/micoxAjax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript">

	function abrirDadosPA( id )
	{
		window.open(
		'?modulo=principal/dadospa&acao=C&atiid=' + id,
		'dadospa',
		'width=670,height=580,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}

	function abrirDadosAtividade( id )
	{
		window.open(
		'/pde/projetos.php?modulo=principal/atividade_/subatividades&acao=A&atiid=' + id,
		'dadospa',
		'width=670,height=580,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}

	function removerProjeto( id, nome ) {
		if ( confirm( 'Deseja excluir o projeto \'' + nome + '\'?' ) ) {
			document.formulario.atiid.value = id;
			document.formulario.remover.value = 'R';
			document.formulario.submit();
			//window.location.href = '?modulo=principal/projetos&acao=A&remover=' + id;
		}
	}

	function selecionarProjeto( id ){
		//document.formulario.selecionar_projeto.value = id;

		//redirecionar( 'principal/atividade_/arvore', 'A' );
		window.location.href = '?modulo=principal/projetos&acao=A&selecionar_projeto=' + id;
	}

	function selecionarContratacao( id ){
		window.location.href = '?modulo=principal/contratacoespa&acao=I&atiid=' + id;
	}

	function cadastrarProjeto(){
		var titulo = window.prompt( 'Título do projeto:', 'Novo Projeto' );
		if ( titulo ) {
			window.location.href = '?modulo=principal/projetos&acao=A&cadastrar_projeto=' + titulo;
		}
	}

	function enviar_email( cpf ){
		var nome_janela = 'janela_enviar_emai_' + cpf;
		window.open(
			'/geral/envia_email.php?cpf=' + cpf,
			nome_janela,
			'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}

	function listar_subclasses( clsid )
	{
		var campo_select = document.getElementById( 'sbcid' );
		var campo_clsid = parseInt(clsid, 2);

		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		if( clsid )
		{
			ajaxGet('../ajax/lista_subclasse.php?clsid='+clsid+'&sbcid='+p_sbcid,document.getElementById('span_sbcid'),true);
		}
	}

	function verificar_pronac(valor)
	{
		if ( valor == 'false' || valor == '' )	{

			document.getElementById("tppid").className = "CampoEstiloLeitura";
			document.getElementById("tscid").className = "CampoEstiloLeitura";
			document.formulario.atipronac.className = "leitura";

			document.getElementById('tppid').disabled  = true;
			document.getElementById('tscid').disabled  = true;
			document.formulario.atipronac.disabled  = true;
		} else {

			document.getElementById('tppid').disabled  = false;
			document.getElementById('tscid').disabled  = false;
			document.formulario.atipronac.disabled  = false;

			document.getElementById("tppid").className = "CampoEstilo";
			document.getElementById("tscid").className = "CampoEstilo";
			document.formulario.atipronac.className = "normal";
		}

	}


	function listar_partidos( tauid )
	{
		var ppoid = '<?=$ppoid; ?>';
		var campo_select = document.getElementById( 'ppoid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if ( tauid != '')
		{
			ajaxGet('../ajax/lista_partidos.php?tauid='+tauid+'&ppoid='+ppoid ,document.getElementById('span_ppoid'),true);
		}
	}

	function listar_autores( ppoid )
	{
		var aueid = '<?=$aueid ?>';
		var tauid = document.formulario.tauid.value;

		tauid = tauid ? tauid : '<?=$tauid ?>';

		var campo_select = document.getElementById( 'aueid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if (tauid != '' && ppoid != '' )
		{
			ajaxGet('../ajax/lista_autores.php?ppoid='+ppoid+'&tauid='+tauid+'&aueid='+aueid,document.getElementById('span_aueid'),true);
		}
	}


	function verificar_emenda(valor)
	{
		if ( valor == 'false' || valor == '' )	{
			document.getElementById("tauid").className = "CampoEstiloLeitura";
			document.getElementById("ppoid").className = "CampoEstiloLeitura";
			document.getElementById("aueid").className = "CampoEstiloLeitura";
			document.getElementById('tauid').disabled  = true;
			document.getElementById('ppoid').disabled  = true;
			document.getElementById('aueid').disabled  = true;
		} else {
			document.getElementById('tauid').disabled  = false;
			document.getElementById('ppoid').disabled  = false;
			document.getElementById('aueid').disabled  = false;
			document.getElementById("tauid").className = "CampoEstilo";
			document.getElementById("ppoid").className = "CampoEstilo";
			document.getElementById("aueid").className = "CampoEstilo";
		}

	}

	function listar_iniciativas( dfeid )
	{
		var campo_select = document.getElementById( 'iteid' );
		var campo_dfeid = parseInt(dfeid, 2);

		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		if( dfeid )
		{
			ajaxGet('../ajax/lista_iniciativas.php?dfeid='+dfeid+'&iteid='+p_iteid,document.getElementById('span_iteid'),true);
		}
	}

	function listar_acoes_estrategicas(preid)
	{
		var aesid = '<?=$aesid ?>';
		var campo_select = document.getElementById( 'aesid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if (preid != '' )
		{
			ajaxGet('../ajax/lista_acoes_estrategica.php?preid='+preid+'&aesid='+aesid,document.getElementById('span_aesid'),true);
		}
	}

</script>
<? extract($_POST); ?>
	<form action="#" method="POST" name="formulario">
	<input type="hidden" name="atiid" id="atiid" value="" />
	<input type="hidden" name="remover" id="remover" value="" />
	<input type="hidden" name="selecionar_projeto" id="selecionar_projeto" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">ID</td>
			<td><?= campo_texto('atiid','N','S',$permissao_formulario,20,10,'','','','','',''); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td><?= campo_texto('atidescricao','N','S',$permissao_formulario,80,77,'','','','','',''); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Plano Interno</td>
			<td><?= campo_texto('atinumeropi','N','S',$permissao_formulario,40,11,'','','','','',''); ?></td>
		</tr>
		<!--<tr>
			<td class="SubTituloDireita">Protocolo:</td>
			<td><input type="hidden" name="protocolo" id="protocolo" value="<?=$atiprotocolo;?>" /><?= campo_texto('atiprotocolo','N',$permissao_formulario,'',40,20,'','','','','','id="atiprotocolo"'); ?> <span id="resultprotocolo"></span></td>
		</tr>-->
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Número do Processo IPHAN:</td>
			<td><?= campo_texto('atiprotocolo','N',$permissao_formulario,'',25,20,'#####.######/####-##',''); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Cadastro: </td>
			<td>
				A partir de <?= campo_data( 'data_inicio', 'N', $permissao_formulario, '', 'N', '' ); ?>
				até <?= campo_data( 'data_fim', 'N', $permissao_formulario, '', 'N', '' ); ?>
			</td>
		</tr>

			<?php

				if($_REQUEST['esdid'][0] != ""){
					$sqlEstDoc = $_SESSION["indice_sessao_combo_popup"]['esdid']['sql'] . " AND esdid IN (".implode(",", $_REQUEST['esdid']).")";
					$esdid = $db->carregar($sqlEstDoc);
				}else{
					$esdid = null;
				}

				$sql = "SELECT esdid AS codigo, esddsc AS descricao, esdordem FROM workflow.estadodocumento WHERE esdstatus = 'A'";
//				$db->monta_combo("esdid",$sql,$permissao_formulario,'&nbsp','','','','','','esdid');
				mostrarComboPopup( 'Estado do documento:', 'esdid',  $sql, "",  'Selecione...');
				//echo  obrigatorio();
			?>

			<?php

				if($_REQUEST['uexid'][0] != ""){
					$sqlUndExc = $_SESSION["indice_sessao_combo_popup"]['uexid']['sql'] . " AND uexid IN (".implode(",", $_REQUEST['uexid']).")";
					$uexid = $db->carregar($sqlUndExc);
				}else{
					$uexid = null;
				}

				//$sql = "SELECT uexid AS codigo, uexsigla || ' - ' || uexdsc AS descricao FROM planointerno.unidadeexecutora WHERE uexstatus = 'A'";

				$sql = '';
				if ( usuario_possui_perfil(PERFIL_GESTOR_UNIDADE_DESCENTRALIZADA) || usuario_possui_perfil(PERFIL_TECNICO_UNIDADE_DESCENTRALIZADA) )
				{
					$sql = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE uexid in ( SELECT ur.uexid FROM sisplan.usuarioresponsabilidade ur WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.uexid is not null AND ur.rpustatus = 'A')";
				}

				if ( usuario_possui_perfil(PERFIL_APOIO_DIRETOR_DEPARTAMENTO) || usuario_possui_perfil(PERFIL_TECNICO_DEPARTAMENTO) || usuario_possui_perfil(PERFIL_DIRETOR_DEPARTAMENTO) )
				{
					$sql = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE uexid in ( SELECT ur.uexid FROM sisplan.usuarioresponsabilidade ur WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.uexid is not null AND ur.rpustatus = 'A') or uexid = ( SELECT uexid FROM seguranca.usuario where usucpf = '" . $_SESSION['usucpf']."')";
				}

				if ( $db->testa_superuser() ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
					$sql = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE TRUE ";
				 }


				mostrarComboPopup( 'Unidade Executora:', 'uexid',  $sql, "",  'Selecione...');
				//echo  obrigatorio();
			?>

		<tr>
			<td class="SubTituloDireita">Nome do Representante:</td>
			<td><?= campo_texto('atirepresentante','N','S',$permissao_formulario,40,70,'','','','','',''); ?></td>
		</tr>


		<tr>
			<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">PPA:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programa:</td>
			<td><?php
				$sql = sprintf( "SELECT prgcod as codigo, prgcod || ' - ' || prgdsc AS descricao FROM monitora.programa WHERE prgano = '%d' ORDER BY 2", $_SESSION['exercicio'] );
				$db->monta_combo("prgcod",$sql,$permissao_formulario,'&nbsp','listar_acoes','','','','','prgcod');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ação:</td>
			<td><?php
				$sql = array();
				if ( usuario_possui_perfil( PERFIL_COORDENADOR_ACAO_SIGPLAN ) )
				{
					$sql = "
					SELECT
						ur.acacod
					FROM sisplan.usuarioresponsabilidade ur
					WHERE ur.usucpf = '". $_SESSION['usucpf'] . "'
					  AND ur.acacod is not null
					  AND ur.rpustatus = 'A'";

					$acacodArr = (array) $db->carregarColuna($sql);
					if ($acacodArr[0]){
						$acacodWhere = " AND acacod IN ('" . ( implode("','", $acacodArr) ) . "')";
						$sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '%d' AND acacod IN ('" . ( implode("','", $acacodArr) ) . "') ORDER BY 2", $_SESSION['exercicio'] );
					}
				}

				if ( $db->testa_superuser() ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
					$sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '%d' ORDER BY 2", $_SESSION['exercicio'] );
				}

				$db->monta_combo("acacod",$sql,$permissao_formulario,'&nbsp','listar_unidades','','','600','','acacod');
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade Orçamentária:</td>
			<td><?php
				$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' order by 2", $_SESSION['exercicio'] );
				$db->monta_combo("unicod",$sql,$permissao_formulario,'&nbsp','listar_localizadores','','','','','unicod');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Localizador:</td>
			<td><?php
				$sql = "SELECT DISTINCT l.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod ORDER BY 2";
				$db->monta_combo("loccod",$sql,$permissao_formulario,'&nbsp','','','','','','loccod');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Produto:</td>
			<td><?php
				$sql = sprintf( "SELECT p.procod AS codigo, p.prodsc AS descricao FROM public.produto p WHERE p.procod IN ( SELECT a.procod FROM monitora.acao a JOIN pde.atividade at ON at.acaid = a.acaid ) ORDER BY 2", $_SESSION['exercicio'] );
				$db->monta_combo("procod",$sql,$permissao_formulario,'&nbsp','','','','','','procod');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Regionaliza&ccedil;&atilde;o:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Esfera Administrativa:</td>
			<td>
				<?php
					$sql = "SELECT esfid AS codigo, esfdsc AS descricao FROM planointerno.esfera";
					$db->monta_combo( "esfid",$sql,$permissao_formulario,"&nbsp;",'action_esfera','', '', '', 'N', "esfid" );
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Pa&iacute;s:</td>
			<td>
				<?php
					$sql = "SELECT paiid AS codigo, paidescricao AS descricao FROM territorios.pais ORDER BY 2";
					$db->monta_combo("paiid",$sql,$permissao_formulario,'&nbsp','','','','','','paiid');
					//echo  obrigatorio();
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Unidade Federativa:</td>
			<td>
				<div id="estuf_on" style="<?php if($esfid==1 || $esfid==''){ ?>display:none;<?php }else{ ?>display:block;<?php } ?>">
				<?php
					$sql = "SELECT estuf AS codigo, estuf||' - '||estdescricao AS descricao FROM territorios.estado ORDER BY 2";
					$db->monta_combo("estuf",$sql,$permissao_formulario,'&nbsp','listar_municipios','','','','','estuf');
					//echo  obrigatorio();
				?>
				</div>
				<div id="estuf_off" style="color:#909090;">&nbsp;</div>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Município:</td>
			<td>
				<div id="muncod_on" style="<?php if($esfid ==1 || $esfid ==2 || $esfid==''){ ?>display:none;<?php } else { echo "display:block;"; } ?>">
					<?
						if ( $estuf == 'DF' )
						{
							$sql = "SELECT rgaid, rgaid as codigo, rgadsc as descricao FROM planointerno.regiaoadministrativa ORDER BY 3 ";
						}
						else
						{
							$sql = "SELECT estuf, muncod as codigo, estuf || ' - ' || mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf."' ORDER BY 3 ";
						}
						$db->monta_combo("muncod",$sql,$permissao_formulario,"&nbsp;",'','','','','','muncod');
					?>
				</div>
				<div id="muncod_off" style="color:#909090;">&nbsp;</div>
			</td>
		</tr>



		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Prioridade:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Desafio Estratégico:</td>
			<td><?php
				$sql = "SELECT dfeid AS codigo, dfedsc AS descricao FROM sisplan.desafioestrategico WHERE dfestatus = 'A' ORDER BY 2";
				$db->monta_combo("dfeid",$sql,$permissao_formulario,'&nbsp','listar_iniciativas','','','','','dfeid');
				//echo  obrigatorio();
			?><span id="span_dfeid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Iniciativa estratégica:</td>
			<td>
			<?
				$sql = array();
				if ( $dfeid )
					$sql = sprintf( "SELECT iteid AS codigo, itedsc AS descricao FROM sisplan.iniciativaestrategica WHERE itestatus = 'A' and dfeid = '%d' ORDER BY 2", $_GET['acacod'] );
				else
					$sql = sprintf( "SELECT iteid AS codigo, itedsc AS descricao FROM sisplan.iniciativaestrategica WHERE itestatus = 'A' ORDER BY 2");

				$db->monta_combo("iteid",$sql,$permissao_formulario,'&nbsp','','','','','','iteid');
				//echo  obrigatorio();
			?><span id="span_iteid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programas estratégicos:</td>
			<td><?php
				$sql = sprintf( "SELECT preid AS codigo, predsc AS descricao FROM planointerno.programaestrategico WHERE prestatus = 'A' ORDER BY 2" );
				$db->monta_combo("preid",$sql,$permissao_formulario,'&nbsp','listar_acoes_estrategicas','','','','','preid');
				//echo  obrigatorio();
			?><span id="span_preid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ações estratégicas:</td>
			<td><?php
				if ( $preid )
					$sql = sprintf( "SELECT aesid AS codigo, aesdsc AS descricao FROM planointerno.acaoestrategica WHERE aesstatus = 'A' AND preid =%d ORDER BY 2", $preid );
				else
					$sql = "SELECT aesid AS codigo, aesdsc AS descricao FROM planointerno.acaoestrategica WHERE aesstatus = 'A' ORDER BY 2";
				$db->monta_combo("aesid",$sql,$permissao_formulario,'&nbsp','','','','','','aesid');
				//echo  obrigatorio();
			?><span id="span_aesid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Prioridade:</td>
			<td>
			<?php
				$sql = "SELECT pddid as codigo, pdddsc as descricao FROM sisplan.prioridade ORDER BY 1";
				$db->monta_combo("pddid",$sql,$permissao_formulario,'&nbsp','','','','','','pddid');
			?>
			</td>
		</tr>



		<tr>
			<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Caracterização do Projeto:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Classe:</td>
			<td><?php
				$sql = "SELECT clsid AS codigo, clsdsc AS descricao FROM sisplan.classe WHERE clsstatus = 'A' ORDER BY 2";
				$db->monta_combo("clsid",$sql,$permissao_formulario,'&nbsp','listar_subclasses','','','','','clsid');
				//echo  obrigatorio();
			?><span id="span_clsid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Sub-classe:</td>
		<td>
			<?php
				$sql = sprintf( "SELECT sbcid AS codigo, sbcdsc AS descricao FROM sisplan.subclasse WHERE sbcstatus = 'A' ORDER BY 2");

				$db->monta_combo("sbcid",$sql,$permissao_formulario,'&nbsp','','','','','','sbcid');
				//echo  obrigatorio();
			?><span id="span_sbcid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Área:</td>
			<td><?php
				$sql = "SELECT areid AS codigo, aredsc AS descricao FROM planointerno.area WHERE arestatus = 'A' ORDER BY 2";
				$db->monta_combo("areid",$sql,$permissao_formulario,'&nbsp','','','','','','areid');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Segmento:</td>
			<td><?php
				$sql = "SELECT segid AS codigo, segdsc AS descricao FROM planointerno.segmento WHERE segstatus = 'A' ORDER BY 2";
				$db->monta_combo("segid",$sql,$permissao_formulario,'&nbsp','','','','','','segid');
				//echo  obrigatorio();
			?></td>
		</tr>



		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">
			PRONAC<br>
			<?
			if ( $_REQUEST['atipronacquest'] == "true" )
				$atipronacquest = 't';
			elseif ( $_REQUEST['atipronacquest'] == "false" )
				$atipronacquest = 'f';
			else
				$atipronacquest = '';
			?>
			<input type="radio" id="atipronacquest_sim" <? if ($atipronacquest == 't') echo 'checked'; ?> name="atipronacquest" value="true" onclick="verificar_pronac(this.value)" />SIM
			<input type="radio" id="atipronacquest_nao" <? if ($atipronacquest == 'f') echo 'checked'; ?> name="atipronacquest" value="false" onclick="verificar_pronac(this.value)" />NÃO
			<input type="radio" id="atipronacquest_nao" <? if ($atipronacquest == '') echo 'checked'; ?> name="atipronacquest" value="" onclick="verificar_pronac(this.value)" />Indiferente
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nº PRONAC: </td>
			<td><?= campo_texto('atipronac','N','S',$permissao_formulario,40,20,'','','','','',''); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Tipo:</td>
			<td><?php
				$sql = "SELECT tppid AS codigo, tppdsc AS descricao FROM planointerno.tipoproponente WHERE tppstatus = 'A' ORDER BY 2";
				$db->monta_combo("tppid",$sql,$permissao_formulario,'&nbsp','','','','','','tppid');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Forma de Seleção:</td>
			<td><?php
				$sql = "SELECT tscid AS codigo, tscdsc AS descricao FROM planointerno.tiposetorcultural WHERE tscstatus = 'A' ORDER BY 2";
				$db->monta_combo("tscid",$sql,$permissao_formulario,'&nbsp','','','','','','tscid');
				//echo  obrigatorio();
			?></td>
		</tr>



		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">
			Emenda Parlamentar<br>
			<?
			if ( $_REQUEST['atiemenda'] == "true" )
				$atiemenda = 't';
			elseif ( $_REQUEST['atiemenda'] == "false" )
				$atiemenda = 'f';
			else
				$atiemenda = '';
			?>
			<input type="radio" id="atiemenda_sim" name="atiemenda" <? if ($atiemenda == 't') echo 'checked'; ?> value="true" onclick="verificar_emenda(this.value)" />SIM
			<input type="radio" id="atiemenda_nao" name="atiemenda" <? if ($atiemenda == 'f') echo 'checked'; ?> value="false" onclick="verificar_emenda(this.value)" />NÃO
			<input type="radio" id="atiemenda_nao" name="atiemenda" <? if ($atiemenda == '') echo 'checked'; ?> value="" onclick="verificar_emenda(this.value)" />Indiferente
			</td>
		</tr>

		<div id="div_tabela_emenda_sim">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Tipo de Autor:</td>
			<td><?php
				$sql = "SELECT tauid AS codigo, taudsc AS descricao FROM planointerno.tipoautor WHERE taustatus = 'A' ORDER BY 2";
				$db->monta_combo("tauid",$sql,$permissao_formulario,'&nbsp','listar_partidos','','','','','tauid');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Partido:</td>
			<td><?php
				if ( $tauid )
					$sql = sprintf( "SELECT DISTINCT p.ppoid AS codigo, p.pposigla || ' - ' || p.ppodsc AS descricao FROM planointerno.partidopolitico p JOIN planointerno.autoremenda a ON p.ppoid = a.ppoid WHERE p.ppostatus = 'A' AND a.tauid = %d ORDER BY 2", $tauid );
				else
					$sql = "SELECT ppoid AS codigo, pposigla || ' - ' || ppodsc AS descricao FROM planointerno.partidopolitico WHERE ppostatus = 'A' ORDER BY 2";
				$db->monta_combo("ppoid",$sql,$permissao_formulario,'&nbsp','listar_autores','','','','','ppoid');
				//echo  obrigatorio();
			?><span id="span_ppoid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor:</td>
			<td><?php
				if ( $tauid && $ppoid )
					$sql = sprintf( "SELECT aueid AS codigo, auedsc AS descricao FROM planointerno.autoremenda WHERE auestatus = 'A' AND tauid = %d AND ppoid = %d ORDER BY 2", $tauid, $ppoid );
				else
					$sql = "SELECT aueid AS codigo, auedsc AS descricao FROM planointerno.autoremenda WHERE auestatus = 'A' ORDER BY 2";
				$db->monta_combo("aueid",$sql,$permissao_formulario,'&nbsp','','','','','','aueid');
				//echo  obrigatorio();
			?><span id="span_aueid"></span></td>
		</tr>
		</div>

		<tr>
			<td bgcolor="#cccccc">&nbsp;</td>
			<td bgcolor="#cccccc">
				<input type="hidden" name="submitPesquisar" value="1">
				<input type="button" name="botao" value="Pesquisar" onclick="pesquisar();"/>
				<input type="button" name="botao" value="Limpar" onclick="limpar();"/>
			</td>
		</tr>
</table>
<?php
$output = "";
$outputCabecalho = "";

?>
</form>
<?php if ( $usuario_gerente_projeto ) : ?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
		<tr>
			<td style="padding: 10px;">
				<span
					style="cursor: pointer"
					onclick="cadastrarProjeto();"
					title="novo projeto"
				>
					<img
						align="absmiddle"
						src="/imagens/gif_inclui.gif"
					/>
					Cadastrar Projeto
				</span>
			</td>
		</tr>
	</table>
<?php endif; ?>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<?php
	if ( count( $projetos ) && $_REQUEST['submitPesquisar'] ) :

		$outputCabecalho = '
		<thead>
			<tr bgcolor="#dfdfdf">
				<td align="center" width="70" rowspan="2"><b>Ações</b></td>
				<td align="center" rowspan="2"><b>ID</b></td>
				<td align="center" rowspan="2"><b>Plano Interno</b></td>
				<td align="center" rowspan="2"><b>UF</b></td>
				<td align="center" rowspan="2"><b>Unidade Executora</b></td>
				<td align="center" rowspan="2"><b>Projeto</b></td>
				<td align="center" rowspan="2"><b>Data Cadastro</b></td>
				<td align="center" rowspan="2"><b>Estado do Documento</b></td>
				<td align="center" rowspan="2"><b>PPA</b></td>
				<td align="center" colspan="2"><b>Meta Física</b></td>
				<td align="center" colspan="3"><b>Financeiro (R$)</b></td>
			</tr>
			<tr bgcolor="#dfdfdf">
				<td align="center"><b>Produto/Unidade</b></td>
				<td align="center"><b>Quantidade</b></td>
				<td align="center"><b>Custeio</b></td>
				<td align="center"><b>Investimento</b></td>
				<!--<td align="center"><b>Capital</b></td>-->
				<td align="center"><b>Total</b></td>
			</tr>
		</thead>
		' ;
		$cor = '';
		foreach ( $projetos as $projeto ) :
			$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;
			$img = "";

			$output .= '
			<tr
				bgcolor="'.$cor.'"
				onmouseout="this.style.backgroundColor=\''.$cor.'\';"
				onmouseover="this.style.backgroundColor=\'#ffffcc\';"
			>';


			if ( atividade_verificar_responsabilidade( $projeto['atiid'], $_SESSION['usucpf'] ) || unidadeexecutora_verificar_responsabilidade($projeto['uexid'])  ) :


				if(   usuario_possui_perfil(PERFIL_SUPERUSER) || usuario_possui_perfil(PERFIL_COORDENADOR_PLANEJAMENTO)|| usuario_possui_perfil(PERFIL_COORDENADOR_ORCAMENTO)|| usuario_possui_perfil(PERFIL_APOIO_COORDENADOR_ORCAMENTO)|| usuario_possui_perfil(PERFIL_APOIO_COORDENADOR_PLANEJAMENTO) ){
					$img .= '<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="window.location.href = \'?modulo=principal/cadpa&acao=I&atiid='.$projeto['atiid'].'\'" title="alterar informações do projeto"/>';
				}else{
		  			if($projeto['esdid'] == WORKFLOW_EM_ELABORACAO){
						$img .= '<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="window.location.href = \'?modulo=principal/cadpa&acao=I&atiid='.$projeto['atiid'].'\'" title="alterar informações do projeto"/>';
		  			}else{
						$img .= '<img align="absmiddle" src="/imagens/alterar_01.gif"/>';
		  			}
				}

				if ( !$projeto['possui_atividade'] && $db->testa_superuser() ):
					$img .= '&nbsp;<img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer" onclick="removerProjeto( '.$projeto['atiid'] .', \''.$projeto['atidescricao'] .'\' );" title="excluir projeto" />';
				else:
					$img .= '&nbsp;<img align="absmiddle" src="/imagens/excluir_01.gif"/>';
				endif;
			 else:
			 	if( ( usuario_possui_perfil(PERFIL_DIRETOR_DEPARTAMENTO) || usuario_possui_perfil(PERFIL_APOIO_DIRETOR_DEPARTAMENTO) ) && ($projeto['esdid'] == WORKFLOW_ANALISE_DEPARTAMENTO) ){

			 	if(   usuario_possui_perfil(PERFIL_SUPERUSER) || usuario_possui_perfil(PERFIL_COORDENADOR_PLANEJAMENTO)|| usuario_possui_perfil(PERFIL_COORDENADOR_ORCAMENTO)|| usuario_possui_perfil(PERFIL_APOIO_COORDENADOR_ORCAMENTO)|| usuario_possui_perfil(PERFIL_APOIO_COORDENADOR_PLANEJAMENTO) ){
					$img .= '<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="window.location.href = \'?modulo=principal/cadpa&acao=I&atiid='.$projeto['atiid'].'\'" title="alterar informações do projeto"/>';
				}else{
		  			if($projeto['esdid'] == WORKFLOW_EM_ELABORACAO){
						$img .= '<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="window.location.href = \'?modulo=principal/cadpa&acao=I&atiid='.$projeto['atiid'].'\'" title="alterar informações do projeto"/>';
		  			}else{
						$img .= '<img align="absmiddle" src="/imagens/alterar_01.gif"/>';
		  			}
				}
//				$img .= '<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="window.location.href = \'?modulo=principal/cadpa&acao=I&atiid='.$projeto['atiid'].'\'" title="alterar informações do projeto"/>';

			 	}else{
					$img .= '<img align="absmiddle" src="/imagens/alterar_01.gif"/>';
			 	}
				$img .= '&nbsp;<img align="absmiddle" src="/imagens/excluir_01.gif"/>';
			endif;

			$output .= '
			<td align="center" nowrap="nowrap">
				'.$img.'&nbsp;&nbsp;&nbsp;<img align="absmiddle" src="/imagens/consultar.gif" style="cursor: pointer" onclick="abrirDadosPA('.$projeto['atiid'].')" title="Tramitação / Abrir Plano de Ação "/>
				&nbsp;&nbsp;
				<img align="absmiddle" src="/imagens/codigo.gif" style="cursor: pointer" onclick="selecionarContratacao('.$projeto['atiid'].')" title="Abrir Contratação"/>
				</td>
				<td>
					'.$projeto['atiid'].'
				</td>
			<td>';

			$output .= sprintf( '%05s', $projeto['atinumeropi'] );

			$output .= '
				</td>
				<td>
					'.$projeto['estado'].'
				</td>
				<td>
					'.$projeto['unidadeexecutora'].'
				</td>
				<td>
					<img src="/imagens/anexo.gif" title="Anexar Arquivo" style="cursor:pointer;" onclick="window.location=\'sisplan.php?modulo=principal/atividade_/anexo&acao=A&atiid='.$projeto['atiid'].'\';">&nbsp;&nbsp;&nbsp;
						'.$projeto['atidescricao'].'
					<br/>
				';
				if ( $projeto['usunome'] ) :
					$output .= '
						<a href="#" onclick="enviar_email( \''.$projeto['usucpf'].'\' ); return false;" style="text-decoration: none;" title="Enviar um email para '.$projeto['usunome'].'">
							<span style="font-size: 9px; font-weight: normal; color: #808080;">
								Usuário: '.$projeto['usunome'].'
							</span>
						</a>
					';
				endif;

				$output .= '
				</td>
				<td style="text-align:center">
					'.$projeto['atidatacadastro'].'
				</td>
				<td style="text-align:center">
					'.($projeto['esddsc'] == '' ? '-' : $projeto['esddsc']).'
				</td>
				<td style="text-align:center">
					'.$projeto['codppa'].'
				</td>
				<td>'.$projeto['prodsc'].'<br/>
					<i>'.$projeto['unmdsc'].'</i>
				</td>
				<td style="text-align:center">
					'.number_format( $projeto['atipropameta'], 0, ",", "." ).'</td>
				</td>
				<td style="text-align:right">
					'.number_format( $projeto['atiorcamentocusteio'], 2, ",", "." ).'
				</td>
				<td style="text-align:right">
					'.number_format( $projeto['atiorcamentocapital'], 2, ",", "." ).'
				</td>
				<td style="text-align:right">
					'.number_format( $projeto['atiorcamentototal'], 2, ",", "." ).'
				</td>
			</tr>
			';
		endforeach;

		$outputCabecalhoFora = '
		<thead>
			<tr bgcolor="#dfdfdf">
				<td align="center" width="70" rowspan="2"></td>
				<td align="center" width="70" rowspan="2">ID</td>
				<td align="center" width="70" rowspan="2"><b>Plano Interno</b></td>
				<td align="center" rowspan="2"><b>UF</b></td>
				<td align="center" rowspan="2"><b>Unidade Executora</b></td>
				<td align="center" rowspan="2"><b>Projeto</b></td>
				<td align="center" rowspan="2"><b>Data Cadastro</b></td>
				<td align="center" rowspan="2"><b>Estado do Documento</b></td>
				<td align="center" rowspan="2"><b>PPA</b></td>
				<td align="center" colspan="2"><b>Meta Física</b></td>
				<td align="center" colspan="3"><b>Financeiro (R$)</b></td>
			</tr>
			<tr bgcolor="#dfdfdf">
				<td align="center"><b>Produto/Unidade</b></td>
				<td align="center"><b>Quantidade</b></td>
				<td align="center"><b>Custeio</b></td>
				<td align="center"><b>Investimento</b></td>
				<!--<td align="center"><b>Capital</b></td>-->
				<td align="center"><b>Total</b></td>
			</tr>
		</thead>
		' ;

		$_SESSION['REL_PESQUISAPA'] = '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">'.$outputCabecalhoFora.$output;


		print ( $outputCabecalho.$output );
		?>
	<?php elseif ( !$_REQUEST['submitPesquisar'] ) : ?>
	<tr>
	<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;" colspan="2">
	Para listar os projetos cadastrados, escolha os argumentos de pesquisa desejados e clique em pesquisar.<br/>
	Se nada for escolhido todos serão listados.
	</td>
	</tr>
	<?php else : ?>
		<tr>
			<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;" colspan="2">
				Nenhum resultado encontrado.
			</td>
		</tr>
	<?php endif; ?>

	<?php if ( count( $projetos ) && $_REQUEST['submitPesquisar'] ) : ?>
	</table>
		<table id="tbExportar" class="tabela" cellspacing="1" cellpadding="3" border="0" align="center" bgcolor="#f5f5f5">
		<tr>
			<td align="center">Exportar o resultado em <a href="#" onclick="imprime_rel('xls');" >XLS</a></td>
		</tr>
	</table>
	<?php endif; ?>
<script type="text/javascript">

<?php
echo <<<EOS
var p_prgcod = '$prgcod';
var p_acacod = '$acacod';
var p_unicod = '$unicod';
var p_loccod = '$loccod';
var p_iteid = '$iteid';
var p_sbcid = '$sbcid';
var p_usucpfcoordenacao = '$usucpfcoordenacao';
EOS;
?>

	function imprime_rel(cod)
	{
	     //Abre popup em branco
	   	 janela = window.open('about:blank',"relatorio","menubar=no,location=no,open=yes,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");

	     if ( cod == 'pdf' ){
	     	//Abre relatório pdf no popup
	     	janela.location ="projetos.php?modulo=principal/relatorios/consolidadoPdf&acao=R";
	     }else if( cod == 'xls' ){
	     	//Abre relatório pdf no popup
	     	janela.location ="sisplan.php?modulo=principal/pesquisapaExcel&acao=A";
	     }
	}

	function listar_acoes( prgcod )
	{
		var campo_select = document.getElementById( 'acacod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if( prgcod != '' )
		{
			ajaxGet('../ajax/lista_acoes.php?prgcod='+prgcod+'&acacod='+p_acacod,document.getElementById('acacod'),true);
		}
	}

	function listar_unidades( acacod )
	{
		var campo_select = document.getElementById( 'unicod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_unidades.php?acacod='+acacod+'&unicod='+p_unicod,document.getElementById('unicod'),true);

		}
	}

	function listar_localizadores( unicod )
	{
		var prgcod = document.formulario.prgcod.value ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value ? document.formulario.acacod.value : p_acacod;;
		var unicod = document.formulario.unicod.value ? document.formulario.unicod.value : p_unicod;;

		var campo_select = document.getElementById( 'loccod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );


		if( unicod !='' && prgcod != '' && acacod != '')
		{
			ajaxGet('../ajax/lista_localizadores.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+p_loccod,document.getElementById('loccod'),true);
		}
	}

	function listar_partidos( tauid )
	{
		var ppoid = '<?=$ppoid; ?>';
		var campo_select = document.getElementById( 'ppoid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if ( tauid != '')
		{
			ajaxGet('../ajax/lista_partidos.php?tauid='+tauid+'&ppoid='+ppoid ,document.getElementById('ppoid'),true);
		}
	}

	function listar_autores( ppoid )
	{
		var aueid = '<?=$aueid ?>';
		var tauid = document.formulario.tauid.value;

		tauid = tauid ? tauid : '<?=$tauid ?>';

		var campo_select = document.getElementById( 'aueid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if (tauid != '' && ppoid != '' )
		{
			ajaxGet('../ajax/lista_autores.php?ppoid='+ppoid+'&tauid='+tauid+'&aueid='+aueid,document.getElementById('aueid'),true);
		}
	}


	function listar_acoes_estrategicas(preid){

		var aesid = '<?=$aesid ?>';
		var campo_select = document.getElementById( 'aesid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if (preid != '' )
		{
			ajaxGet('../ajax/lista_acoes_estrategica.php?preid='+preid+'&aesid='+aesid,document.getElementById('aesid'),true);
		}
	}




	function action_esfera(esfid)
	{

		esfid = esfid ? esfid : document.formulario.esfid.value;

		if(esfid=='')
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf_on').style.display='none';
			document.getElementById('estuf_off').style.display='block';
			document.getElementById('muncod_on').style.display='none';
			document.getElementById('muncod_off').style.display='block';

			document.getElementById('estuf').value='';
			document.getElementById('muncod').value='';
		}
		if(esfid==1)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
//			document.getElementById('estuf').value='';
//			document.getElementById('muncod').value='';
//
//			listar_municipios( document.getElementById('estuf').value );
//
//			document.getElementById('estuf_on').style.display='block';
//			document.getElementById('estuf_off').style.display='none';
//			document.getElementById('muncod_on').style.display='block';
//			document.getElementById('muncod_off').style.display='none';
		}
		else if(esfid==2)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf').value='<?=$estuf ?>';
			document.getElementById('muncod').value='';

			listar_municipios( document.getElementById('estuf').value );

			document.getElementById('estuf_on').style.display='block';
			document.getElementById('estuf_off').style.display='none';
			document.getElementById('muncod_on').style.display='block';
			document.getElementById('muncod_off').style.display='none';
		}
		else if(esfid==3)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf').value  = '<?=$estuf ?>';
			document.getElementById('muncod').value = '';

			<?
			if (!$muncod){
			?>
			listar_municipios( document.getElementById('estuf').value );
			<?
			}else{
			?>
			document.getElementById('muncod').value = '<?=$muncod ?>';
			<?
			}
			?>
			document.getElementById('estuf_on').style.display='block';
			document.getElementById('estuf_off').style.display='none';
		}
		else if(esfid==4)
		{
			document.formulario.paiid.disabled = false;
			document.getElementById('estuf_on').style.display='none';
			document.getElementById('estuf_off').style.display='block';
			document.getElementById('muncod_on').style.display='none';
			document.getElementById('muncod_off').style.display='block';

			document.getElementById('estuf').value='';
			document.getElementById('muncod').value='';
		}

	}


	function listar_municipios( regcod )
	{
		var campo_select = document.getElementById( 'muncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if(document.formulario.esfid.value!='')
		{
			document.getElementById('muncod_on').style.display='block';
			document.getElementById('muncod_off').style.display='none';
			ajaxGet('../ajax/lista_mun_pi.php?estuf='+regcod,document.getElementById('muncod'),true);
		}
	}
	var p_prgcod = '';
	var p_acacod = '';
	var p_unicod = '';
	var p_loccod = '';
	function pesquisar(){
		if ( document.getElementById('esdid') )
			selectAllOptions( document.getElementById( 'esdid' ) );
		if ( document.getElementById('uexid') )
			selectAllOptions( document.getElementById( 'uexid' ) );

		document.formulario.submit();
	}

	function limpar()
	{
		window.location = "?modulo=principal/pesquisapa&acao=A";
	}

	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}



<?
if ($esfid){
	echo "action_esfera($esfid);";
}

if ( $dfeid )
	echo "listar_iniciativas($dfeid);";

//PRONAC
if ( $_REQUEST['atipronacquest'] == "false" )
	echo "verificar_pronac('".$_REQUEST['atipronacquest']."');";
elseif ( $_REQUEST['atipronacquest'] == "true" )
	echo "verificar_pronac('".$_REQUEST['atipronacquest']."');";
else
	echo "verificar_pronac('');";

//EMENDA
if ( $_REQUEST['atiemenda'] == "false" )
	echo "verificar_emenda('".$_REQUEST['atiemenda']."');";
elseif ( $_REQUEST['atiemenda'] == "true" )
	echo "verificar_emenda('".$_REQUEST['atiemenda']."');";
else
	echo "verificar_emenda('');";
?>
</script>
