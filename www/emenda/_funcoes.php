<?php
function disabledPTA(){
	global $db;

	$sql = "SELECT
				pu.pflcod
			FROM 
				seguranca.perfil AS p 
			LEFT JOIN 
				seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE 
				p.sisid = '".$_SESSION['sisid']."'
			  	AND pu.usucpf = '".$_SESSION['usucpf']."'";

	$pflcod = $db->carregarColuna($sql);
	$retorno = false;

	if($pflcod && $_SESSION['exercicio'] != '2009') {
		if($pflcod) {
			foreach($pflcod as $perfil){
				if($perfil == SUPER_USUARIO || $perfil == ADMINISTRADOR_INST || $perfil == INSTITUICAO_BENEFICIADA) {
					$retorno = true;
					break;
				}
			}
		}
	}

	if( !$retorno ){
		return 'disabled="disabled"';
	} else {
		return '';
	}
}

function disabled($categoria = 'geral') {
	global $db;
	$retorno = '';
	if( empty($categoria) ) $categoria = 'geral';
	if(!possuiPermissao($categoria)) {
		$retorno = 'disabled="disabled"';
	}
	return $retorno;
}

function testa_superAdm(){ // testa se  Super Administrador
	global $db;
	
	if ( !$_SESSION['usucpf'] || !$_SESSION['sisid'] ) {
		return false;
	}
	
	$sql= "SELECT pu.usucpf FROM seguranca.perfilusuario pu 
				inner join seguranca.perfil p on p.pflcod = pu.pflcod 
					and p.pflsuperuser='t' 
					and pu.usucpf ='".$_SESSION['usucpf']."' 
					and p.pflcod ='".SUPER_ADMINISTRADOR."' 
					and p.sisid=".$_SESSION['sisid'];
	$registro = $db->recuperar($sql);
	if (is_array($registro)){
		return true;
	}else {
		if( $db->testa_superuser() )return true;
		else return false;
	}
}

function verificaPermissaoPerfil($tipo = '', $tipoCampo = 'button', $categoria = 'geral', $boPermissao = false){
	global $db;
	if( empty($categoria) ) $categoria = 'geral';	
	$param = $tipo == 'analise' ? 'ptridAnalise' : 'ptrid';	
	$estadoAtual = pegarEstadoAtual( $_SESSION['emenda'][$param] );
	
	$retorno = '';
	if($estadoAtual == CONVENIO_CANCELADO && !$db->testa_superuser() ){
		$retorno = 'disabled="disabled"';
	} else {
		if( $boPermissao ) $retorno = disabled( $categoria );
	}
	
	if( $tipoCampo == 'boolean' ){
		if( !empty($retorno) ) return false;
		else return true;
	} else {
		return $retorno;
	}
}
function possuiPermissao($categoria = 'geral') {

	include_once APPRAIZ . "www/emenda/permissoes_perfil.php";

	global $db;
	if( empty($categoria) ) $categoria = 'geral';
	// deve-se depois modificar o método 'pegaPerfil'
	$sql = "SELECT
				pu.pflcod
			FROM 
				seguranca.perfil AS p 
			LEFT JOIN 
				seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE 
				p.sisid = '".$_SESSION['sisid']."'
			  	AND pu.usucpf = '".$_SESSION['usucpf']."'";
	$pflcod = $db->carregarColuna($sql);
	$retorno = false;

	if($pflcod) {
		foreach($pflcod as $perfil){
			if($perfil == SUPER_USUARIO) {
				$retorno = true;
				break;
			} else {
				if(!$retorno){
					$retorno = permissoesPerfil($perfil, $_REQUEST['modulo'], $categoria);
				}
			}
		}
	}
	
	$retorno = ($retorno == NULL) ? false : $retorno;
	return $retorno;
}

function exibeFiltroPesquisaInstituicao($retorno=''){
	?>
<form action="" method="POST" id="formulario" name="formulario"><input
	type="hidden" value="<?=$retorno; ?>" name="retorno" id="retorno">
<table width="95%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem">
	<tr>
		<td class="SubTituloDireita" style="width: 25%"><b>CNPJ:</b></td>
		<td><?
		//$enbcnpj = str_replace( array(".","/","-"), "", $_REQUEST['enbcnpj'] );
		$enbcnpj = $_REQUEST['enbcnpj'];
		echo campo_texto('enbcnpj', 'N', 'S', '', 27, 20, '', '','','','','id="enbcnpj"', "this.value=mascaraglobal('##.###.###/####-##',this.value);", $enbcnpj);
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Nome do Órgão ou Entidade:</b></td>
		<td><?
		$entnome = $_REQUEST['enbnome'];
		echo campo_texto('enbnome', 'N', 'S', '', 50, 50, '', '', '', '', 0, '', '', $entnome );
		?></td>
	</tr>
	<tr>
		<th style="text-align: center;" colspan="2"><input type="button"
			id="btPesquisar" value="Pesquisar" onclick="submetePesquisa();" /> <input
			type="button" id="btLimpar" value="Limpar Campos"
			onclick="limparPesquisa();" /></th>
	</tr>
</table>
</form>
		<?
}

function exibeInstituicaoBenefiada() {
	global $db;

	if(pegaPerfil($_SESSION['usucpf']) == INSTITUICAO_BENEFICIADA) {
		if($_SESSION["emenda"]["enbid"]) {
			$intituicao = $db->pegaUm("SELECT enbnome FROM emenda.entidadebeneficiada WHERE enbid = ".$_SESSION["emenda"]["enbid"]);
				
			$intituicao = ($intituicao) ? $intituicao : 'Nenhuma Selecionada';
		} else {
			$intituicao = 'Nenhuma Selecionada';
		}
		echo '<table cellspacing="0" cellpadding="0" border="0" width="100%" style="border-top:solid 1px #8F8F8F;">
				<tbody>
					<tr>
						<td height="17" bgcolor="#e0e0e0" align="left" valign="middle">
							<font color="#696969" style="margin-right: 10px;">
							&nbsp;&nbsp;<b>Intituição Beneficiada:</b>
							'.$intituicao.'
							</font>
						</td>
					</tr>
					<tr><td></td></tr>
				</tbody>
			</table>';
	}
}


function verificaPermissao() {
	if(pegaPerfil($usucpf) == INSTITUICAO_BENEFICIADA) {
		$enbid = verificaEntidadePta();

		if($enbid) {
			if(!in_array($_SESSION["emenda"]["enbid"], $enbid)) {
				echo "<script>
					alert('Você não tem permissão para acessar esta tela.');
					history.back(-1);
				  </script>";
				die;
			}
		} else {
			echo "<script>
					alert('Você não tem permissão para acessar esta tela.');
					history.back(-1);
				  </script>";
			die;
		}
	}
}

function verificaEntidadePta(){

	global $db;

	if ( !$db->testa_superuser() ){
		return true;
	}else{

		$sql = "SELECT
					ur.enbid
				FROM
					emenda.usuarioresponsabilidade ur
				INNER JOIN
					emenda.planotrabalho ep ON ep.enbid = ur.enbid
				WHERE
					ur.rpustatus = 'A' AND
					usucpf = '{$_SESSION["usucpf"]}' AND 
					pflcod = " . INSTITUICAO_BENEFICIADA;

		$enbid = $db->carregarColuna( $sql );
		return $enbid;

	}


}

/*
 * Função que monta o combo de responsáveis de acordo com o perfil do usuário.
 */
function comboResponsavel($usucpf, $selecionado='', $funcao='', $habil='', $obrigatorio = null) {
	global $db;
	$select = '';

	$perfil  = pegaPerfilArray($_SESSION['usucpf']);

	if( !in_array( LIBERAR_SENHA, $perfil ) && in_array( ADMINISTRADOR_INST, $perfil ) && !in_array( ANALISTA_FNDE, $perfil ) ){
		$responsaveis = recuperaResponsaveis($usucpf, $perfil);
	}
	if($responsaveis) {
		if(count($responsaveis) == 1) {
			$sql = "SELECT resid as codigo,resdsc as descricao FROM emenda.responsavel WHERE resstatus = 'A' AND resid = ".$responsaveis[0];
			$dados = $db->carregar($sql);
				
			$select .= '<select id="resid_disabled" class="CampoEstilo" style="width:265px;" name="resid_disabled" disabled="disabled">';
			$select .= '<option value="'.$dados[0]["codigo"].'">'.$dados[0]["descricao"].'</option>';
			$select .= '</select>';
			$select .= '<input type="hidden" name="resid" id="resid" value="'.$dados[0]["codigo"].'" />';
		} else {
			$resids = implode(",", $responsaveis);
				
			$sql = "SELECT resid as codigo,resdsc as descricao FROM emenda.responsavel WHERE resstatus = 'A' AND resid in (".$resids.") ORDER BY resdsc";
			$dados = $db->carregar($sql);
				
			$select .= '<select id="resid" '.$habil.' class="CampoEstilo" style="width:150px;" name="resid" onchange="'.$funcao.';">';
			$select	.= '<option value="">Selecione...</option>';
				
			for($i=0; $i<count($dados); $i++) {
				$selected = ($dados[$i]["codigo"] == $selecionado) ? 'selected="selected"' : '';

				$select .= '<option value="'.$dados[$i]["codigo"].'" '.$selected.'>'.$dados[$i]["descricao"].'</option>';
			}
				
			$select .= '</select>';
		}
	} else {
		$sql = "SELECT resid as codigo,resdsc as descricao FROM emenda.responsavel WHERE resstatus = 'A' ORDER BY resdsc";
		$dados = $db->carregar($sql);

		$select .= '<select id="resid" '.$habil.' class="CampoEstilo" style="width:150px;" title="Responsável" name="resid" onchange="'.$funcao.';">';
		$select	.= '<option value="">Selecione...</option>';

		for($i=0; $i<count($dados); $i++) {
			$selected = ($dados[$i]["codigo"] == $selecionado) ? 'selected="selected"' : '';
				
			$select .= '<option value="'.$dados[$i]["codigo"].'" '.$selected.'>'.$dados[$i]["descricao"].'</option>';
		}

		$select .= '</select>';
		if( $obrigatorio ){
			$select .= ' <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>';
		}
	}

	return $select;
}

/*
 * Função que retorna um array com os responsáveis associados ao usuário.
 */
function recuperaResponsaveis($usucpf, $perfil = array() ) {
	global $db;

	if( empty( $perfil ) ){
		$perfil  = pegaPerfilArray($_SESSION['usucpf']);
	}
	if(  in_array( ADMINISTRADOR_INST, $perfil ) ){
		$sql = "SELECT resid FROM emenda.usuarioresponsabilidade WHERE usucpf = '".$usucpf."' AND rpustatus = 'A' AND resid is not null";
		$responsaveis = $db->carregarColuna($sql);
	} else {
		$responsaveis = array();
	}

	return $responsaveis;
}

/*
 * Manter Programas
 */

function pesquisaProgramaAjax($post){
	global $db;

	$filtro = array();

	if($post['tpeid']){
		array_push($filtro, " te.tpeid = " . $post['tpeid']);
	}
	if($post['pronome']){
		array_push($filtro, " lower(p.pronome) = lower('" . $post['pronome']."')");
	}

	if( ($post['provigenciainicial']) && (!$post['provigenciafinal']) ){
		array_push($filtro, " p.provigenciainicial > '" . formata_data_sql($post['provigenciainicial'])."'" );

	}elseif( ($post['provigenciafinal']) && (!$post['provigenciainicial']) ){
		array_push($filtro, " p.provigenciafinal < '" . formata_data_sql($post['provigenciafinal'])."'" );

	}elseif( ($post['provigenciafinal']) && ($post['provigenciainicial']) ){
		array_push($filtro, " p.provigenciainicial >= '".formata_data_sql($post['provigenciainicial'])."' AND p.provigenciafinal <= '" . formata_data_sql($post['provigenciafinal'])."'" );
	}

	$sql = "SELECT
			  ( '<center><a href=\"emenda.php?modulo=principal/cadastroPrograma&acao=C&proid='|| p.proid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
			    '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiPrograma('|| p.proid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
			  p.pronome,
			  te.tpedesc,
			  p.procontrapartidaminima || '%' as procontrapartidaminima,
			  to_char(p.provigenciainicial, 'DD/MM/YYYY') || ' a ' || to_char(p.provigenciafinal, 'DD/MM/YYYY') as vigencia
			FROM 
			  emenda.programa p INNER JOIN public.tipoensino te
			    ON (p.tpeid = te.tpeid)
			WHERE p.prostatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ).
	        " ORDER BY p.pronome";

	monta_titulo( '', 'Lista de Programas Cadastrados' );
	$cabecalho = array("Comando", "Nome", "Nível de Ensino", "Contra-parte mínima", "Vigência");
	$tamanho = array( '5%', '45%', '20%', '10%', '20%' );
	$alinhamento = array( '', '', '', '', 'center' );

	$db->monta_lista(iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 20, 4, 'N','Center','','form', $tamanho, $alinhamento);
}

function carregaAcaoProgramaBanco($proid){
	global $db;
	if(!$_SESSION['emenda']['InsereAcao']){
		$_SESSION['emenda']['InsereAcao'] = array();
	}

	if($proid){

		$sql = "SELECT
				  ('<center><img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiPrograma('|| a.acaid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
				  pa.pacid,
				  p.proid,
				  a.acanome,
				  a.acaid,
				  pa.pacstatus
				FROM 
				  emenda.acao a INNER JOIN emenda.programaacao pa
				  ON (a.acaid = pa.acaid and pa.pacstatus = 'A') INNER JOIN emenda.programa p
				  ON (pa.proid = p.proid and p.prostatus = 'A')
				WHERE a.acastatus = 'A'
				  AND p.proid = $proid
				ORDER BY a.acanome";

		$dados = $db->carregar(iconv( "UTF-8", "ISO-8859-1", $sql));

		if($dados){
			$_SESSION['emenda']['InsereAcao'] = $dados;
			$_SESSION['emenda']['InsereAcaoPrograma'] = $_SESSION['emenda']['InsereAcao'];
			unset($_SESSION['emenda']['InsereAcao']);
			return "true";
		}else{
			return "false";
		}
	}else{
		return "false";
	}
}

function popInsereAcaoPrograma($post){

	if(empty($_SESSION['emenda']['InsereAcaoPrograma']) ){
		$_SESSION['emenda']['InsereAcaoPrograma'] = array();
	}

	if($post['check'] == "true"){
		$valor = explode("_", iconv( "UTF-8", "ISO-8859-1", $post['valor']) );

		$registro = Array("acao" => "<center><img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiPrograma('$valor[0]');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>",
						  "pacid" => $valor[2],
						  "proid" => $post['proid'],
						  "acanome" => $valor[1],
				          "acaid" => $valor[0],
				          "pacstatus" => $valor[3]
		);

		$arPacid = array();
		foreach( $_SESSION['emenda']['InsereAcaoPrograma'] as $arDadoPac ){
			$arPacid[] = $arDadoPac["acanome"];
		}

		if( !in_array($valor[1], $arPacid ) ) {
			array_push($_SESSION['emenda']['InsereAcaoPrograma'], $registro);
		}

	}
	if($post['check'] == "false"){
		$valor = explode("_", $post['valor']);
		foreach ($_SESSION['emenda']['InsereAcaoPrograma'] as $key => $value) {

			if($value['acaid'] == $valor[0]){
				unset($_SESSION['emenda']['InsereAcaoPrograma'][$key]);
			}
		}
	}
}

function excluiAcoesCadastradas($acaid){
	global $db;
	$msg = "";
	$arDados = array();

	if($acaid != ""){

		$arDados = $_SESSION['emenda']['InsereAcaoPrograma'];

		foreach ($arDados as $key => $valor) {
				
			if($acaid == $key){
				unset($_SESSION['emenda']['InsereAcaoPrograma'][$acaid]);
			}
		}
		$msg = "Operação realizada com sucesso!";
	}
}

function listaAcaoPrograma($proid){
	global $db;

	if($_SESSION['emenda']['InsereAcaoPrograma']){
		$dados = $_SESSION['emenda']['InsereAcaoPrograma'];
	}
	?>
<table width="50%" border="0" cellspacing="0" cellpadding="2"
	class="listagem">
	<thead>
		<tr>
			<td align="Center" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Excluir</strong></td>
			<td align="Center" valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ações
			Permitidas</strong></td>
		</tr>
	</thead>
	<?if($dados){
		foreach ($dados as $key => $value) { ?>
	<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';"
		onmouseout="this.bgColor='';">
		<td title="Excluir">
		<center><img src="/imagens/excluir.gif " style="cursor: pointer"
			onclick="excluiPrograma('<?=$key; ?>');" border=0 alt="Ir"
			title="Excluir"></center>
			<?//=$value['acao'] ?></td>
		<td title="Ação Permitida"><?=$value['acanome'] ?></td>
	</tr>
	<?}
}else{?>
	<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';"
		onmouseout="this.bgColor='';">
		<td colspan="2"><b>Nenhuma ação cadastrada!</b></td>
	</tr>
	<?}?>
</table>
	<?
}

function excluirProgramaAjax($proid){
	global $db;

	$sql = "UPDATE
			  emenda.programa  
			SET 
			  prostatus = 'I'
			 
			WHERE 
			  proid = $proid";

	$db->executar($sql);
	$sql = "UPDATE
			  emenda.programaemenda  
			SET 
			  prestatus = 'I'
			WHERE 
			  proid = $proid";
	$db->executar($sql);
	echo $db->commit();
}

/**
 * Função que retorna o array para montar as abas do Plano de Trabalho
 *
 * @return array
 *
 */
function carregaAbasPlanoTrabalho($pagina) {
	global $db;

	switch($pagina) {

		case 'acoesPlanoTrabalho':
				$menu = array(
							0 => array("id" => 1, "descricao" => "Lista de Ações", "link" => "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A")
							);
			break;
				
		case 'insereAcaoPlanoTrabalho':
			if(!empty($_SESSION["emenda"]["ptiid"])){
				if( $_SESSION['emenda']['federal'] == 'S' ){
					$menu = array(
					0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A"),
					1 => array("id" => 2, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&evento=A&ptiid=".$_SESSION["emenda"]["ptiid"].""),
					2 => array("id" => 3, "descricao" => "Especificações", "link" => "/emenda/emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A"),
					3 => array("id" => 4, "descricao" => "Rendimento Aplicação", "link" => "/emenda/emenda.php?modulo=principal/especificacaoRendimento&acao=A"),
					);
				} else {
					$menu = array(
					0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A"),
					1 => array("id" => 2, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&evento=A&ptiid=".$_SESSION["emenda"]["ptiid"].""),
					2 => array("id" => 3, "descricao" => "Especificações", "link" => "/emenda/emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A"),
					3 => array("id" => 4, "descricao" => "Rendimento Aplicação", "link" => "/emenda/emenda.php?modulo=principal/especificacaoRendimento&acao=A"),
					4 => array("id" => 5, "descricao" => "Beneficiarios", "link" => "/emenda/emenda.php?modulo=principal/insereBeneficiario&acao=A&acao=A"),
					);
				}
			}
			else {
				$menu = array(
				0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A"),
				1 => array("id" => 2, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A"),
				);
			}
			break;
				
		case 'insereBenecifiario':
			$menu = array(
			0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A"),
			1 => array("id" => 2, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&evento=A&ptiid=".$_SESSION["emenda"]["ptiid"].""),
			2 => array("id" => 3, "descricao" => "Especificações", "link" => "/emenda/emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A"),
			3 => array("id" => 4, "descricao" => "Rendimento Aplicação", "link" => "/emenda/emenda.php?modulo=principal/especificacaoRendimento&acao=A"),
			4 => array("id" => 5, "descricao" => "Beneficiarios", "link" => "/emenda/emenda.php?modulo=principal/insereBeneficiario&acao=A"),

			);
			break;
		case 'especificacaoRendimento':
			$menu = array(
			0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A"),
			1 => array("id" => 2, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&evento=A&ptiid=".$_SESSION["emenda"]["ptiid"].""),
			2 => array("id" => 3, "descricao" => "Especificações", "link" => "/emenda/emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A"),
			3 => array("id" => 4, "descricao" => "Rendimento Aplicação", "link" => "/emenda/emenda.php?modulo=principal/especificacaoRendimento&acao=A"),
			4 => array("id" => 5, "descricao" => "Beneficiarios", "link" => "/emenda/emenda.php?modulo=principal/insereBeneficiario&acao=A"),

			);
			break;

		case 'insereEspecificacaoAcao':
			if( $_SESSION['emenda']['federal'] == 'S' ){
				$menu = array(
				0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A"),
				1 => array("id" => 2, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&evento=A&ptiid=".$_SESSION["emenda"]["ptiid"].""),
				2 => array("id" => 3, "descricao" => "Especificações", "link" => "/emenda/emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A"),
				3 => array("id" => 4, "descricao" => "Rendimento Aplicação", "link" => "/emenda/emenda.php?modulo=principal/especificacaoRendimento&acao=A"),
				);
			} else {
				$menu = array(
				0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A"),
				1 => array("id" => 2, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&evento=A&ptiid=".$_SESSION["emenda"]["ptiid"].""),
				2 => array("id" => 3, "descricao" => "Especificações", "link" => "/emenda/emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A"),
				3 => array("id" => 4, "descricao" => "Rendimento Aplicação", "link" => "/emenda/emenda.php?modulo=principal/especificacaoRendimento&acao=A"),
				4 => array("id" => 5, "descricao" => "Beneficiarios", "link" => "/emenda/emenda.php?modulo=principal/insereBeneficiario&acao=A"),
				);
			}
			break;
	}

	$menu = $menu ? $menu : array();
	 
	return $menu;
}

/*
 * Manter Beneficiario
 */

function pesquisaBeneficiarioAjax($post){
	global $db;
	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
	
	if( is_array($post) ){
		$benome = $post['bennome'] ? " AND lower(bennome) LIKE lower('%".trim($post['bennome'])."%')" : "";
	}

	if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
		$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"novoBeneficiario('|| benid || ',\'' || bennome ||'\')\" \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
				   '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluiPrograma('|| benid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
	} else {
		$acao = "( '<center><img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\"  border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
				   '<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
	}

	$sql = "SELECT
			  ".$acao." as acao,
			  bennome
			FROM 
			  emenda.beneficiario
			WHERE benstatus = 'A' ". $benome . "
			ORDER BY bennome";

	monta_titulo( '', 'Lista de Beneficiários Cadastrados' );
	$cabecalho = array("Opção", "Beneficiário");

	$db->monta_lista(iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 20, 4, 'N','Center','','form');
}

/**
 * Função Pesquisar Tema
 * Método usado para pesquisa de um registro do banco
 * @param array $post - Campos do Formulário
 * @return void
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 03/09/2009
 */
function pesquisaTemaAjax($temnome){
	global $db;
	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

	$temome = ($temnome ? " AND lower(temnome) ILIKE lower('%".trim($temnome)."%')" : "");

	if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
		$acao = "( '<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"novoTema('|| temid ||',\''|| temnome ||'\');\" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
	    		   '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiTema('|| temid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
	} else {
		$acao = "( '<center><img src=\"/imagens/alterar_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Alterar\"> ' ||
	    		   '<img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
	}

	$sql = "SELECT
			  ".$acao." as acao,
			  temnome
			FROM 
			  emenda.tema
			WHERE
			  temstatus = 'A' ".$temome."
			ORDER BY temnome";

	monta_titulo( '', 'Lista de Temas Cadastrados' );
	$cabecalho = array("Opção", "Tema");

	$db->monta_lista( iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 50, 10, 'N', 'center', '', '', '','');
}

/*
 * Função que monta o cabeçalho do Plano de Trabalho
 */
function cabecalhoPlanoTrabalho($ptrid, $boAnalise = false) {
	global $db;

	if($ptrid) {
		$existePta = $db->pegaUm("SELECT count(*) FROM emenda.planotrabalho WHERE ptrid = ".$ptrid);

		if($existePta) {
			$sql = "SELECT
					  enb.enbcnpj AS cnpj,
					  enb.enbid,
	                  (CASE WHEN vede.emedescentraliza = 'N' THEN
	                  '<img src=\"/imagens/consultar.gif\" border=0 align=\"absmiddle\" vspace=\"3\" title=\"Atualizar dados da entidade\" style=\"cursor:pointer;\" onclick=\"atualizaEntidadeBaseFNDE('|| ptr.enbid || ', '|| ptr.ptrid || ');\"/> &nbsp; <a style=\"cursor:pointer;\" onclick=\"atualizaEntidadeBaseFNDE('|| ptr.enbid || ', '|| ptr.ptrid || ');\">' || enb.enbnome || ' (' || enb.enbid || ') </a>'
	                  ELSE
					  '<img src=\"/imagens/consultar.gif\" border=0 align=\"absmiddle\" vspace=\"3\" title=\"Editar Entidade\" style=\"cursor:pointer;\" onclick=\"Entidade('|| enb.enbid || ');\"/> &nbsp; <a style=\"cursor:pointer;\" onclick=\"Entidade('|| enb.enbid || ');\">' || enb.enbnome || ' (' || enb.enbid || ') </a>' END) AS orgao_entidade,                  
					  ptr.ptrexercicio AS exercicio,				
					  CASE WHEN enb.muncod IS NULL THEN
						CASE WHEN vede.emedescentraliza = 'N' THEN
							'<span style=\"color: red\">Clique no link acima para atualizar os dados da entidade, caso os dados continuem desatualizados, favor entrar em contato com a Coordenação de Habilitação para Projetos Educacionais (COHAP). Telefones: (61) 2022-4291 / 2022-4296.</span>'
						ELSE 
							'<span style=\"color: red\">Dados desatualizados. Clique no link acima para atualizar o endereço da entidade.</span>' 
						END
					ELSE
						(select mun.mundescricao || ' / ' || mun.estuf 
						from territorios.municipio mun 
						WHERE mun.muncod = enb.muncod)
					END	as endereco,
					  ptr.ptrcod as ptrid, 
					  md.mdedescricao as resassunto,
					  (sum(ped.pedvalor) + ptr.ptrvalorproponente) as ptrvalorconcedente,
					  pti.ptivalortotal as valor,
					  ptr.ptrpropostasiconv,
					  ptr.ptrpropostasiconvano,
					  sum(vede.edevalorreducao) as edevalorreducao
					FROM
					  emenda.entidadebeneficiada enb 
					  INNER JOIN emenda.planotrabalho ptr ON (enb.enbid = ptr.enbid) 
					  --INNER JOIN emenda.responsavel res ON (res.resid=ptr.resid)
					  inner join emenda.modalidadeensino md on md.resid = ptr.resid and md.mdeid = ptr.mdeid
					  INNER JOIN emenda.ptemendadetalheentidade ped on ped.ptrid = ptr.ptrid
					  INNER JOIN emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid AND vede.edestatus = 'A'
		  			  LEFT JOIN 
						(SELECT 
							ptrid,
							sum(ptivalortotal) as ptivalortotal
						 FROM emenda.v_ptiniciativa 
						 GROUP BY ptrid) pti ON pti.ptrid = ptr.ptrid
					WHERE
					  ptr.ptrid = $ptrid
					  AND ptr.ptrexercicio = ".$_SESSION["exercicio"]."
					Group by enb.enbcnpj,
					  enb.muncod,
					  orgao_entidade,
					  ptr.ptrexercicio,
					  enb.enbid,
					  ptr.ptrid, 
					  ptr.ptrcod,
					  md.mdedescricao,
					  ptr.ptrvalorproponente,
					  pti.ptivalortotal,
					  vede.emedescentraliza,
					  ptr.ptrpropostasiconv,
					  ptr.ptrpropostasiconvano";
				
			$dadosPlanoTrabalho = $db->carregar($sql);
			
			$dadosPlanoTrabalho[0]['cnpj'] = substr($dadosPlanoTrabalho[0]['cnpj'],0,2) . "." .
			substr($dadosPlanoTrabalho[0]['cnpj'],2,3) . "." .
			substr($dadosPlanoTrabalho[0]['cnpj'],5,3) . "/" .
			substr($dadosPlanoTrabalho[0]['cnpj'],8,4) . "-" .
			substr($dadosPlanoTrabalho[0]['cnpj'],12,2);

			$cabecalho = htmlcabecalhoPTA( $dadosPlanoTrabalho[0] );
				
			return $cabecalho;
		}
		else {
			echo "<script>
					alert('Número do Plano de Trabalho inválido.');
					window.location.href = 'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A';
				  </script>";
			die;
		}
	}
	else {
		echo "<script>
				alert('Você deve primeiramente selecionar um Plano de Trabalho.');
				window.location.href = 'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A';
			  </script>";
		die;
	}
}

function htmlcabecalhoPTA( $dadosPTA ){
	$titleValorRecurso = 'Composto pela soma do valor do recurso disponibilizado para a entidade pelo concedente e o valor da contrapartida informado pelo proponente anterior ao preenchimento do PTA (Plano de Trabalho)';
	$titleValorPTA = 'Composto pelos valores concedente e proponente detalhados nas iniciativas do PTA (Plano de Trabalho).';
	
	if( $dadosPTA['valor'] == $dadosPTA['ptrvalorconcedente'] ){
		$corFonte = 'color: blue;';
	} else {
		$corFonte = 'color: red;';
	}
	
	if( !$boAnalise ){
		$pendencia = "<div style=\"float:right\" ><a onclick=\"window.open('emenda.php?modulo=principal/validacaoPTA&acao=A&popup=true','Indicador','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');\" href=\"#\" >Pendências PTA</a></div>";
	}
		
	if( $dadosPTA['ptrpropostasiconv'] ){
		$propostasiconv = $dadosPTA['ptrpropostasiconv'].'/'.$dadosPTA['ptrpropostasiconvano'];
	} else {
		$propostasiconv = '<span style="color: red;">Proposta não enviada</span>';
	}
	
	if( $_SESSION['exercicio'] == '2014' ){
	
		$ptrid = $dadosPTA['ptrid'];
		$enbid = $dadosPTA['enbid'];
		if( $ptrid ){
			$boImpositivo = verificaOrcamentoImpositivo( $ptrid );
			//if( $boImpositivo > 0 ) $strIndicacao = verificaPrazoPreenchimentoEmenda( $enbid, $ptrid );
		}
		
		$cabecalho = "<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
						 <tbody>
						 	<tr>
						 		<td colspan=\"4\" class=\"subtitulocentro\">Dados do Plano de Trabalho</td>
						 	</tr>
							<tr>
								<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">Número do PTA / Exercício:</td>
								<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['ptrid']} / {$_SESSION['exercicio']}</td>
								<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">CNPJ:</td>
								<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\"><div><div style=\"float:left\">{$dadosPTA['cnpj']}</div></div>$pendencia</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nível de Ensino do PTA:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['resassunto']}</td>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nome do Órgão ou Entidade:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['orgao_entidade']}</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor total dos recursos:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; color: blue;\" class=\"SubTituloDireita\" title='$titleValorRecurso'>R$ ".number_format($dadosPTA['ptrvalorconcedente'],2,',','.')."</td>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Município / UF:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['endereco']}</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor do PTA:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; $corFonte\" class=\"SubTituloDireita\" title='$titleValorPTA'>R$ ".number_format($dadosPTA['valor'],2,',','.')."</td>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Número Proposta SICONV:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$propostasiconv}</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Redução com base em 1,2% da RCL de 2013:</td>
								<td colspan=3 style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; $corFonte\" class=\"SubTituloDireita\"><span style=\"color: red;\">R$ ".number_format($dadosPTA['edevalorreducao'],2,',','.')."</span></td>
							</tr>
							$strIndicacao
						 </tbody>
						</table>";
	} else if( $_SESSION['exercicio'] == '2013' ){
		$cabecalho = "<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
						 <tbody>
						 	<tr>
						 		<td colspan=\"4\" class=\"subtitulocentro\">Dados do Plano de Trabalho</td>
						 	</tr>
							<tr>
								<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">Número do PTA / Exercício:</td>
								<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['ptrid']} / {$_SESSION['exercicio']}</td>
								<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">CNPJ:</td>
								<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\"><div><div style=\"float:left\">{$dadosPTA['cnpj']}</div></div>$pendencia</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nível de Ensino do PTA:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['resassunto']}</td>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nome do Órgão ou Entidade:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['orgao_entidade']}</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor total dos recursos:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; color: blue;\" class=\"SubTituloDireita\" title='$titleValorRecurso'>R$ ".number_format($dadosPTA['ptrvalorconcedente'],2,',','.')."</td>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Município / UF:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['endereco']}</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor do PTA:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; $corFonte\" class=\"SubTituloDireita\" title='$titleValorPTA'>R$ ".number_format($dadosPTA['valor'],2,',','.')."</td>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Número Proposta SICONV:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$propostasiconv}</td>
							</tr>
						 </tbody>
						</table>";
	} else {
		$cabecalho = "<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
						 <tbody>
						 	<tr>
						 		<td colspan=\"4\" class=\"subtitulocentro\">Dados do Plano de Trabalho</td>
						 	</tr>
							<tr>
								<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">Número do PTA / Exercício:</td>
								<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['ptrid']} / {$_SESSION['exercicio']}</td>
								<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">CNPJ:</td>
								<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\"><div><div style=\"float:left\">{$dadosPTA['cnpj']}</div></div>$pendencia</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nível de Ensino do PTA:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['resassunto']}</td>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nome do Órgão ou Entidade:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['orgao_entidade']}</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor total dos recursos:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; color: blue;\" class=\"SubTituloDireita\" title='$titleValorRecurso'>R$ ".number_format($dadosPTA['ptrvalorconcedente'],2,',','.')."</td>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Município / UF:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPTA['endereco']}</td>
							</tr>
							<tr>
								<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor do PTA:</td>
								<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; $corFonte\" class=\"SubTituloDireita\" title='$titleValorPTA'>R$ ".number_format($dadosPTA['valor'],2,',','.')."</td>
							</tr>
						 </tbody>
						</table>";
		
	}
	return $cabecalho;
}

/*
 * Manter entidades beneficiadas
 */

function ajustarDadosEntidade($entnumcpfcnpj) {
	return str_replace(array(".","-","/"),"", $entnumcpfcnpj);
}

function totalizadorRegistroLista($total){
	print '<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">
			<tbody>
			<tr bgcolor="#ffffff">
				<td><b>Total de Registros: '.$total.'</b></td>
			</tr>
			</tbody>
		  </table>';
}

/*
 * manter verifica perfil
 */
function pegaPerfil( $usucpf ){
	global $db;
	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p LEFT JOIN seguranca.perfilusuario AS pu 
			  ON pu.pflcod = p.pflcod
			WHERE 
			  p.sisid = '{$_SESSION['sisid']}'
			  AND pu.usucpf = '$usucpf'
			--ORDER BY p.pflnivel";	

	$pflcod = $db->pegaUm( $sql );
	return $pflcod;
}
function pegaPerfilArray( $usucpf ){
	global $db;
	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p LEFT JOIN seguranca.perfilusuario AS pu 
			  ON pu.pflcod = p.pflcod
			WHERE 
			  p.sisid = '{$_SESSION['sisid']}'
			  AND pu.usucpf = '$usucpf'
			--ORDER BY p.pflnivel";	

	$pflcod = $db->carregarColuna( $sql );
	return $pflcod;
}

/**
 * Função que verifica se o usuário logado possui ao menos um dos perfis passados por parâmetro
 *
 * @author Wesley Romualdo da Silva - <wesleyromualdo@gmail.com>
 * @since 18/11/2008
 * @param array $pflcods - Array de Perfis
 * @return integer - A quantidade de perfis dentre os informados que o usuário logado possui
 */
function emenda_possuiPerfil( $pflcods ){
	global $db;
	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $pflcods ) . " ) ";

	return $db->pegaUm( $sql ) > 0;
}

/*
 * manter plano de trabalho Ação Especificação
 */

function verificaValorTotalRecursoPTA(){
	global $db;

	$ptiid = $_SESSION['emenda']['ptiid'];

	$sql = "SELECT
			  sum(ptir.pervalor) as total
			FROM	
			  emenda.ptiniesprecurso ptir 
			  	INNER JOIN emenda.ptiniciativaespecificacao ptie
			    	ON ptie.pteid = ptir.pteid
			WHERE
			  ptie.ptiid = $ptiid";
		
	$totalRercuso = $db->pegaUm($sql);

	$sql = "SELECT
			  sum(ede.edevalor) as total
			FROM
			  emenda.planotrabalho ptr 
			  	INNER JOIN emenda.ptemendadetalheentidade ptede
			    	ON ptede.ptrid = ptr.ptrid
			    INNER JOIN emenda.emendadetalheentidade ede
			    	ON ede.edeid = ede.edeid 
			    INNER JOIN emenda.ptiniciativa pti
			    	ON ptr.ptrid = pti.ptrid
			WHERE
			  ede.edestatus = 'A'
			  AND pti.ptiid = $ptiid";

	$totalRercusoPTA = $db->pegaUm($sql);

	if($totalRercuso <= $totalRercusoPTA){
		return true;
	}else{
		return false;
	}
}

function manterDadosPTARecurso($post, $pteid){
	global $db;

	$arValorRecurso = explode('|', $post['valorrecurso']);

	if($post['perid']){
		for($i=0; $i<sizeof($arValorRecurso); $i++){
			$sql = "UPDATE
					  emenda.ptiniesprecurso  
					SET 
					  pteid = $pteid,
					  pedid = {$post['pedid']},
					  pervalor = {$arValorRecurso[$i]}
					 
					WHERE 
					  perid = {$post['perid']}";
				
			echo $sql."<br>";
			$db->executar($sql);
		}
	}else{
		for($i=0; $i<sizeof($arValorRecurso); $i++){
			$sql = "INSERT INTO
					  emenda.ptiniesprecurso(
					  pteid,
					  pedid,
					  pervalor
					) 
					VALUES (
					$pteid,
					{$post['pedid']},
					{$arValorRecurso[$i]}
					)";
						
					$db->executar($sql);
						
		}
	}
	$db->commit();

}

#não pode excluir
function validaSessionPTA($session, $boPTA = true){
	if( $boPTA ){
		if( empty($session) ){
			echo "<script>
					alert('Faltam dados na sessão sobre este PTA.');
					window.location.href = 'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A';
				  </script>";
			die();
		}
	} else {
		if( empty($session) ){
			echo "<script>
					alert('Faltam dados na sessão.');
					window.location.href = 'emenda.php?modulo=principal/listaPtaAnalise&acao=A';
				  </script>";
			die();
		}
	}
}

function iniciativaEspecificacaoRecursoAjax( $ptiid, $pteid = '' ){

	global $db;

	if ( !empty($pteid) ){
		$select = " pr.pervalor, pr.perid,";

		$join = " LEFT JOIN emenda.ptiniesprecurso pr
						ON pr.pedid = ped.pedid AND pr.pteid = {$pteid}";

	}

	$sql = "SELECT DISTINCT
			    vede.edeid, 
				vede.emecod,
				(CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
					ELSE 'Complemento' END) as tipoemenda, 
			    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome, 
			    aut.autid, 
			    fup.fupdsc,
				ped.pedvalor,
                (SELECT SUM(sper.pervalor)
                 FROM emenda.ptiniesprecurso sper
                 WHERE sper.pedid = ped.pedid) as pervalorutilizado,
                 {$select}
			    ped.pedid
			FROM emenda.planotrabalho ptr
				INNER JOIN emenda.ptemendadetalheentidade  ped
			    	ON ptr.ptrid = ped.ptrid
			    INNER JOIN emenda.v_emendadetalheentidade vede
			    	ON vede.edeid = ped.edeid
			    left JOIN emenda.v_funcionalprogramatica fup
			    	ON (vede.acaid = fup.acaid)
			    INNER JOIN emenda.autor aut
			    	ON aut.autid = vede.autid
			    INNER JOIN emenda.ptiniciativa pti
			    	on pti.ptrid = ptr.ptrid
			    	{$join}
			WHERE ptr.ptrid = ".$_SESSION['emenda']['ptrid']."
				AND pti.ptiid = $ptiid
			    AND vede.edestatus = 'A'";

			    	$arDados = $db->carregar($sql);
			    	?>
<span> Abaixo estão listadas os recursos do PTA que poderão financiar a
especificação. Indique o valor a ser utilizado em cada recurso:</span>
<table id="tblform" class="listagem" width="80%" bgcolor="#f5f5f5"
	cellSpacing="1" cellPadding="3" align="left">
	<thead>
		<tr>
			<td>&nbsp;</td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Código</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
			<td align="Center" class="title" width="25%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Autor</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Total
			da Emenda</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(-)
			Já Utilizado</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(=)
			Valor Disponível</strong></td>
			<td align="Center" class="title" width="25%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor</strong></td>
		</tr>
	</thead>
	<?php
	if($arDados){
		if(sizeof($arDados) == 1){
			$boValor = 'true';
			echo '<input type="hidden" name="boPedid" id="boPedid" value="'.$arDados[0]['pedid'].'">';
		}else{
			$boValor = 'false';
		}
		echo '<input type="hidden" name="boValor" id="boValor" value="'.$boValor.'">';
		foreach ($arDados as $key => $value) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			$pervalorUtilizado = $value['pervalorutilizado'] - $value["pervalor"];
			$value["pervalor"] = number_format($value["pervalor"] , 2, ',', '.');
			?>
	<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>"
		onmouseout="this.bgColor='<?=$cor?>';"
		onmouseover="this.bgColor='#ffffcc';">
		<td><img src="/imagens/seta_filho.gif" border="0"></td>
		<td style="text-align: center;"><?=$value['emecod']; ?></td>
		<td style="text-align: center;"><?=$value['tipoemenda']; ?></td>
		<td style="text-align: center;"><?=$value['autnome']; ?></td>
		<td style="text-align: center;"><?=number_format($value['pedvalor'],2,',','.'); ?></td>
		<td style="text-align: center;"><?=number_format($pervalorUtilizado,2,',','.'); ?></td>
		<td style="text-align: center;"><?=number_format( $value['pedvalor'] - $pervalorUtilizado,2,',','.'); ?></td>
		<td style="text-align: center;"><input
			id="pervalor_<?=$value['pedid'];?>" class="normal" type="text"
			title="" style="width: 43ex;"
			onblur="MouseBlur(this); formataValor(this); calculaEmendaEspecificacaoRecurso(); populaHidden(this.value, <?=$value['pedid'];?>); this.value=mascaraglobal('[###.]###,##',this.value)"
			onmouseout="MouseOut(this);"
			onfocus="MouseClick(this);this.select();"
			onmouseover="MouseOver(this);"
			onkeyup="this.value=mascaraglobal('[###.]###,##',this.value);"
			value="<?=$value["pervalor"] ?>" maxlength="15" size="41"
			name="pervalor" /></td>
		<input type="hidden" name="pervalor[<?=$value['pedid'];?>]"
			id="pervalor[<?=$value['pedid'];?>]" value="">
		<input type="hidden" name="emecod[<?=$key;?>]" id="emecod[<?=$key;?>]"
			value="<?=$value['emecod']; ?>">
	</tr>
	<?
}
?>
	<tr>
		<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">Total do
		concedente:</td>
		<td style="text-align: left;"><span id="concedente2">R$ 0,00</span></td>
	</tr>
	<tr>
		<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">(-)Total
		informado:</td>
		<td style="text-align: left;"><span id="informado">R$ 0,00</span></td>
	</tr>
	<tr>
		<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">(=)Diferença:</td>
		<td style="text-align: left;"><span id="restante">R$ 0,00</span></td>
	</tr>
	<?


} else{
	print '<table width="80%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">';
	print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	print '</table>';
}
echo "</table>";
}

function ListarbeneficiariosAcaoPlanoTrabalho($ptiid){
	global $db;

	$sql = "SELECT DISTINCT
			  ptib.ptbquantidaderural as rural,
			  ptib.ptbquantidadeurbana as urbana,
			  ben.bennome as beneficiario,
			  (ptib.ptbquantidaderural + ptib.ptbquantidadeurbana) as total
			FROM
			  emenda.ptiniciativa pti INNER JOIN emenda.ptiniciativabeneficiario ptib 
			  ON (pti.ptiid = ptib.ptiid) RIGHT JOIN emenda.iniciativabeneficiario inb
			  ON (ptib.icbid = inb.icbid) INNER JOIN emenda.beneficiario ben
			  ON (inb.benid = ben.benid)
			WHERE
			  inb.icbstatus = 'A'
			  AND ben.benstatus = 'A' ".($ptiid ? " AND pti.ptiid = $ptiid " : "")."
		    ORDER BY ben.bennome";

	$dadosben = $db->carregar($sql);
	//$dadosben = $dadosben ? $dadosben : array();
	if($dadosben){
		?>
	<table class='listagem' width='100%'>
		<thead>
			<tr>
				<td width='60%'></td>
				<td align='center'><b>Rural</b></td>
				<td align='center'><b>Urbana</b></td>
				<td align='center'><b>Total</b></td>
			</tr>
		</thead>
		<tbody>
		<?php	foreach( $dadosben as $benef ){
			echo "	<tr>
						<td align='center'><b>".$benef['beneficiario']."</b></td>
						<td align='center'>".$benef['rural']."</td>
						<td align='center'>".$benef['urbana']."</td>
						<td align='center'>".$benef['total']."</td>
					<tr>";
		}
		?>
		</tbody>
	</table>
	<?
} else {
	?>
	<table class='listagem' width='100%'>
		<tr style="text-align: center;">
			<td>-</td>
		</tr>
	</table>
	<?
}
}

function mostraCronogramaExecucaoConcedente($ptrid, $totalConcedente){
	global $db;
	?>
	<input type="text" name="totalProponente" id="totalProponente"
		value="<?=$totalConcedente; ?>">
	<input type="text" name="totalInformadoC" id="totalInformadoC" value="">
	<input type="text" name="totalNaoInformadoC" id="totalNaoInformadoC"
		value="">
	<table id="tblform" class="listagem" width="60%" bgcolor="#f5f5f5"
		cellSpacing="1" cellPadding="3" align="center">
		<thead>
			<tr>
				<td align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Mês</strong></td>
				<td align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ano</strong></td>
				<td align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor</strong></td>
				<td align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
			</tr>
		</thead>
		<?
		$arMes = array(array("codigo" => "1", "descricao" => "Janeiro"),
		array("codigo" => "2", "descricao" => "Fevereiro"),
		array("codigo" => "3", "descricao" => "Março"),
		array("codigo" => "4", "descricao" => "Abril"),
		array("codigo" => "5", "descricao" => "Maio"),
		array("codigo" => "6", "descricao" => "Junho"),
		array("codigo" => "7", "descricao" => "Julho"),
		array("codigo" => "8", "descricao" => "Agosto"),
		array("codigo" => "9", "descricao" => "Setembro"),
		array("codigo" => "10", "descricao" => "Outubro"),
		array("codigo" => "11", "descricao" => "Novembro"),
		array("codigo" => "12", "descricao" => "Dezembro")
		);
			
		$arAno = array();
		$quantAno = ( (int) $_SESSION['exercicio'] + 1) - 2008;
		$n = substr('2008', 0, strlen('2008') - 2 );
		$n1 = substr('2008', strlen('2008')-2, strlen('2008'));

		for($y=0; $y<=$quantAno; $y++){
			$s = ($n1 + $y);
			if(strlen((String) $s) == 1){
				$s = '0'.$s;
			}
			$ano = $n . $s;
			$arAno[] = array("codigo" => $ano, "descricao" =>$ano );
		}

		?>
		<tr>
			<td style="text-align: center;"><?=$db->monta_combo("ptcmes", $arMes, 'S','-- Selecione um mês --','', '', '',150,'S','ptcmes', '', '', 'Especificação da Ação');?></td>
			<td style="text-align: center;"><?=$db->monta_combo("ptcano", $arAno, 'S','-- Selecione um ano --','', '', '',150,'S','ptcano', '', '', 'Especificação da Ação');?></td>
			<td style="text-align: center;"><?=campo_texto( 'ptcvalor', 'S', 'S', 'Valor Unitário Padrão', 40, 17, '[###.]###,##', '','','','','id="ptcvalor"');?></td>
			<td style="text-align: center;"><input type="button" value="Incluir"
				name="btnIncluir" id="btnIncluir" onclick="incluirConcedente;"> <input
				type="button" value="Cancelar" name="btnCancelar" id="btnCancelar"
				onclick="Cancelar();"></td>
		</tr>
		<?php
		$sql = "SELECT
				  ( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptcid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">  '  || 
			            '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptcid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao, 
				  ptcid,
				  ptrid,
				  ptctipo,
				  ptcdata,
				  to_char(ptcdata, 'YYYY') as ptcano,
				  ptcvalor
				FROM 
				  emenda.ptcronogramadesembolso
				WHERE ptrid = $ptrid
				  AND ptctipo = 'C'";

		$arCronograma = $db->carregar($sql);

		foreach ($arCronograma as $key => $value) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			?>
		<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>"
			onmouseout="this.bgColor='<?=$cor?>';"
			onmouseover="this.bgColor='#ffffcc';" style="text-align: center;">
			<td><?=retornaMesExtenso($value['ptcdata']); ?></td>
			<td><?=$value['ptcano']; ?></td>
			<td style="color: rgb(0, 102, 204);">R$ <?=number_format($value['ptcvalor'], 2, ',', '.'); ?></td>
			<td><?=$value['acao']; ?></td>
		</tr>
		<?php
		$totalInformado += $value['ptcvalor'];
}

?>
	</table>
	<table id="tblform" class="listagem" width="60%" bgcolor="#f5f5f5"
		cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#dcdcdc">
			<td colspan="5" style="text-align: right;"><b>VALOR A SER
			DESEMBOLSADO PELO CONCEDENTE</b></td>
			<td style="text-align: right; color: rgb(0, 102, 204);"><b>R$ <?=number_format($totalConcedente, 2, ',', '.'); ?></b></td>
		</tr>
		<tr bgcolor="#dcdcdc">
			<td colspan="5" style="text-align: right;"><b>VALOR JÁ INFORMADO</b>(no
			cronograma de desembolso - Concedente)</td>
			<td style="text-align: right;">
			<div id="informadoC">R$ <?=number_format($totalInformado, 2, ',', '.'); ?></div>
			</td>
		</tr>
		<tr bgcolor="#dcdcdc">
			<td colspan="5" style="text-align: right;"><b>VALOR AINDA NÃO
			INFORMADO</b>(no cronograma de desembolso - Concedente)</td>
			<td style="text-align: right; color: rgb(0, 102, 204);">
			<div id="NinformadoC">R$ <?=number_format($totalConcedente - $totalInformado, 2, ',', '.'); ?></div>
			</td>
		</tr>
	</table>
	<?php
}

function mostraCronogramaExecucaoProponente($ptrid, $totalProponente){
	global $db;
	?>
	<input type="text" name="totalProponente" id="totalProponente"
		value="<?=$totalProponente; ?>">
	<input type="text" name="totalInformadoP" id="totalInformadoP" value="">
	<input type="text" name="totalNaoInformadoP" id="totalNaoInformadoP"
		value="">

	<table id="tblform" class="listagem" width="60%" bgcolor="#f5f5f5"
		cellSpacing="1" cellPadding="3" align="center">
		<thead>
			<tr>
				<td align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Mês</strong></td>
				<td align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ano</strong></td>
				<td align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor</strong></td>
				<td align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
			</tr>
		</thead>
		<?
		$arMes = array(array("codigo" => "1", "descricao" => "Janeiro"),
		array("codigo" => "2", "descricao" => "Fevereiro"),
		array("codigo" => "3", "descricao" => "Março"),
		array("codigo" => "4", "descricao" => "Abril"),
		array("codigo" => "5", "descricao" => "Maio"),
		array("codigo" => "6", "descricao" => "Junho"),
		array("codigo" => "7", "descricao" => "Julho"),
		array("codigo" => "8", "descricao" => "Agosto"),
		array("codigo" => "9", "descricao" => "Setembro"),
		array("codigo" => "10", "descricao" => "Outubro"),
		array("codigo" => "11", "descricao" => "Novembro"),
		array("codigo" => "12", "descricao" => "Dezembro")
		);
			
		$arAno = array();
		$quantAno = ( (int) $_SESSION['exercicio'] + 1) - 2008;
		$n = substr('2008', 0, strlen('2008') - 2 );
		$n1 = substr('2008', strlen('2008')-2, strlen('2008'));

		for($y=0; $y<=$quantAno; $y++){
			$s = ($n1 + $y);
			if(strlen((String) $s) == 1){
				$s = '0'.$s;
			}
			$ano = $n . $s;
			$arAno[] = array("codigo" => $ano, "descricao" =>$ano );
		}

		?>
		<tr>
			<td style="text-align: center;"><?=$db->monta_combo("ptcmes", $arMes, 'S','-- Selecione um mês --','', '', '',150,'S','ptcmes', '', '', 'Especificação da Ação');?></td>
			<td style="text-align: center;"><?=$db->monta_combo("ptcano", $arAno, 'S','-- Selecione um ano --','', '', '',150,'S','ptcano', '', '', 'Especificação da Ação');?></td>
			<td style="text-align: center;"><?=campo_texto( 'ptcvalor', 'S', 'S', 'Valor Unitário Padrão', 40, 17, '[###.]###,##', '','','','','id="ptcvalor"');?></td>
			<td style="text-align: center;"><input type="button" value="Incluir"
				name="btnIncluir" id="btnIncluir" onclick="incluirProponente();"> <input
				type="button" value="Cancelar" name="btnCancelar" id="btnCancelar"
				onclick="Cancelar();"></td>
		</tr>
		<?php
		$sql = "SELECT
				  ( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptcid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">  '  || 
			            '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptcid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao, 
				  ptcid,
				  ptrid,
				  ptctipo,
				  ptcdata,
				  to_char(ptcdata, 'YYYY') as ptcano,
				  ptcvalor
				FROM 
				  emenda.ptcronogramadesembolso
				WHERE ptrid = $ptrid
				  AND ptctipo = 'P'";

		$arCronograma = $db->carregar($sql);

		foreach ($arCronograma as $key => $value) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			?>
		<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>"
			onmouseout="this.bgColor='<?=$cor?>';"
			onmouseover="this.bgColor='#ffffcc';" style="text-align: center;">
			<td><?=retornaMesExtenso($value['ptcdata']); ?></td>
			<td><?=$value['ptcano']; ?></td>
			<td style="color: rgb(0, 102, 204);">R$ <?=number_format($value['ptcvalor'], 2, ',', '.'); ?></td>
			<td><?=$value['acao']; ?></td>
		</tr>
		<?php
		$totalInformado += $value['ptcvalor'];
}

?>
	</table>
	<table id="tblform" class="listagem" width="60%" bgcolor="#f5f5f5"
		cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#dcdcdc">
			<td colspan="5" style="text-align: right;"><b>VALOR A SER
			DESEMBOLSADO PELO PROPONENTE</b></td>
			<td style="text-align: right; color: rgb(0, 102, 204);"><b>R$ <?=number_format($totalProponente, 2, ',', '.'); ?></b></td>
		</tr>
		<tr bgcolor="#dcdcdc">
			<td colspan="5" style="text-align: right;"><b>VALOR JÁ INFORMADO</b>(no
			cronograma de desembolso - Proponente)</td>
			<td style="text-align: right;">
			<div id="informadoC">R$ <?=number_format($totalInformado, 2, ',', '.'); ?></div>
			</td>
		</tr>
		<tr bgcolor="#dcdcdc">
			<td colspan="5" style="text-align: right;"><b>VALOR AINDA NÃO
			INFORMADO</b>(no cronograma de desembolso - Proponente)</td>
			<td style="text-align: right; color: rgb(0, 102, 204);">
			<div id="NinformadoC">R$ <?=number_format($totalProponente - $totalInformado, 2, ',', '.'); ?></div>
			</td>
		</tr>
	</table>
	<?php
}

function filtraAcao($pacid){
	global $db;

	$sql = "SELECT count(*) FROM emenda.ptacao
			WHERE pacid = $pacid";
	$qtaca = $db->pegaUm($sql);
	if($qtaca>0){
		echo "<script>alert('Esta ação já está cadastrada!');</script>";
		echo $db->monta_combo('pacid',$sql,(($pacid)?'S':'N'),'Selecione...',"filtraAcao",'','','','S','','',$pacid);
	}


}

/*
 * PLANO DE TRABALHO RELATÓRIO
 */

/*
 * RELATÓRIO DECLARAÇÃO ADIMPLENCIA
 */
function relatDeclaracaoAdimplenciaPlanoTrabalho($width, $arDados, $bgColor){
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b><span style="text-align: center; font-size: 9px">DECLARAÇÃO DE
					ADIMPLÊNCIA</span></b></td>
					<td class="fot">ANEXO<br>
					1</td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES
					DO FNDE</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - CNPJ</b></td>
					<td><b>2 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnumcpfcnpj']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>3 - MUNICÍPIO</b></td>
					<td><b>4 - UF</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>DECLARAÇÃO</b></td>
				</tr>
				<tr>
					<td
						style="font-family: arial; font-size: 12px; text-align: justify;"><br>
					<br>
					<br>
					<br>
					<center>DECLARAÇÃO DE CUMPRIMENTO DOS CONDICIONANTES LEGAIS</center>
					<br>
					<br>
					<br>
					<br>
					<br>
					&nbsp;&nbsp;&nbsp;&nbsp; Na qualidade de representante legal do
					proponente, DECLARO, para fins de prova junto ao FUNDO NACIONAL DE
					DESENVOLVIMENTO DA EDUCAÇÃO - FNDE, para os efeitos e sob as penas
					da lei, que o convenente: <br>
					<br>
					a) acha-se em dia quanto ao pagamento de tributos, empréstimos e
					financiamentos devidos à União, inclusive no que concerne às
					contribuições relativas ao PIS/PASEP, de que trata o Art. 239 da
					Constituição Federal; <br>
					<br>
					b) acha-se em dia quanto à prestação de contas de recursos
					anteriormente recebidos da União; <br>
					<br>
					c) fez previsão orçamentária da contrapartida, no valor previsto no
					presente instrumento de transferências voluntárias, conforme § 1º
					art. 44 da Lei 11.178 de 20 de setembro de 2005 - Lei de Diretrizes
					Orçamentárias. <br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					<br>
					</td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>
			<? echo autenticacaoRelatPlanoTrabalho('5','AUTENTICAÇÃO DA DECLARAÇÃO', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}

/*
 * RELATÓRIO DESCRIÇÃO DO PROJETO
 */
function relatDescricaoProjetoPlanoTrabalho($width, $arDados, $bgColor){
	global $db;

	$sql = "SELECT DISTINCT
				pt.ptrexercicio,
			    p.pronome,
			    te.tpedesc,
			    e.emecodigo
			FROM
			    emenda.planotrabalho pt INNER JOIN emenda.programaemenda pe
			    ON (pt.preid = pe.preid) INNER JOIN emenda.programa p
			    ON (pe.proid = p.proid) INNER JOIN public.tipoensino te
			    ON (p.tpeid = te.tpeid) INNER JOIN emenda.emenda e
			    ON (pe.emeid = e.emeid)
			WHERE
				pt.ptrstatus = 'A'
				AND te.tpestatus = 'A'
			    AND pt.ptrid = ".$arDados['ptrid'];

	$arRegistro = $db->pegaLinha($sql);

	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b><span style="text-align: center; font-size: 9px">DESCRIÇÃO DO
					PROJETO</span></b></td>
					<td class="fot">ANEXO<br>
					2</td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES
					DO FNDE</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - EXERCÍCIO</b></td>
					<td><b>2 - NÍVEL DE ENSINO</b></td>
					<td><b>3 - PROGRAMA</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['ptrexercicio']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['tpedesc']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['pronome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>4 - CNPJ</b></td>
					<td><b>5 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnumcpfcnpj']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>6 - MUNICÍPIO</b></td>
					<td><b>7 - UF</b></td>
					<td><b>8 - EMENDA Nº</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['emecodigo']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>9 - AÇÃO A SER EXECUTADA</b></td>
				</tr>
				<tr>
				<?
				$sql = "SELECT DISTINCT
					pg.pacid,
					ac.acanome
				FROM emenda.acao ac INNER JOIN emenda.programaacao pg
					ON (ac.acaid = pg.acaid) INNER JOIN emenda.programa p
				    ON (pg.proid = p.proid) INNER JOIN emenda.planotrabalho pt
				    ON (pt.proid = p.proid) INNER JOIN emenda.ptacao pta
				    ON (pt.ptrid = pta.ptrid)
				WHERE
				    pt.ptrid = ".$arDados['ptrid']."
				    AND ac.acastatus = 'A' 
				    AND pg.pacstatus = 'A'
				    AND pt.ptrstatus = 'A'";

				$arPlano = $db->carregar($sql);
				?>
					<td><?
					if($arPlano){
						foreach ($arPlano as $value) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['acanome']."<br>";
		   	}
					}else{
						echo "&nbsp;";
					}
					?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>10 - JUSTIFICATIVA DO PROJETO</b></td>
				</tr>
				<tr>
					<td style="height: 200px;" valign="top"><?=$arDados['ptrjustificativa']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>

			<? echo autenticacaoRelatPlanoTrabalho('11','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}


/*
 * RELATÓRIO DESCRIÇÃO DO PROJETO CONTINUAÇÃO
 */
function relatDescricaoProjetoPlanoTrabalhoContinuacao($width, $arDados, $bgColor){
	global $db;

	$sql = "SELECT DISTINCT
				pt.ptrexercicio,
			    p.pronome,
			    te.tpedesc,
			    e.emecodigo
			FROM
			    emenda.planotrabalho pt INNER JOIN emenda.programaemenda pe
			    ON (pt.preid = pe.preid) INNER JOIN emenda.programa p
			    ON (pe.proid = p.proid) INNER JOIN public.tipoensino te
			    ON (p.tpeid = te.tpeid) INNER JOIN emenda.emenda e
			    ON (pe.emeid = e.emeid)
			WHERE
				pt.ptrstatus = 'A'
				AND te.tpestatus = 'A'
			    AND pt.ptrid = ".$arDados['ptrid'];

	$arRegistro = $db->pegaLinha($sql);
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b><span style="text-align: center; font-size: 9px">DESCRIÇÃO DO
					PROJETO</span></b></td>
					<td style="text-align: center;"><span class="fot">ANEXO<br>
					2</span><br>
					(Continuação)</td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" class="lista" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES
					DO FNDE</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - EXERCÍCIO</b></td>
					<td><b>2 - NÍVEL DE ENSINO</b></td>
					<td><b>3 - PROGRAMA</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['ptrexercicio']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['tpedesc']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['pronome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>4 - CNPJ</b></td>
					<td><b>5 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnumcpfcnpj']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>6 - MUNICÍPIO</b></td>
					<td><b>7 - UF</b></td>
					<td><b>8 - EMENDA Nº</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['emecodigo']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>9 - JUSTIFICATIVA DO PROJETO</b></td>
				</tr>
				<tr>
					<td style="height: 200px;" valign="top"><?=$arDados['ptrjustificativa']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>

			<? echo autenticacaoRelatPlanoTrabalho('10','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}

/*
 * RELATÓRIO DETALHAMENTO DA AÇÃO
 */
function relatDetalhamentoAcaoPlanoTrabalho($width, $arDados, $bgColor){
	global $db;
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b> <span style="text-align: center; font-size: 9px">DETALHAMENTO
					DA AÇÃO</span></b></td>
					<td class="fot">ANEXO<br>
					3</td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" class="lista" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES
					DO FNDE</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>2 - MUNICÍPIO</b></td>
					<td><b>3 - UF</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>4 - AÇÃO A SER EXECUTADA</b></td>
				</tr>
				<tr>
				<?
				$sql = "SELECT DISTINCT
				    ac.acanome
				FROM emenda.acao ac INNER JOIN emenda.programaacao pg
				    ON (ac.acaid = pg.acaid) INNER JOIN emenda.ptacao pta
				    ON (pg.pacid = pta.pacid) INNER JOIN emenda.planotrabalho pt
				    ON (pta.ptrid = pt.ptrid)
				WHERE
				    pt.ptrid = ".$arDados['ptrid']."
				    AND ac.acastatus = 'A' 
				    AND pg.pacstatus = 'A'
				    AND pt.ptrstatus = 'A'";

				$arPlano = $db->carregar($sql);
				?>
					<td><?
					if($arPlano){
						foreach ($arPlano as $value) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['acanome']."<br>";
						}
					}else{
						echo "&nbsp;";
					}
					?></td>
				</tr>
			</table>
			<?
			$sql = "SELECT DISTINCT
			    pta.ptaid,
			    b.bennome,
			    ptab.ptbquantidaderural,
			    ptab.ptbquantidadeurbana,
			    (ptab.ptbquantidaderural + ptab.ptbquantidadeurbana ) as total
			FROM emenda.planotrabalho pt INNER JOIN emenda.ptacao pta
				ON (pt.ptrid = pta.ptrid) INNER JOIN emenda.ptacaobeneficiario ptab
			    ON (pta.ptaid = ptab.ptaid) INNER JOIN emenda.acaobeneficiario ab
			    ON (ptab.acbid = ab.acbid) INNER JOIN emenda.beneficiario b
			    ON (ab.benid = b.benid)
			WHERE
				pt.ptrstatus = 'A'
			    AND ab.acbstatus = 'A'
			    AND b.benstatus = 'A'
			    AND pt.ptrid = ".$arDados['ptrid']."
			ORDER BY b.bennome";

			$arBen = $db->carregar($sql);
			?>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td colspan="4"><b>5 - BENEFICIÁRIOS DA AÇÃO</b></td>
				</tr>
				<tr <?=$bgColor; ?>>
					<td style="text-align: center;"><b>5.1 - BENEFICIÁRIO</b></td>
					<td style="text-align: center;"><b>5.2 - ZONA RURAL</b></td>
					<td style="text-align: center;"><b>5.3 - ZONA URBANA</b></td>
					<td style="text-align: center;"><b>5.4 - TOTAL</b></td>
				</tr>
				<?
				$i = 0;
				if($arBen){
					foreach ($arBen as $valor) { $i++;?>
				<tr>
					<td <?=$bgColor; ?>><b>5.1.<?=$i; ?> - <?=$valor['bennome']; ?></b></td>
					<td style="text-align: center;"><b><?=$valor['ptbquantidaderural']; ?></b></td>
					<td style="text-align: center;"><b><?=$valor['ptbquantidadeurbana']; ?></b></td>
					<td style="text-align: center;"><b><?=$valor['total']; ?></b></td>
				</tr>
				<?	}
}else{
	?>
				<tr>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
				</tr>
				<?
}
?>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>6 - DETALHAMENTO DA AÇÃO</b></td>
				</tr>
				<?
				$sql = "SELECT
				pta.ptadescricao 
			FROM emenda.planotrabalho pt INNER JOIN emenda.ptacao pta
				ON (pt.ptrid = pta.ptrid)
			WHERE
				pt.ptrstatus = 'A'
			    AND pt.ptrid = ".$arDados['ptrid']."
			ORDER BY pta.ptadescricao";

				$arDetalhe = $db->carregar($sql);
				?>
				<tr>
					<td style="height: 200px;" valign="top"><?
					if($arDetalhe){
						foreach ($arDetalhe as $value) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['ptadescricao']."<br>";
		   	}
					}else{
						echo "&nbsp;";
					}
					?></td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>

			<? echo autenticacaoRelatPlanoTrabalho('7','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}

/*
 * RELATÓRIO DETALHAMENTO DA AÇÃO CONTINUAÇÃO
 */
function relatDetalhamentoAcaoContinuacaoPlanoTrabalho($width, $arDados, $bgColor){
	global $db;
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b> <span style="text-align: center; font-size: 9px">DETALHAMENTO
					DA AÇÃO</span></b></td>
					<td class="fot">ANEXO<br>
					3</td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" class="lista" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES
					DO FNDE</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>2 - MUNICÍPIO</b></td>
					<td><b>3 - UF</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>4 - AÇÃO A SER EXECUTADA</b></td>
				</tr>
				<tr>
				<?
				$sql = "SELECT DISTINCT
				    ac.acanome
				FROM emenda.acao ac INNER JOIN emenda.programaacao pg
				    ON (ac.acaid = pg.acaid) INNER JOIN emenda.ptacao pta
				    ON (pg.pacid = pta.pacid) INNER JOIN emenda.planotrabalho pt
				    ON (pta.ptrid = pt.ptrid)
				WHERE
				    pt.ptrid = ".$arDados['ptrid']."
				    AND ac.acastatus = 'A' 
				    AND pg.pacstatus = 'A'
				    AND pt.ptrstatus = 'A'";

				$arPlano = $db->carregar($sql);
				?>
					<td><?
					if($arPlano){
						foreach ($arPlano as $value) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['acanome']."<br>";
		   	}
					}else{
						echo "&nbsp;";
					}
					?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>5 - DETALHAMENTO DA AÇÃO - CONTINUAÇÃO</b></td>
				</tr>
				<?
				$sql = "SELECT
				pta.ptadescricao 
			FROM emenda.planotrabalho pt INNER JOIN emenda.ptacao pta
				ON (pt.ptrid = pta.ptrid)
			WHERE
				pt.ptrstatus = 'A'
			    AND pt.ptrid = ".$arDados['ptrid']."
			ORDER BY pta.ptadescricao";

				$arDetalhe = $db->carregar($sql);
				?>
				<tr>
					<td style="height: 200px;" valign="top"><?
					if($arDetalhe){
						foreach ($arDetalhe as $value) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['ptadescricao']."<br>";
		   	}
					}
					?></td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>

			<? echo autenticacaoRelatPlanoTrabalho('6','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}

/*
 * RELATÓRIO ESPECIFICAÇÃO DA AÇÃO
 */
function relatEspecificacaoAcaoPlanoTrabalho($width, $arDados, $bgColor){
	global $db;
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b> <span style="text-align: center; font-size: 9px">ESPECIFICAÇÃO
					DA AÇÃO</span></b></td>
					<td class="fot">ANEXO<br>
					4</td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES
					DO FNDE</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - EXERCÍCIO</b></td>
					<td><b>2 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
					<td><b>3 - MUNICÍPIO</b></td>
					<td><b>4 - UF</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$_SESSION['exercicio']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>5 - AÇÃO A SER EXECUTADA</b></td>
				</tr>
				<tr>
				<?
				$sql = "SELECT DISTINCT
				    ac.acanome
				FROM emenda.acao ac INNER JOIN emenda.programaacao pg
				    ON (ac.acaid = pg.acaid) INNER JOIN emenda.ptacao pta
				    ON (pg.pacid = pta.pacid) INNER JOIN emenda.planotrabalho pt
				    ON (pta.ptrid = pt.ptrid)
				WHERE
				    pt.ptrid = ".$arDados['ptrid']."
				    AND ac.acastatus = 'A' 
				    AND pg.pacstatus = 'A'
				    AND pt.ptrstatus = 'A'";

				$arPlano = (array) $db->carregar($sql);
				?>
					<td><?
					if($arPlano){
						foreach ($arPlano as $value) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['acanome']."<br>";
		   	}
					}else{
						echo "&nbsp;";
					}
					?></td>
				</tr>
			</table>
			<?
			$sql = "SELECT
			e.espnome,
		    e.espunidademedida,
			ptae.ptequantidade,
		    ptae.ptevalorunitario,
		    (ptae.ptequantidade * ptae.ptevalorunitario) as total,
		    ptae.ptevalorproponente,
		  ( CASE WHEN ptae.ptevalorproponente is null THEN (ptae.ptequantidade * ptae.ptevalorunitario)
		         ELSE ((ptae.ptequantidade * ptae.ptevalorunitario) - ptae.ptevalorproponente) END) as concedente
		FROM emenda.planotrabalho pt 
			INNER JOIN emenda.ptacao pta ON (pt.ptrid = pta.ptrid) 
			INNER JOIN emenda.ptacaoespecificacao ptae ON (pta.ptaid = ptae.ptaid) 
			INNER JOIN emenda.acaoespecificacao ae ON (ptae.aceid = ae.aceid) 
		    INNER JOIN emenda.especificacao e ON (ae.espid = e.espid)
		    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
		WHERE pt.ptrstatus = 'A'
			AND ptae.ptestatus = 'A'
		    AND ae.acestatus = 'A'
		    AND e.espstatus = 'A'
		    AND pt.ptrid = ".$arDados['ptrid'];

			$arEspecif = $db->carregar($sql);
			?>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td rowspan="2" style="text-align: center" width="40%"><b>6 -
					ESPECIFICAÇÃO DA AÇÃO</b></td>
					<td colspan="2" style="text-align: center" width="30%"><b>6.1 -
					INDICADOR FÍSICO</b></td>
					<td colspan="2" style="text-align: center" width="30%"><b>6.2 -
					CUSTO</b></td>
				</tr>
				<tr <?=$bgColor; ?>>
					<td style="text-align: center" width="20%"><b>UNIDADE DE MEDIDA</b></td>
					<td style="text-align: center" width="10%"><b>QUANTIDADE</b></td>
					<td style="text-align: center" width="15%"><b>VALOR UNITÁRIO</b></td>
					<td style="text-align: center" width="15%"><b>VALOR TOTAL</b></td>
				</tr>
				<?
				if($arEspecif){
					foreach ($arEspecif as $valor) {
						?>
				<tr>
					<td><?=$valor['espnome']; ?></td>
					<td><?=$valor['espunidademedida']; ?></td>
					<td><?=$valor['ptequantidade']; ?></td>
					<td>R$ <?=number_format($valor['ptevalorunitario'], 2, ',', '.'); ?></td>
					<td>R$ <?=number_format($valor['total'], 2, ',', '.'); ?></td>
				</tr>
				<?
				$totAcao += $valor['total'];
				$totConcedente+= $valor['concedente'];
				$totProponente+= $valor['ptevalorproponente'];
}
}
?>
				<tr>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
				</tr>
				<tr>
					<td <?=$bgColor; ?> colspan="4"><b>7 - TOTAL DA AÇÃO</b></td>
					<td>R$ <?=number_format($totAcao, 2, ',', '.'); ?></td>
				</tr>
				<tr>
					<td <?=$bgColor; ?> colspan="4"><b>7.1 - TOTAL DO PROPONENTE</b></td>
					<td>R$ <?=number_format($totProponente, 2, ',', '.'); ?></td>
				</tr>
				<tr>
					<td <?=$bgColor; ?> colspan="4"><b>7.2 - TOTAL DO CONCEDENTE</b></td>
					<td>R$ <?=number_format($totConcedente, 2, ',', '.'); ?></td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>

			<? echo autenticacaoRelatPlanoTrabalho('8','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}

function dateDiff($sDataInicial, $sDataFinal){
	$sDataI = explode("/", $sDataInicial);
	$sDataF = explode("/", $sDataFinal);

	$nDataInicial = mktime(0, 0, 0, $sDataI[1], $sDataI[0], $sDataI[2]);
	$nDataFinal = mktime(0, 0, 0, $sDataF[1], $sDataF[0], $sDataF[2]);

	return ($nDataInicial > $nDataFinal) ?
	floor(($nDataInicial - $nDataFinal)/84600) : floor(($nDataFinal - $nDataInicial)/84600);
}


/*
 * RELATÓRIO de CRONOGRAMA DE EXECUÇÃO E DESEMBOLSO
 */

function relatCronogramaExecucaoDesembolsoPlanoTrabalho($width, $arDados, $bgColor){
	global $db;
	$sql = "SELECT DISTINCT
				pt.ptrexercicio,
			    p.pronome,
			    te.tpedesc,
			    e.emecodigo
			FROM
			    emenda.planotrabalho pt INNER JOIN emenda.programaemenda pe
			    ON (pt.preid = pe.preid) INNER JOIN emenda.programa p
			    ON (pe.proid = p.proid) INNER JOIN public.tipoensino te
			    ON (p.tpeid = te.tpeid) INNER JOIN emenda.emenda e
			    ON (pe.emeid = e.emeid)
			WHERE
				pt.ptrstatus = 'A'
				AND te.tpestatus = 'A'
			    AND pt.ptrid = ".$arDados['ptrid'];

	$arRegistro = $db->pegaLinha($sql);

	$sql_data = "SELECT
			    to_char(min(ptedatainicio), 'DD/MM/YYYY') as min_dataini, 
			    to_char(max(ptedatafim), 'DD/MM/YYYY') as max_datafim,
			    max(ptedatafim) - min(ptedatainicio) as quantdias
			FROM emenda.ptacaoespecificacao ptae INNER JOIN emenda.ptacao pta
			    ON (ptae.ptaid = pta.ptaid) INNER JOIN emenda.planotrabalho pt
			    ON (pta.ptrid = pt.ptrid)
			WHERE ptae.ptestatus = 'A' 
			    AND pt.ptrstatus = 'A'
				AND pt.ptrid = ".$arDados['ptrid'];

	$arData = $db->pegaLinha($sql_data);

	$mesIni = explode('/', $arData["min_dataini"]);
	$mesFim = explode('/', $arData["max_datafim"]);

	$DataInicio	= retornaMes( $mesIni[1] ) . '/' . $mesIni[2];
	$DataFim	= retornaMes( $mesFim[1] ) . '/' . $mesFim[2];

	$quantDias = $arData['quantdias'];// dateDiff($arData["min_dataini"], $arData["max_datafim"]);

	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b><span style="text-align: center; font-size: 9px">CRONOGRAMA DE
					EXECUÇÃO E DESEMBOLSO</span></b></td>
					<td class="fot">ANEXO<br>
					5</td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES
					DO FNDE</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - EXERCÍCIO</b></td>
					<td><b>2 - NÍVEL DE ENSINO</b></td>
					<td><b>3 - PROGRAMA</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['ptrexercicio']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['tpedesc']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['pronome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>4 - CNPJ</b></td>
					<td><b>5 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnumcpfcnpj']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>6 - MUNICÍPIO</b></td>
					<td><b>7 - UF</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td style="text-align: center;" colspan="6"><b>8 - CRONOGRAMA DE
					EXECUÇÃO</b></td>
				</tr>
				<tr>
					<td <?=$bgColor; ?> width="16%"><b>8.1 - INÍCIO - MÊS/ANO</b></td>
					<td><?=$DataInicio; ?></td>
					<td <?=$bgColor; ?> width="16%"><b>8.2 - TÉRMINO - MÊS/ANO</b></td>
					<td><?=$DataFim; ?></td>
					<td <?=$bgColor; ?> width="16%"><b>8.3 - QUANT. DE DIAS</b></td>
					<td><?=$quantDias; ?></td>
				</tr>
			</table>
			<?
			$sql = "SELECT
			  ptcid,
			  ptrid,
			  ptctipo,
			  ptcdata,
			  ptcvalor
			FROM 
			  emenda.ptcronogramadesembolso
			WHERE ptrid = ".$arDados['ptrid']."
			ORDER BY ptcdata";

			$arDados = $db->carregar($sql);

			/*	$sql = "SELECT
			 ptcid,
			 ptrid,
			 ptctipo,
			 to_char(ptcdata, 'DD/MM/YYYY') as ptcdata,
			 trim(to_char(ptcvalor, '999G999G999D99')) as ptcvalor
			 FROM
			 emenda.ptcronogramadesembolso
			 WHERE ptctipo = 'P'
			 AND ptrid = ".$arDados['ptrid']."
			 ORDER BY ptcdata";

			 $arDadosP = $db->carregar($sql);*/

			$quantAno = $mesFim[2] - $mesIni[2];
			$n = substr($mesIni[2], 0, strlen($mesIni[2]) - 2 );
			$n1 = substr($mesIni[2], strlen($mesIni[2])-2, strlen($mesIni[2]));

			if($arDados){

				for($y=0; $y<=$quantAno; $y++){

					$s = ($n1 + $y);
					if(strlen((String) $s) == 1){
						$s = '0'.$s;
					}
					$ano = $n . $s;

					foreach ($arDados as $valor){
						$mes = explode("-", $valor['ptcdata']);
							
						for($i=1; $i<=12; $i++){

							if(strlen( (String) $i) == 1){
								$i = '0'.$i;
							}

							if($i == $mes[1] && $ano == $mes[0]){

								if($valor['ptctipo'] == "C"){
									$varptcvalor = 'ptcvalorC_'.$ano.'_'.$i;
									$varptcid = 'ptcidC_'.$ano.'_'.$i;

									$$varptcvalor = $valor['ptcvalor'];
									$$varptcid = $valor['ptcid'];
								}else{
									$varptcvalorP = 'ptcvalorP_'.$ano.'_'.$i;
									$varptcidP = 'ptcidP_'.$ano.'_'.$i;

									$$varptcvalorP = $valor['ptcvalor'];
									$$varptcidP = $valor['ptcid'];
								}
							}
						}
					}
				}
			}
			?>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td style="text-align: center;" colspan="6"><b>9 - CRONOGRAMA DE
					DESEMBOLSO - VALORES CONCEDENTE</b></td>
				</tr>
				<?

				for($y=0; $y<=$quantAno; $y++){

					$s = ($n1 + $y);
					if(strlen((String) $s) == 1){
						$s = '0'.$s;
					}
					$ano = $n . $s;

					echo '<tr>
					<td colspan="6" '.$bgColor.' style="font-size: 12px"><b>'.$ano.'</b></td>
				</tr>';

					for($i=1; $i<=12; $i++){
						if(strlen( (String) $i) == 1){
							$i = '0'.$i;
						}

						if($i <= 6 && $i == 1){
							echo '<tr style="text-align: center;" '.$bgColor.'>
							<td><b>JANEIRO</b></td>
							<td><b>FEVEREIRO</b></td>
							<td><b>MARÇO</b></td>
							<td><b>ABRIL</b></td>
							<td><b>MAIO</b></td>
							<td><b>JUNHO</b></td>
						</tr>';
						}else if($i > 6 && $i == 7){
							echo '<tr style="text-align: center;" '.$bgColor.'>
							<td><b>JULHO</b></td>
							<td><b>AGOSTO</b></td>
							<td><b>SETEMBRO</b></td>
							<td><b>OUTUBRO</b></td>
							<td><b>NOVEMBRO</b></td>
							<td><b>DEZEMBRO</b></td>
						</tr>';
						}
						$ptcid = "ptcidC_".$ano."_".$i;
						$ptcvalor = 'ptcvalorC_'.$ano.'_'.$i;


						if($$ptcvalor){
							echo "<td style='text-align: center;'>R$ ".number_format($$ptcvalor, 2, ',', '.')."</td>";
							$valorTotalConcedente += $$ptcvalor;
						}else{
							echo "<td style='text-align: center;'>&nbsp;</td>";
						}

					}
				}
				?>
				<tr>
					<td colspan="5" <?=$bgColor; ?>><b>10 - VALOR TOTAL A SER
					DESEMBOLSADO PELO CONCEDENTE</b></td>
					<td style="text-align: right">R$ <?=number_format($valorTotalConcedente, 2, ',', '.'); ?></td>
				</tr>
				<tr <?=$bgColor; ?>>
					<td colspan="6">&nbsp;</td>
				</tr>
				<tr <?=$bgColor; ?>>
					<td colspan="6" style="text-align: center;"><b>11 - CRONOGRAMA DE
					DESEMBOLSO - VALORES PROPONENTE</b></td>
				</tr>
				<?
				for($y=0; $y<=$quantAno; $y++){

					$s = ($n1 + $y);
					if(strlen((String) $s) == 1){
						$s = '0'.$s;
					}
					$ano = $n . $s;

					echo '<tr>
					<td colspan="6" '.$bgColor.' style="font-size: 12px"><b>'.$ano.'</b></td>
				</tr>';
					for($i=1; $i<=12; $i++){
						if(strlen( (String) $i) == 1){
							$i = '0'.$i;
						}

						if($i <= 6 && $i == 1){
							echo '<tr style="text-align: center;" '.$bgColor.'>
							<td><b>JANEIRO</b></td>
							<td><b>FEVEREIRO</b></td>
							<td><b>MARÇO</b></td>
							<td><b>ABRIL</b></td>
							<td><b>MAIO</b></td>
							<td><b>JUNHO</b></td>
						</tr>';
						}else if($i > 6 && $i == 7){
							echo '<tr style="text-align: center;" '.$bgColor.'>
							<td><b>JULHO</b></td>
							<td><b>AGOSTO</b></td>
							<td><b>SETEMBRO</b></td>
							<td><b>OUTUBRO</b></td>
							<td><b>NOVEMBRO</b></td>
							<td><b>DEZEMBRO</b></td>
						</tr>';
						}

						$ptcvalor = 'ptcvalorP_'.$ano.'_'.$i;

						if($$ptcvalor){
							echo "<td style='text-align: center;'>R$ ".number_format($$ptcvalor, 2, ',', '.')."</td>";
							$valorTotalProponente += $$ptcvalor;
						}else{
							echo "<td style='text-align: center;'>&nbsp;</td>";
						}
					}
				}
				?>
				<tr>
					<td colspan="5" <?=$bgColor; ?>><b>12 - VALOR TOTAL A SER
					DESEMBOLSADO PELO PROPONENTE (valor mínimo de 1%)</b></td>
					<td style="text-align: right">R$ <?=number_format($valorTotalProponente, 2, ',', '.'); ?></td>
				</tr>
				<tr>
					<td colspan="5" <?=$bgColor; ?>><b>13 - VALOR TOTAL DO PROJETO</b></td>
					<td style="text-align: right">R$ &nbsp;</td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>

			<? echo autenticacaoRelatPlanoTrabalho('14','AUTENTICAÇÃO', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}

/*
 * RELATÓRIO DE ESCOLAS BENEFICIADAS PELA AÇÃO
 */

function relatEscolaBeneficiadaAcaoPlanoTrabalho($width, $arDados, $bgColor){
	global $db;
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b><span style="text-align: center; font-size: 9px">ESCOLAS
					BENEFICIADAS PELA AÇÃO</span></b></td>
					<td class="fot">ANEXO<br>
					6</td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center; font-size: 9px;"><b>Utilize
					este formulário para as ações: MAT. DID. PEDAGÓGICO, EQUIP, REFORMA
					E ADAPTAÇÃO DE ESCOLAS <br>
					Obs.: Utilize quantos formulários forem necessários para
					complementação deste anexo</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - EXERCÍCIO</b></td>
					<td><b> 2 - AÇÃO A SER EXECUTADA</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$_SESSION['exercicio']; ?></td>
					<?
					$sql = "SELECT DISTINCT
				    ac.acanome
				FROM emenda.acao ac INNER JOIN emenda.programaacao pg
				    ON (ac.acaid = pg.acaid) INNER JOIN emenda.ptacao pta
				    ON (pg.pacid = pta.pacid) INNER JOIN emenda.planotrabalho pt
				    ON (pta.ptrid = pt.ptrid)
				WHERE
				    pt.ptrid = ".$arDados['ptrid']."
				    AND ac.acastatus = 'A' 
				    AND pg.pacstatus = 'A'
				    AND pt.ptrstatus = 'A'";

					$arPlano = $db->carregar($sql);
					?>
					<td><?
					if($arPlano){
						foreach ($arPlano as $value) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['acanome']."<br>";
		   	}
					}
					?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>3 - CNPJ</b></td>
					<td><b>4 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnumcpfcnpj']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>5 - MUNICÍPIO</b></td>
					<td><b>6 - UF</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
				</tr>
			</table>
			<?
			$sql = "SELECT
				esb.esbid, 
			    ent.entid,
			    entd.entcodent, 
			    ent.entnome, 
			    esb.esbquantidadealunos
			FROM	
				entidade.entidade ent, 
			    entidade.entidade2 ent2, 
			    entidade.entidadedetalhe entd,
			    emenda.ptescolasbeneficiadas esb
			WHERE 
				ent.entid = ent2.entid 
			    AND ent.entid = entd.entid 
			    AND ent.entid = esb.entid 
			    AND ent.entstatus='A' 
			    AND ent2.funid in(3,4,11,12,16,18,56) 
			    AND esb.ptrid = ".$arDados['ptrid'];

			$arEscola = $db->carregar($sql);

			?>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td style="text-align: center;" colspan="3"><b>7 - ESCOLAS
					BENEFICIADAS</b></td>
				</tr>
				<tr>
					<td <?=$bgColor; ?> width="20%"><b>7.1 - CÓDIGO CENSO ESCOLAR</b></td>
					<td <?=$bgColor; ?> width="60%"><b>7.2 - NOME DA ESCOLA</b></td>
					<td <?=$bgColor; ?> width="20%"><b>7.3 - ALUNOS</b></td>
				</tr>
				<?
				if($arEscola){
					foreach ($arEscola as $valor) {
						?>
				<tr>
					<td><?=$valor['entcodent']; ?></td>
					<td><?=$valor['entnome']; ?></td>
					<td><?=$valor['esbquantidadealunos']; ?></td>
				</tr>
				<?
				$totaAlunos += $valor['esbquantidadealunos'];
}
$totalEscola = sizeof($arEscola);
}else{
	?>
				<tr>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
				</tr>
				<?
}
?>
				<tr <?=$bgColor; ?>>
					<td><b>8 - TOTAIS</b></td>
					<td><?=$totalEscola; ?></td>
					<td><?=$totaAlunos; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>

			<? echo autenticacaoRelatPlanoTrabalho('9','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}

/*
 * DETALHAMENTO DOS ITENS DA ESPECIFICAÇÃO DA AÇÃO
 */

function relatDetalhamentoItensEspecificacaoAcaoPlanoTrabalho($width, $arDados, $bgColor){
	global $db;
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0"
		style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
			<table <?=$width; ?> class="lista" align="center" <?=$bgColor; ?>
				cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b><span style="text-align: center; font-size: 9px">DETALHAMENTO
					DOS ITENS DA ESPECIFICAÇÃO DA AÇÃO</span></b></td>
					<td class="fot">ANEXO<br>
					7</td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center; font-size: 9px;"><b>Utilize
					este formulário para as ações: MAT. DID. PEDAGÓGICO, EQUIPto,
					REFORMA E ADAPTAÇÃO DE ESCOLAS <br>
					Obs.: Utilize quantos formulários forem necessários para
					complementação deste anexo</b></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>1 - EXERCÍCIO</b></td>
					<td><b> 2 - AÇÃO A SER EXECUTADA</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$_SESSION['exercicio']; ?></td>
					<?
					$sql = "SELECT DISTINCT
				    ac.acanome
				FROM emenda.acao ac INNER JOIN emenda.programaacao pg
				    ON (ac.acaid = pg.acaid) INNER JOIN emenda.ptacao pta
				    ON (pg.pacid = pta.pacid) INNER JOIN emenda.planotrabalho pt
				    ON (pta.ptrid = pt.ptrid)
				WHERE
				    pt.ptrid = ".$arDados['ptrid']."
				    AND ac.acastatus = 'A' 
				    AND pg.pacstatus = 'A'
				    AND pt.ptrstatus = 'A'";

					$arPlano = $db->carregar($sql);
					?>
					<td><?
					if($arPlano){
						foreach ($arPlano as $value) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['acanome']."<br>";
		   	}
					}else{
						echo "&nbsp;";
					}
					?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>3 - CNPJ</b></td>
					<td><b>4 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnumcpfcnpj']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>5 - MUNICÍPIO</b></td>
					<td><b>6 - UF</b></td>
				</tr>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td><b>7 - ESPECIFICAÇÃO DA AÇÃO</b></td>
				</tr>
				<tr>
				<?
				$sql = "SELECT
					e.espnome,
				    ptik.ptkdescricao,
				    ptik.ptkunidademedida,
				    ptik.ptkquantidade,
				    ptik.ptkvalorunitario,
				    (ptik.ptkquantidade * ptik.ptkvalorunitario) as total
				FROM emenda.planotrabalho pt 
					INNER JOIN emenda.ptacao pta ON (pt.ptrid = pta.ptrid) 
					INNER JOIN emenda.ptacaoespecificacao ptae ON (pta.ptaid = ptae.ptaid) 
					INNER JOIN emenda.acaoespecificacao ae ON (ptae.aceid = ae.aceid) 
					INNER JOIN emenda.especificacao e ON (ae.espid = e.espid) 
					INNER JOIN emenda.ptitemkit ptik ON (ptae.pteid = ptik.pteid)
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE pt.ptrstatus = 'A'
					AND ptae.ptestatus = 'A'
				    AND ae.acestatus = 'A'
				    AND e.espstatus = 'A'
				    AND pt.ptrid = ".$arDados['ptrid']."
				ORDER BY ptik.ptkdescricao";

				$arRegistro = $db->carregar($sql);
				?>
					<td><?
					if($arRegistro){
						$espAnt = "";
						foreach ($arRegistro as $value) {
							if($espAnt != $value['espnome']){
								echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['espnome']."<br>";
								$espAnt = $value['espnome'];
							}
						}
					}else{
						echo "&nbsp;";
					}
					?></td>
				</tr>
			</table>
			<table <?=$width; ?> class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr <?=$bgColor; ?>>
					<td style="text-align: center;" colspan="5"><b>8 - DETALHAMENTO DOS
					ITENS QUE COMPÕEM A ESPECIFICAÇÃO</b></td>
				</tr>
				<tr <?=$bgColor; ?>>
					<td rowspan="2" width="35%" style="text-align: center;"><b>8.1
					IDENTIFICAÇÃO DOS ITENS</b></td>
					<td rowspan="2" width="15%" style="text-align: center;"><b>8.2
					UNIDADE DE MEDIDA</b></td>
					<td rowspan="2" width="15%" style="text-align: center;"><b>8.3
					QUANTIDADE</b></td>
					<td colspan="2" width="25%" style="text-align: center;"><b>8.4
					ESTIMATIVA DE CUSTO</b></td>
				</tr>
				<tr <?=$bgColor; ?>>
					<td style="text-align: center;"><b>8.5.1 VALOR UNITÁRIO</b></td>
					<td style="text-align: center;"><b>8.5.2 VALOR TOTAL</b></td>
				</tr>
				<?
				if($arRegistro){
					foreach ($arRegistro as $valor) {
						?>
				<tr>
					<td><?=$valor['ptkdescricao']; ?></td>
					<td><?=$valor['ptkunidademedida']; ?></td>
					<td><?=$valor['ptkquantidade']; ?></td>
					<td>R$ <?=number_format($valor['ptkvalorunitario'], 2, ',', '.'); ?></td>
					<td>R$ <?=number_format($valor['total'], 2, ',', '.'); ?></td>
				</tr>
				<?
				$totalAnexo += $valor['total'];
}
}
?>
				<tr>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
				</tr>
				<tr <?=$bgColor; ?>>
					<td colspan="4"><b>9 - TOTAL DESTE ANEXO</b></td>
					<td>R$ <?=number_format($totalAnexo, 2, ',', '.'); ?></td>
				</tr>
			</table>
			<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td>&nbsp;</td>
				</tr>
			</table>

			<? echo autenticacaoRelatPlanoTrabalho('10','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
	<?
}

function autenticacaoRelatPlanoTrabalho($num, $titulo, $arDados, $width, $bgColor){
	$arDados['ptrtipodirigente'] == 'D' ? $tipoDirigente = 'Dirigente' : $tipoDirigente = 'Representante Legal';
	?>
	<table <?=$width ?> class="lista1" align="center" cellspacing="1"
		cellpadding="4">
		<tr <?=$bgColor; ?>>
			<td colspan="2"><b><?=$num; ?> - <?=$titulo; ?></b></td>
		</tr>
		<tr <?=$bgColor; ?>>
			<td style="text-align: right;"><b>ASSINADO PELO:</b></td>
			<td style="text-align: left;"><b><? echo $tipoDirigente; ?></b></td>
		</tr>
	</table>
	<table <?=$width ?> class="lista1" align="center" cellspacing="1"
		cellpadding="4">
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr>
			<td style="text-align: center" colspan="2"><b>______________________________________________</b></td>
		</tr>
		<tr>
			<td style="text-align: center" colspan="2"><b>LOCALIDADE, UF E DATA</b></td>
		</tr>
		<tr>
			<td colspan="2"></td>
		</tr>
		<tr>
			<td style="text-align: center" valign="top"><b><span
				style="font-size: 10px;"><?=$arDados['entnumcpfcnpj']; ?> <br>
				<?=$arDados['dirigente']; ?></span></td>
			<td style="text-align: center;" valign="bottom"><b>______________________________________________</b></td>
		</tr>
		<tr>
			<td style="text-align: center;">&nbsp;</td>
			<td style="text-align: center;"><b>ASSINATURA <?=strtoupper($tipoDirigente); ?></b></td>
		</tr>
	</table>
	<table <?=$width; ?> class="lista" align="center" cellspacing="1"
		cellpadding="4">
		<tr>
			<td colspan="2"><b>Formulário confeccionado obedecendo aos preceitos
			da IN/STN/MF nº 01, de 15.1.1997 e as suas alterações.</b></td>
		</tr>
	</table>
	<?
}

function filtraEmendas($proid){
	global $db;

	$sql = "SELECT
				prem.preid AS codigo,
				eme.emecodigo AS descricao
			FROM
				emenda.emenda eme, emenda.programaemenda prem
			WHERE
				eme.emeid=prem.emeid and
				prem.prestatus='A' and
				prem.proid = $proid and
				emeexercicio = ".$_SESSION["exercicio"];

	echo $db->monta_combo( "emeid", $sql, 'S', 'Selecione...', '', '', '', '215', 'N');


}

function totalizaBeneficiariosPlanoTrabalho(){
	global $db;

	//Query do total de beneficiários

	$sql = "SELECT DISTINCT
			  sum(ptib.ptbquantidaderural) as rural,
			  sum(ptib.ptbquantidadeurbana) as urbana, 
			  sum(ptib.ptbquantidaderural + ptib.ptbquantidadeurbana) as total
			FROM
			  emenda.ptiniciativa pti INNER JOIN emenda.ptiniciativabeneficiario ptib 
			  ON (pti.ptiid = ptib.ptiid) RIGHT JOIN emenda.iniciativabeneficiario inb
			  ON (ptib.icbid = inb.icbid)
			WHERE
			  inb.icbstatus = 'A'
			  AND pti.ptrid = ".$_SESSION["emenda"]["ptrid"];

	$totalben = $db->carregar($sql);
	$totalben = $totalben ? $totalben : array();
	?>
	<table class='listagem' width='100%'>
	<?php	foreach( $totalben as $totalbenef ){
		echo "	<tr>
						<td align='center' width='60%'></td>
						<td align='center'><b>".$totalbenef['rural']."</b></td>
						<td align='center'><b>".$totalbenef['urbana']."</b></td>
						<td align='center'><b>".$totalbenef['total']."</b></td>
					<tr>";
	}
	?>
	</table>
	<?
}

function totalizaAcoesPlanoTrabalho($coluna){
	global $db;

	$sql = "SELECT
			  CASE WHEN sum(ptie.ptequantidade * ptie.ptevalorunitario) is null
			         THEN '-'
			         ELSE 'R$ ' || trim(to_char(sum(ptie.ptequantidade * ptie.ptevalorunitario), '999G999G999D99'))
			    END as valor_total,
			    CASE WHEN sum(ptie.ptevalorproponente) is null
			         THEN '-'
			         ELSE 'R$ ' || trim(to_char(sum(ptie.ptevalorproponente), '999G999G999D99'))
			    END as valor_proponente,
			    CASE WHEN (sum(ptie.ptequantidade * ptie.ptevalorunitario)-sum(ptie.ptevalorproponente)) is null
			         THEN '-'
			         ELSE 'R$ ' || trim(to_char((sum(ptie.ptequantidade * ptie.ptevalorunitario)-sum(ptie.ptevalorproponente)),'999G999G999D99'))
			    END as valor_convenente
			FROM
			  emenda.planotrabalho ptr INNER JOIN emenda.ptiniciativa pti
			  ON (ptr.ptrid = pti.ptrid) INNER JOIN emenda.ptiniciativaespecificacao ptie
			  ON (pti.ptiid = ptie.ptiid)
			WHERE
			  ptr.ptrstatus = 'A'
			  AND ptie.ptestatus = 'A'
			  AND pti.ptrid = ".$_SESSION["emenda"]["ptrid"]."
			GROUP BY
			    ptr.ptrid
			ORDER BY
			    ptr.ptrid";

	$valores = $db->pegalinha($sql);

	if($coluna=='total')
	return $valores['valor_total'];
	else if($coluna=='proponente')
	return $valores['valor_proponente'];
	else
	return $valores['valor_convenente'];

}

function retornaMes($mes){

	switch ($mes){
		case '01' :
			return 'Jan';
			break;
		case '02' :
			return 'Fev';
			break;
		case '03' :
			return 'Mar';
			break;
		case '04' :
			return 'Abr';
			break;
		case '05' :
			return 'Mai';
			break;
		case '06' :
			return 'Jun';
			break;
		case '07' :
			return 'Jul';
			break;
		case '08' :
			return 'Ago';
			break;
		case '09' :
			return 'Set';
			break;
		case '10' :
			return 'Out';
			break;
		case '11' :
			return 'Nov';
			break;
		case '12' :
			return 'Dez';
			break;
	}
}

function retornaMesExtenso($data){

	$arData = explode('-', $data);

	$mes = $arData[1];

	switch ($mes){
		case '01' :
			return 'Janeiro';
			break;
		case '02' :
			return 'Fevereiro';
			break;
		case '03' :
			return 'Março';
			break;
		case '04' :
			return 'Abril';
			break;
		case '05' :
			return 'Maio';
			break;
		case '06' :
			return 'Junho';
			break;
		case '07' :
			return 'Julho';
			break;
		case '08' :
			return 'Agosto';
			break;
		case '09' :
			return 'Setembro';
			break;
		case '10' :
			return 'Outubro';
			break;
		case '11' :
			return 'Novembro';
			break;
		case '12' :
			return 'Dezembro';
			break;
	}
}

function retiraPontos($valor){
	$valor = str_replace(".","", $valor);
	$valor = str_replace(",",".", $valor);

	return $valor;
}

function insereCronogramaExecucaoDesembolso($post){
	global $db;

	$ptrid = $_SESSION["emenda"]["ptrid"];

	$quantAno = $post['anoFim'] - $post['anoIni'];
	$n = substr($post['anoIni'], 0, strlen($post['anoIni']) - 2 );
	$n1 = substr($post['anoIni'], strlen($post['anoIni'])-2, strlen($post['anoIni']));

	for($y=0; $y<=$quantAno; $y++){

		$s = ($n1 + $y);
		if(strlen((String) $s) == 1){
			$s = '0'.$s;
		}
		$ano = $n . $s;

		for($i=1; $i<=12; $i++){

			if(strlen((String) $i) == 1){
				$i = '0'.$i;
			}

			$valorC = retiraPontos($post['ptcvalorC_'.$ano.'_'.$i]);
			$valorP = retiraPontos($post['ptcvalorP_'.$ano.'_'.$i]);

			$valorIdC = $post['ptcidC_'.$ano.'_'.$i];
			$valorIdP = $post['ptcidP_'.$ano.'_'.$i];

			if($valorIdC){
				if($valorC){
					$data = $ano . '-' . $i . '-' . '01';

					$registro = array("ptcid" => $valorIdC,
									  "ptrid" => $ptrid,
									  "ptcdata" => $data,
									  "ptcvalor" => $valorC
					);

					$tipo = 'C';
					alteraCronograma($registro, $tipo);
				}else{
					excluiCronograma($valorIdC);
				}
			}else{
				if($valorC){
					$data = $ano . '-' . $i . '-' . '01';

					$registro = array("ptrid" => $ptrid,
									  "ptcdata" => $data,
									  "ptcvalor" => $valorC
					);

					$tipo = 'C';
					insereCronograma($registro, $tipo);
				}
			}
			if($valorIdP){
				if($valorP){
					$data = $ano . '-' . $i . '-' . '01';

					$registro = array("ptcid" => $valorIdP,
									  "ptrid" => $ptrid,
									  "ptcdata" => $data,
									  "ptcvalor" => $valorP
					);

					$tipo = 'P';
					alteraCronograma($registro, $tipo);
				}else{
					excluiCronograma($valorIdP);
				}
			}else{
				if($valorP){
					$data = $ano . '-' . $i . '-' . '01';

					$registro = array("ptrid" => $ptrid,
									  "ptcdata" => $data,
									  "ptcvalor" => $valorP
					);

					$tipo = 'P';
					insereCronograma($registro, $tipo);
				}
			}
		}
	}
	echo $db->commit();
}
function insereCronograma($arDados = array(), $tipo){
	global $db;

	if($tipo == 'C'){
		$sql = "INSERT INTO
			  emenda.ptcronogramadesembolso(
			  ptrid,
			  ptctipo,
			  ptcdata,
			  ptcvalor
			) 
			VALUES (
			  ".$arDados['ptrid'].",
			  'C',
			  '".$arDados['ptcdata']."',
			  ".$arDados['ptcvalor']."
			)";

		$db->executar($sql);
		//echo $sql."<br>";
	}


	if($tipo == 'P'){

		$sql = "INSERT INTO
			  emenda.ptcronogramadesembolso(
			  ptrid,
			  ptctipo,
			  ptcdata,
			  ptcvalor
			) 
			VALUES (
			  ".$arDados['ptrid'].",
			  'P',
			  '".$arDados['ptcdata']."',
			  ".$arDados['ptcvalor']."
			)";

		$db->executar($sql);
		//echo $sql."<br>";
	}
	//echo $db->commit();
}

function alteraCronograma($arDados = array(), $tipo){
	global $db;

	if($tipo == 'C'){
		$sql = "UPDATE
				  emenda.ptcronogramadesembolso  
				SET 
				  ptrid = ".$arDados['ptrid'].",
				  ptctipo = '$tipo',
				  ptcdata = '".$arDados['ptcdata']."',
				  ptcvalor = ".$arDados['ptcvalor']."
				 
				WHERE 
				  ptcid = ".$arDados['ptcid'];

		$db->executar($sql);
		//echo $sql."<br>";
	}

	if($tipo == 'P'){

		$sql = "UPDATE
				  emenda.ptcronogramadesembolso  
				SET 
				  ptrid = ".$arDados['ptrid'].",
				  ptctipo = '$tipo',
				  ptcdata = '".$arDados['ptcdata']."',
				  ptcvalor = ".$arDados['ptcvalor']."
				 
				WHERE 
				  ptcid = ".$arDados['ptcid'];

		$db->executar($sql);
		//echo $sql."<br>";
	}
}

function excluiCronograma($ptcid){
	global $db;

	$sql = "DELETE FROM
			  emenda.ptcronogramadesembolso 
			WHERE 
			  ptcid = $ptcid";

	//echo $sql."<br>";
	$db->executar($sql);
}


function salvarEscola($post){
	//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
	$obPTA = new PTA();
	global $db;

	validaSessionPTA( $post['entid'] );
	validaSessionPTA( $post['ptrid'] );

	$resultado=0;
	if(!empty($post['esbid']) ){
		$sql = "UPDATE
				  emenda.ptescolasbeneficiadas  
				SET 
				  ptrid = $post[ptrid],
				  entid = $post[entid],
				  esbquantidadealunos = $post[esbquantidadealunos]
				WHERE 
				  esbid = $post[esbid]";
		$db->executar($sql);
		return $db->commit();
	}else{
		$sql = "SELECT	count(*)
				FROM	entidade.entidadedetalhe entd, emenda.ptescolasbeneficiadas esb
				WHERE 	entd.entid=esb.entid and entd.entcodent='".$post['inepID']."' and esb.ptrid=".$post['ptrid'];
		$quant = $db->pegaUm($sql);

		if($quant==0){

			$ptrid = $post['ptrid'];

			$sql = "INSERT INTO emenda.ptescolasbeneficiadas
						(ptrid,entid,esbquantidadealunos) 
						VALUES 
						($ptrid,$post[entid],$post[esbquantidadealunos])";
			$db->executar($sql);
			return $db->commit();
			//}
		} else {
			echo 1;
		}
	}
	echo "<script>window.opener.location = 'emenda.php?modulo=principal/reformulacaoPTA&acao=A&ptrid=".$post['ptrid']."'; </script>";
	/*$db->executar($sql);
	 $db->commit();*/
	//echo $resultado;
}

function removerEscola($esbid){
	global $db;

	$sql = "DELETE FROM emenda.ptescolasbeneficiadas
			WHERE esbid = $esbid";		
	$db->executar($sql);
	$db->commit();

}

function buscaEscola($entcodent){
	global $db;

	$sql = "SELECT
				ent.entid,
				ent.entnome,
				tpl.tpldesc
			FROM 
				entidade.entidade ent
				INNER JOIN entidade.funcaoentidade f 
			    	ON f.entid = ent.entid
				INNER JOIN entidade.endereco ende 
					ON ent.entid=ende.entid
				INNER JOIN territorios.municipio ter 
					ON ende.muncod=ter.muncod 
				INNER JOIN entidade.tipolocalizacao tpl
					ON ent.tplid=tpl.tplid
			WHERE 
				f.funid in (3,4,11,12,16,18,56)
				AND ent.entstatus='A'
				AND ent.entcodent='$entcodent'";

	$nome = $db->pegalinha($sql);

	if( !empty( $nome ) ){
		echo "	<input type='hidden' name='entid' id='entid' value='".$nome['entid']."'>
				<input type='text' size='100' readonly style='text-align:center' name='nmescola' id='nmescola' value='".$nome['entnome']."' onfocus='buscaEscola(document.formulario.inepID.value);'>
				<input type='hidden' name='hdn_zona' id='hdn_zona' value='".$nome['tpldesc']."'>";
	} else {
		echo 1;
	}
}

function filtraMunicipio($estuf){
	global $db;
	$sql = "SELECT
				ter.muncod AS codigo,
				ter.mundescricao AS descricao
			FROM
				territorios.municipio ter
			WHERE
				ter.estuf = '$estuf'
			ORDER BY ter.mundescricao";

	echo $db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '215', 'N','id="muncod"');

}

function pesquisaINEP($post){
	global $db;
	$where = ($_POST["entnome"]) ? " AND UPPER(ent.entnome) like UPPER('%".$_POST["entnome"]."%')" : "";
	$where.=($_POST["estcod"]) ? " AND ende.estuf = '".$_POST["estcod"]."'" : "";
	$where.=($_POST["muncod"]) ? " AND ende.muncod = '".$_POST["muncod"]."'" : "";

	if(!empty($where)){
		$sql = "SELECT	'<center>
								<a style=\"cursor:pointer;\" onclick=\"selecionaINEP(\''||entd.entcodent||'\');\">
									<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
								</a>
							</center>' as acao,
							entd.entcodent, ent.entnome, ende.estuf, ter.mundescricao
					FROM	entidade.entidade ent, entidade.entidade2 ent2, entidade.entidadedetalhe entd, entidade.endereco ende,territorios.municipio ter 
					WHERE 	ent.entid=ent2.entid AND 
							ent.entid=entd.entid AND
							ent.entid=ende.entid AND
							ende.muncod=ter.muncod AND
							ent.entstatus='A' AND 
							ent2.funid in(3,4,11,12,16,18,56)
							$where";

							$cabecalho 		= array("Ação", "INEP","Escola","UF","Município");
							$db->monta_lista( $sql, $cabecalho, 100, 10, 'N', 'center', '', '', '','');
	}
	echo 1;

}

/*
 * Manter Responsável
 */

/**
 * Função Pesquisa Responsavel
 * Metodo usado para pesquisar registro na base de dados
 * @param array $post - Dados do Formulário
 * @return void
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 04/09/2009
 */

function pesquisaResponsavelAjax($post){
	global $db;
	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

	$arPost = !empty($post) ? $post : array();

	extract($arPost);

	$filtro = array();

	if($unicod){
		$filtro[] = " r.unicod = '" . $unicod . "'";
	}
	if($resdsc){
		$filtro[] = " lower(r.resdsc) = lower('" . $resdsc . "')";
	}
	if($resassunto){
		$filtro[] = " lower(r.resassunto) = lower('" . $resassunto . "')";
	}

	if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
		$acao = "( '<center><a href=\"emenda.php?modulo=principal/cadastraResponsavel&acao=A&resid='|| r.resid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
	    			'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiResponsavel('|| r.resid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
	} else {
		$acao = "( '<center><a href=\"\"><img src=\"/imagens/alterar_01.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
	    			'<img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
	}

	$sql = "SELECT
			  ".$acao." as acao,
	    	  r.resdsc,
			  u.unidsc,
			  r.resassunto,
			  rescontrapartidapadrao
			FROM 
			  emenda.responsavel r INNER JOIN public.unidade u
			  ON (r.unicod = u.unicod)
			WHERE
			  u.unistatus = 'A'
			  AND r.resstatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : "" ) . "
			ORDER BY
				r.resid";

	$cabecalho = array("Ação", "Responsável","Unidade","Tema" , "Contrapartida Mínima Padrão");
	$db->monta_lista( $sql, $cabecalho, 100, 10, 'N', 'center', '', '', '','');

}

/**
 * Função Exclui Responsavel
 * Método usado para exclusão de um registro do banco
 * @param integer resid - Código do Responsável
 * @return void
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 04/09/2009
 */

function excluirResponsavelAjax($resid){
	global $db;

	$sql = "UPDATE
			  emenda.responsavel  
			SET 
			  resstatus = 'I'
			 
			WHERE 
			  resid = $resid";

	$db->executar($sql);
	echo $db->commit();
}

/*
 * Recursos do PTA
 */

function verificaResponsavel( $resid ){

	global $db;

	$sql = "SELECT resid FROM emenda.responsavel WHERE resid = {$resid}";
	$existe = $db->pegaUm( $sql );

	if ( !$existe ){
		print "<script>"
		. "		alert('O Responsável informado não existe!');"
		. "		history.back(-1);"
		. "</script>";

		die;

	}

}

function cabecalhoResponsavel( $resid ){

	global $db;

	$sql = "SELECT
				unidsc,
				resdsc,
				resassunto
			FROM
				emenda.responsavel r
			INNER JOIN
				public.unidade u ON r.unicod = u.unicod
			WHERE
				resid = {$resid}";

	$responsavel = $db->pegaLinha($sql);

	print "<table class='tabela' bgcolor='#f5f5f5' cellspacing='1' cellpadding='3' align='center'>"
	. "		<tr><td colspan='2' class='subtitulocentro'>Dados do Responsável</td></tr>"
	. "		<tr>"
	. "			<td class='subtitulodireita' width='180px'>Unidade Orçamentária:</td>"
	. "			<td>{$responsavel["unidsc"]}</td>"
	. "		</tr>"
	. "		<tr>"
	. "			<td class='subtitulodireita' width='180px'>Descrição:</td>"
	. "			<td>{$responsavel["resdsc"]}</td>"
	. "		</tr>"
	. "		<tr>"
	. "			<td class='subtitulodireita' width='180px'>Assunto:</td>"
	. "			<td>{$responsavel["resassunto"]}</td>"
	. "		</tr>"
	. "</table>";


}

function geraFiltroResponsavel( $dados ){

	$filtro .= !empty( $dados["exercicio"] ) ? " AND ef.prgano = '{$dados["exercicio"]}'" : "";
	$filtro .= !empty( $dados["autid"] ) 	 ? " AND ea.autid  = {$dados["autid"]}" 	: "";
	$filtro .= !empty( $dados["acacod"] ) 	 ? " AND ef.acacod = '{$dados["acacod"]}'" 	: "";
	$filtro .= !empty( $dados["funcod"] ) 	 ? " AND ef.funcod = '{$dados["funcod"]}'" 	: "";
	$filtro .= !empty( $dados["sfucod"] ) 	 ? " AND ef.sfucod = '{$dados["sfucod"]}'" 	: "";

	return $filtro;

}

function listaEmendasResponsavel( $filtros, $tipo = 'resp', $habilitado = true ){

	global $db;

	$cabecalho = array( "Ação", "Número", "Autor", "Funcional Prog", "Subtítulo");

	if ( $habilitado && possuiPermissao() ) {
		$botao = "<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluiVinculoEmendas(' || ee.emeid || ');\">";
	} else {
		$botao = "<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" >";
	}


	if ( $tipo == 'resp' ){

		$sql = "SELECT
				'<center>
				{$botao}
				 </center>' as acao,
				ee.emecod as numero,
				case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end as autor,
				ef.fupfuncionalprogramatica||' ' as funcional,
				ef.fupdsc as subtitulo
			FROM
				emenda.emenda ee
			INNER JOIN
				emenda.autor ea ON ea.autid = ee.autid
			INNER JOIN
				 emenda.v_funcionalprogramatica ef ON ef.acaid = ee.acaid
			WHERE
				ee.resid = {$_SESSION["emenda"]["resid"]} {$filtros}
			ORDER BY
				autor";

				$db->monta_lista( $sql, $cabecalho, 100000, 1, 'N', 'center', '');
					
	}else{

		$unicod = $db->pegaUm( "SELECT unicod FROM emenda.responsavel WHERE resid = {$_SESSION["emenda"]["resid"]}" );

		$sql = "SELECT
				'<center>
					<input type=\"checkbox\" id=\"emenda[' || ee.emeid || ']\" name=\"emenda[]\" value=\"' || ee.emeid || '\"/>
				 </center>' as acao,
				ee.emecod as numero,
				case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end as autor,
				ef.fupfuncionalprogramatica||' ' as funcional,
				ef.fupdsc as subtitulo
			FROM
				emenda.emenda ee
			INNER JOIN
				emenda.autor ea ON ea.autid = ee.autid
			INNER JOIN
				 emenda.v_funcionalprogramatica ef ON ef.acaid = ee.acaid
			WHERE
				ee.resid is null {$filtros} AND ef.unicod = '{$unicod}' AND ef.prgano = '{$_SESSION["exercicio"]}'
			ORDER BY
				autor";

		$db->monta_lista_simples( $sql, $cabecalho, 100000, 1, 'N', '100%');

	}

}

function vinculaEmendasResponsavel( $dados ){

	global $db;

	if ($dados["emenda"]){
		foreach( $dados["emenda"] as $chave=>$valor ){

			$sql = "UPDATE emenda.emenda SET resid = {$_SESSION["emenda"]["resid"]} WHERE emeid = {$valor}; ";
			$db->executar($sql);

		}

		$db->commit();
	}

	print "<script>"
	. "		alert('Operação realizada com sucesso');"
	. "		window.parent.opener.location.reload();"
	. "		self.close();"
	. "</script>";

}

function excluirVinculoResponsavel( $emeid ){

	global $db;

	$sql = "SELECT
				id.iedid
			FROM
				emenda.emenda ee
			INNER JOIN
				emenda.emendadetalhe ed ON ed.emeid = ee.emeid
			INNER JOIN
				emenda.iniciativaemendadetalhe id ON id.emdid = ed.emdid
			WHERE
				ee.emeid = {$emeid}";

	$existe = $db->pegaUm( $sql );

	if ( $existe ){

		print "<script>"
		. "		alert('Não foi possível excluir o responsável, esta emenda já possue uma iniciativa vinculada! ');"
		. "		window.location.href = '?modulo=principal/emendasResponsavel&acao=A';"
		. "</script>";

		die;

	}else{

		$sql = "UPDATE emenda.emenda SET resid = null WHERE emeid = {$emeid}";
		$db->executar( $sql );

		$db->commit();
		$db->sucesso("principal/emendasResponsavel");

	}

}

function montaComboIniciativaTipoEnsinoAjax($iniid, $tpeid){
	global $db;

	if($iniid){
		$sql = "SELECT
				  te.tpeid as codigo,
				  te.tpedsc as descricao
				FROM 
				  emenda.tipoensino te INNER JOIN emenda.iniciativatipoensino ite
				  ON (te.tpeid = ite.tpeid)
				WHERE
				  te.tpestatus = 'A'
				  AND ite.iniid = $iniid";

		$arSelect = $db->carregar($sql);

		//echo $db->monta_combo('tpeid',$sql,'S','Selecione...',"",'','','','S','tpeid', '', $tpeid, 'Tipo de Ensino');?>
	<select id="tpeid" class="CampoEstilo" style="width: auto;"
		title="Tipo de Ensino" name="tpeid">
		<option value="">Selecione...</option>
		<?
		/*<ul>
		 <li>Internet Explorer</li>
		 <li>Opera</li>
		 <li>Firefox</li>
		 <li>Safari</li>
		 </ul>*/
		if($arSelect){
			$tema = "";
			$count = sizeof($arSelect);
			foreach ($arSelect as $arSelect) {
				if($tpeid == $arSelect['codigo'] || $count == 1){
					$select = "selected=\"selected\"";
				}else{
					$select = "";
				}
				//echo ;
				/*if($tema != $arSelect['tema'] ){
				 echo "<option value=\"tema\" $select>".$arSelect['tema']."</option>";
				 $tema = $arSelect['tema'];
					}*/
				echo "<option value=\"".$arSelect['codigo']."\" $select>".$arSelect['descricao']."</option>";
			}
		}
			
		?>
	</select>
	<img border="0" title="Indica campo obrigatório."
		src="../imagens/obrig.gif" />
		<?
}else{
	echo $db->monta_combo('tpeid',array(),'N','Selecione...',"",'','','','S','tpeid', '', '', 'Tipo de Ensino');
}


}

//---- INÍCIO MANTER EMENDAS ----

/**
 * Verifica se o usuário possui perfil de autor
 *
 * @param string $tela
 * @return array
 * @author Fernando Araújo Bagno da Silva
 * @since 14/09/2009
 *
 */
function verificaAutor( $tela ){

	switch ( $tela ){
		case "cadastroEmendas":
			$acao = 'onclick="document.getElementById(\'formulario\').submit();"';
			break;
		case "cadastraDetalheEmenda":
			$acao = 'onclick="salvaDetalheEmenda();" ';
			break;

		default:
			break;

	}


	$perfilEmenda 	   			= pegaPerfil( $_SESSION["usucpf"] );
	$dados["habilitadoEmendas"] = $perfilEmenda == AUTOR_EMENDA ? 'N' : 'S';
	$dados["disabledEmendas"]   = $perfilEmenda == AUTOR_EMENDA ? 'disabled="disabled"' : $acao . ' style="cursor: pointer;" ';

	return $dados;

}

/**
 * Verifica se a emenda, o detalhe da emenda, e entidade do detalhe da emenda
 * informados por request, existem no banco
 *
 * @param integer $dado
 * @param string $tipo
 * @author Fernando Araújo Bagno da Silva
 * @since 10/09/2009
 *
 */
function verificaEmenda( $dado, $tipo = 'emenda' ){

	global $db;

	switch ( $tipo ) {
		case 'emenda':

			$sql = "SELECT emeid FROM emenda.emenda WHERE emeid = {$dado}";
			$existe = $db->pegaUm( $sql );

			$msg = 'A Emenda informada não existe!';

			break;
		case 'detalheEmenda':

			$sql = "SELECT emdid FROM emenda.emendadetalhe WHERE emdid = {$dado}";
			$existe = $db->pegaUm( $sql );

			$msg = 'O Detalhe da Emenda informado não existe!';

			break;
		case 'detalheEntidade':

			$sql = "SELECT edeid FROM emenda.emendadetalheentidade WHERE edeid = {$dado}";
			$existe = $db->pegaUm( $sql );

			$msg = 'A Entidade Detalhe da Emenda informada não existe!';

			break;

	}

	if ( !$existe ){
		print "<script>"
		. "		alert('{$msg}');"
		. "		history.back(-1);"
		. "</script>";
	}


}

/**
 * Busca a emenda atraves do detalhe informado por request
 *
 * @param integer $emdid
 * @author Fernando Araújo Bagno da Silva
 * @since 14/09/2009
 *
 */
function pegaEmendaPorDetalhe( $emdid ){

	global $db;

	$sql   = "SELECT emeid FROM emenda.emendadetalhe WHERE emdid = {$emdid}";
	$emeid = $db->pegaUm( $sql );

	$_SESSION["emenda"]["emeid"] = !empty($_SESSION["emenda"]["emeid"]) ? $_SESSION["emenda"]["emeid"] : $emeid;

}

/**
 * Gera os filtros de pesquisa da lista de emendas.
 *
 * @param array $dados
 * @return array
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function geraFiltroEmendas( $dados ){

	$arJoin = array();
	$filtro = array();

	if( !empty( $dados["enbcnpj"] ) ){
		$enbcnpj = str_replace(".", "", $dados["enbcnpj"]);
		$enbcnpj = str_replace("/", "", $enbcnpj);
		$enbcnpj = str_replace("-", "", $enbcnpj);
		!empty( $dados["enbcnpj"] ) ? $filtro[] = " ent.enbcnpj  = '{$enbcnpj}'"  : "";
	}

	!empty( $dados["unicod"] )   	  ? $filtro[] = " ef.unicod = '{$dados["unicod"]}'" : "";
	!empty( $dados["acacod"] ) 	 	  ? $filtro[] = " ef.acacod = '{$dados["acacod"]}'" : "";
	!empty( $dados["gndcod"] )   	  ? $filtro[] = " ed.gndcod = '{$dados["gndcod"]}'" : "";
	!empty( $dados["autid"] )    	  ? $filtro[] = " ee.autid  = '{$dados["autid"]}'"  : "";
	!empty( $dados["enbnome"] )       ? $filtro[] = " UPPER( removeacento(ent.enbnome) ) like '%".removeAcentos( str_to_upper( trim($dados["enbnome"]) ) )."%' "  : "";

	if( !empty($dados["tipo"]) ){
		if( $dados["tipo"] == 'extra' ){
			$filtro[] = " ee.emetipo  = 'X'";
			$tiposql = " LEFT";
		} else if( $dados["tipo"] == 'emenda' ){
			$filtro[] = " ee.emetipo  = 'E'";
			$tiposql = " INNER";
		}
	}

	$residPossiveis   = recuperaResponsaveis( $_SESSION["usucpf"] );
	$filtroTodosResid = !empty($residPossiveis) ? "(ee.resid in (" . implode( ", ", $residPossiveis ) . ") OR ee.resid is null)" : "";

	!empty( $dados["resid"] )  ? $filtro[] = " ee.resid  = '{$dados["resid"]}'"  : $filtroTodosResid;
	!empty( $dados["emecod"] ) ? $filtro[] = " ee.emecod ilike '%{$dados["emecod"]}%'" : "";
	!empty( $dados["etoid"] ) ? $filtro[] = " ee.etoid = {$dados["etoid"]}" : "";

	if (!empty($dados["responsavel"])){
		$dados["responsavel"] == 'S'  ? $filtro[] = " ee.resid is not null" : $filtro[] = " ee.resid is null";
	}

	if (!empty($dados["iniciativa"])){
		$dados["iniciativa"]  == 'S' ? $filtro[] = " ie.iedid is not null" : $filtro[] = " ie.iedid is null";
	}
	
	if (!empty($dados["emenda_relator"])){
		$dados["emenda_relator"]  == 'S' ? $filtro[] = " ee.emerelator = 'S' " : $filtro[] = " ee.emerelator = 'N' ";
	}
	
	if (!empty($dados["entidade"])){
		$dados["entidade"]    == 'S' ? $filtro[] = " ede.enbid is not null" : $filtro[] = " ede.enbid is null";
	}
	
	if (!empty($dados["emdimpositiva"])){
		if($dados["emdimpositiva"] == 'S'){
			$filtro[] = " ee.emecod in (select distinct
							                            e.emecod
							                        from
							                            emenda.emenda e
							                            inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
							                        where
							                            e.emeano = '{$_SESSION['exercicio']}'
							                            and ed.emdimpositiva = 6)";
		} elseif($dados["emdimpositiva"] == 'N'){
			$filtro[] = " ee.emecod not in (select distinct
						                            e.emecod
						                        from
						                            emenda.emenda e
						                            inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
						                        where
						                            e.emeano = '{$_SESSION['exercicio']}'
						                            and ed.emdimpositiva = 6)";
		}//? $filtro[] = " ede.enbid is not null" : $filtro[] = " ede.enbid is null";
	}

	!empty($dados["descentralizacao"]) ? $filtro[] = " ee.emedescentraliza = '{$dados["descentralizacao"]}'" : "";

	!empty($dados["liberado"]) ? $filtro[] = " ededisponivelpta = '{$dados["liberado"]}'" : "";

	if ( !empty($dados["pendencia"]) ){
		if( $dados['tipo'] == 'emenda' ){
			if ( $dados["pendencia"] == 'S' ){
				$filtro[] = " (/*ede.ededisponivelpta = 'N' AND*/ emenda.f_verificardisponibilidadepta(1, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ie WHERE ie.emdid = ed.emdid) > 0 THEN true ELSE false END ), 1, 1, ee.emedescentraliza) is not null)";
			}else if ( $dados["pendencia"] == 'N' ){
				$filtro[] = " (/*ede.ededisponivelpta = 'N' AND*/ emenda.f_verificardisponibilidadepta(1, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ie WHERE ie.emdid = ed.emdid) > 0 THEN true ELSE false END ), 1, 1, ee.emedescentraliza) is null)";
			}
		} else {
			if ( $dados["pendencia"] == 'S' ){
				$filtro[] = " (/*ede.ededisponivelpta = 'N' AND*/ emenda.f_verificardisponibilidadepta(ede.edeid, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ie WHERE ie.emdid = ed.emdid) > 0 THEN true ELSE false END ), ebc.ebccontrapartida, er.rescontrapartidapadrao, ee.emedescentraliza) is not null)";
			}else if ( $dados["pendencia"] == 'N' ){
				$filtro[] = " (/*ede.ededisponivelpta = 'N' AND*/ emenda.f_verificardisponibilidadepta(ede.edeid, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ie WHERE ie.emdid = ed.emdid) > 0 THEN true ELSE false END ), ebc.ebccontrapartida, er.rescontrapartidapadrao, ee.emedescentraliza) is null)";
			}
		}
	}

	if (!empty($dados["gndcod"]) || !empty($dados["pendencia"]) || !empty($dados["iniciativa"]) || !empty($dados["entidade"]) || !empty( $dados["enbnome"] ) || !empty( $dados["enbcnpj"] ) ){
		$arJoin[] = $tiposql . " JOIN emenda.emendadetalhe ed ON ed.emeid = ee.emeid ";

		if (!empty($dados["iniciativa"])){
			$arJoin[] = " LEFT JOIN emenda.iniciativaemendadetalhe ie ON ie.emdid = ed.emdid";
		}

		if (!empty($dados["entidade"]) || !empty( $dados["enbnome"] ) || !empty( $dados["enbcnpj"] ) || !empty($dados["pendencia"]) ){
			$arJoin[] = " LEFT JOIN emenda.emendadetalheentidade ede
									INNER JOIN emenda.entidadebeneficiada ent on ent.enbid = ede.enbid
									LEFT JOIN emenda.entbeneficiadacontrapartida ebc on ebc.enbid = ent.enbid
										AND ebc.ebcexercicio = '". $_SESSION["exercicio"] ."'
								ON ede.emdid = ed.emdid
								and ede.edestatus = 'A'";
		}
	}

	$arDados['filtro'] = $filtro;
	$arDados['join'] = $arJoin;

	return $arDados;

}

function verificaExisteEmendaDetalhePTA($emeid){
	global $db;

	$sql = "SELECT
				count(e.emeid)
			FROM
				emenda.emenda e
			    inner join emenda.emendadetalhe ed
			    	on ed.emeid = e.emeid
			    inner join emenda.emendadetalheentidade ede
			    	on ede.emdid = ed.emdid
			    inner join emenda.ptemendadetalheentidade pede
			    	on pede.edeid = ede.edeid
                inner join emenda.planotrabalho ptr
                	on ptr.ptrid = pede.ptrid
			WHERE
				e.emeid = $emeid
                AND ptr.ptrstatus = 'A'";

	if($db->pegaUm($sql) != 0){
		return true;
	} else {
		return false;
	}

}

/**
 * Lista as emendas
 *
 * @param array $filtros
 * @param integer $perfil
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function listaEmendas( $filtros, $perfil = '', $tipo, $funcao = null ){

	global $db;

	if($tipo == 'emenda'){
		$emetipo = " ee.emeano = {$_SESSION["exercicio"]} AND ee.emetipo = 'E' ";
		$acao = "<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"verificaIndicacoesSIOP(' || ee.emeid || ', \'E\', \'\');\">";
		$tiposql = "INNER";
	} else {
		$emetipo = " ee.emeano = {$_SESSION["exercicio"]} AND ee.emetipo = 'X' ";
		$tiposql = "LEFT";

		if(  in_array( GESTOR_EMENDAS, $perfil ) ||  in_array( SUPER_USUARIO, $perfil ) ){

			$img = "<img src=\"/imagens/excluir.gif\" border=0 disabled=\"disabled\" title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"ExcluirEmenda(' || ee.emeid || ');\">";
			$acao = "<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"alterarRecursos(' || ee.emeid || ');\">
			$img";
		} else {
			if(!possuiPermissao()) {
				$retorno = 'disabled=\"disabled\"';
				$img = "<img src=\"/imagens/excluir_01.gif\" border=0 disabled=\"disabled\" title=\"Excluir\" style=\"cursor:pointer;\";\">";
			} else {
				$img = "<img src=\"/imagens/excluir.gif\" border=0 disabled=\"disabled\" title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"ExcluirEmenda(' || ee.emeid || ');\">";
			}

			if( in_array( LIBERAR_SENHA, $perfil ) ){
				$acao = "<img src=\"/imagens/alterar_01.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\">
				$img";
			}else{
				$acao = "<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"alterarRecursos(' || ee.emeid || ');\">
				$img";
			}
		}
	}
	if( in_array( ADMINISTRADOR_INST, $perfil ) && !in_array( LIBERAR_SENHA, $perfil ) ){
		$resid 		 = recuperaResponsaveis( $_SESSION["usucpf"] );
		$filtroResid = count($resid) > 0 ? " AND (er.resid in (" . implode( ", ", $resid ) . ") OR er.resid is null)" : "";
	}

	if (  in_array( AUTOR_EMENDA, $perfil ) ){
		$filtroAutid = " AND ea.autid in (SELECT autid FROM emenda.usuarioresponsabilidade WHERE usucpf = '{$_SESSION["usucpf"]}' AND rpustatus = 'A')";
	}

	if( ( in_array( AUTOR_EMENDA, $perfil ) ||  in_array( SUPER_USUARIO, $perfil ) || in_array( LIBERAR_SENHA, $perfil ) ) && $tipo == 'emenda' ){
		$pendente = '<img src=\"/imagens/pendente.gif\" border=0 title=\"Pendências\" style=\"cursor:pointer;\">';
		$pendencia = "CASE WHEN emenda.f_verificardisponibilidadepta(1, er.resid, (CASE WHEN (select count(b.emdid) from emenda.emendadetalhe a
                                                                                            left join emenda.iniciativaemendadetalhe b
                                                                                                on b.emdid = a.emdid
                                                                                                and b.iedstatus = 'A'
                                                                                        where a.emeid = ee.emeid
                                                                                        group by
                                                                                            b.emdid
                                                                                        HAVING count(b.emdid) = 0) = 0 THEN false ELSE true END ), 1, 1, ee.emedescentraliza) is not null
                       THEN '$pendente'
                    ELSE '' END";

		/*$pendencia = "CASE WHEN emenda.f_verificardisponibilidadepta(1, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ied WHERE ied.emdid = ed.emdid) > 0 THEN true ELSE false END ), 1, 1) is not null
		 THEN '$pendente'
		 ELSE '' END";*/

	}
	/*
	 * desabilitarConteudo( \'Emenda' || ee.emeid || '\' );
	 * abreconteudo(\'emenda.php?modulo=principal/listaDeEmendas&acao=A&subAcao=gravarCarga&carga=' || ee.emeid || '&params=\' + params, \'Emenda' || ee.emeid || '\');
	 */

	if(  !in_array( ADMINISTRADOR_INST, $perfil ) && !in_array( SUPER_USUARIO, $perfil ) ){
		$filtroliberar = "AND ee.emelibera = 'S'";
	}

	$filtros['filtro'] = ( $filtros['filtro'] ? $filtros['filtro'] : array() );
	$filtros['join'] = ( $filtros['join'] ? $filtros['join'] : array() );

	$sql = "SELECT DISTINCT
				'<center>
					<img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" 
					align=\"absmiddle\" vspace=\"3\" id=\"imgEmenda' || ee.emeid || '\" name=\"+\" 
					onclick=\"carregaDadosEmendaDetalhe(this.id,' || ee.emeid || ', \'emendaDetalhe\');\"/>
				 </center>' as acao,
				'<center>
					".($pendencia ? "'||$pendencia||'" : '')."					
					$acao
				 </center>' as editar, 
				ee.emecod as numero,
				case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end as autor,
				pu.unicod||' - '||pu.unidsc as unidade,
				ef.fupfuncionalprogramatica as funcional,
				ef.fupdsc as subtitulo,
				CASE WHEN ee.resid is not null THEN er.resdsc ELSE 'Não Informado' END as responsavel,
				ee.emeano,
				--case when ee.etoid <> 4 then
					CASE WHEN emenda.f_verificardisponibilidadepta(1, er.resid, (CASE WHEN (select count(b.emdid) from emenda.emendadetalhe a
                                                                                            left join emenda.iniciativaemendadetalhe b
                                                                                                on b.emdid = a.emdid
                                                                                                and b.iedstatus = 'A'
                                                                                        where a.emeid = ee.emeid
                                                                                        group by
                                                                                            b.emdid
                                                                                        HAVING count(b.emdid) = 0) = 0 THEN false ELSE true END ), 1, 1, ee.emedescentraliza) is not null THEN 
					 			emenda.f_verificardisponibilidadepta(1, er.resid, (CASE WHEN (select count(b.emdid) from emenda.emendadetalhe a
                                                                                            left join emenda.iniciativaemendadetalhe b
                                                                                                on b.emdid = a.emdid
                                                                                                and b.iedstatus = 'A'
                                                                                        where a.emeid = ee.emeid
                                                                                        group by
                                                                                            b.emdid
                                                                                        HAVING count(b.emdid) = 0) = 0 THEN false ELSE true END ), 1, 1, ee.emedescentraliza) 
					 ELSE '<center>-</center>' END
				/*else '<center>-</center>' end*/ as pendencia,
					 et.etodescricao,
  					(case when (select count(emdid) from emenda.emendadetalhe where emeid = ee.emeid and emdimpositiva = 6) > 0 then 'Sim' else 'Não' end) as impositivo,
				'<tr style=\"display:none\" id=\"trEmenda_' || ee.emeid || '\">
					<td id=\"tdEmenda_' || ee.emeid || '\" colspan=\"7\" style=\"padding:0px;border: 5px red\"></td>
				</tr><script>$funcao</script>' as tr
				
			FROM
				emenda.emenda ee
			INNER JOIN
				emenda.autor ea ON ea.autid = ee.autid 
				$tiposql JOIN
				emenda.v_funcionalprogramatica ef ON ef.acaid = ee.acaid 
				$tiposql JOIN
				public.unidade pu ON pu.unicod = ef.unicod
			" . ( !empty($filtros['join']) ? implode("", $filtros['join']) : "" ) . "
			LEFT JOIN
				emenda.responsavel er ON er.resid = ee.resid
				left join emenda.emendatipoorigem et on et.etoid = ee.etoid and et.etostatus = 'A'
			WHERE 
			$emetipo " . ( !empty($filtros['filtro']) ? " AND " . implode(" AND ", $filtros['filtro']) : "" ) . "
			{$filtroAutid}{$filtroResid}
			$filtroliberar
			ORDER BY autor, ee.emecod";
			
			$cabecalho = array( "", "Ação", "Código", "Autor", "UO", "Funcional Prg", "Subtítulo", "Responsável", "Ano", "Pendências", 'Origem', 'Impositivo' );
			$tamanho   = array( '', '5%', '', '', '', '', '', '', '', '10%');
			$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', $tamanho);
}

/**
 * Busca as iniciativas cadastradas na emenda
 *
 * @param integer $emdid
 * @author Fernando Araújo Bagno da Silva
 * @since 14/09/2009
 *
 */
function pegaIniciativasEmenda( $emdid ){

	global $db;

	$sql = "SELECT
				ei.iniid as codigo,
				ininome as descricao
			FROM 
				emenda.iniciativa ei
			INNER JOIN
				emenda.iniciativaemendadetalhe id ON ei.iniid = id.iniid
			WHERE
				id.emdid = {$emdid} AND iedstatus = 'A'
			ORDER BY
				ininome";

	$dados = $db->carregar( $sql );
	return $dados;

}

/**
 * Insere as iniciativas na emenda.
 *
 * @param array $dados
 * @author Fernando Araújo Bagno da Silva
 * @since 14/09/2009
 *
 */
function insereIniciativaEmendaDetalhe( $dados ){

	global $db;

	$sql = "UPDATE emenda.iniciativaemendadetalhe SET iedstatus = 'I'
			WHERE emdid = {$dados["emdid"]}";

	$db->executar( $sql );

	foreach( $dados["iniid"] as $valor ){

		if ( $valor ){

			$sql = "SELECT iedid FROM emenda.iniciativaemendadetalhe
					WHERE iniid = {$valor} AND emdid = {$dados["emdid"]}";

			$iedid = $db->pegaUm( $sql );

			if ( $iedid ){
					
				$sql = "UPDATE emenda.iniciativaemendadetalhe SET iedstatus = 'A'
						WHERE iedid = {$iedid}";
				$db->executar( $sql );

			}else{

				$sql = "INSERT INTO emenda.iniciativaemendadetalhe(emdid, iniid, iedstatus)
						VALUES ({$dados["emdid"]}, {$valor}, 'A')";
				$db->executar( $sql );

			}

		}

	}

	$db->commit();
	$db->sucesso( "principal/cadastraDetalheEmenda" );

}

/**
 * Lista os detalhes das emendas
 *
 * @param integer $emdid
 * @param string $tipo
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function listaEmendasDetalhe( $emeid, $tipo = 'lista' ){

	global $db;
	/*
	 * carregaDadosEmendaDetalhe(this.id,' || ee.emeid || ', \'emendaDetalhe\');

		'<tr style=\"display:none\" id=\"trEmenda_' || ee.emeid || '\">
		<td id=\"tdEmenda_' || ee.emeid || '\" colspan=\"7\" style=\"padding:0px;border: 5px red\"></td>
		</tr>' as tr

		<tr>
		<td style=\"padding:0px;margin:0;\"></td>
		<td id=\"td' || ed.emdid || '\" colspan=\"5\" style=\"padding:0px;display:none;border: 5px red\"></td>
		<td style=\"padding:0px;margin:0;\"></td>
		</tr>' as tr
		*/

	if ( $tipo == 'lista' ){

		$mais = "'<center> <img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\"
							border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"imgEmendaEntidade' || ed.emdid || '\" name=\"+\" 
								onclick=\"carregaDadosEmendaDetalhe(this.id,' || ed.emdid || ', \'emendaDetalheEntidade\');\"/></center>' as acao,";
		$tr_mais = ", '<tr style=\"display:none\" id=\"trEmendaEntidade_' || ed.emdid || '\">
							<td id=\"tdEmendaEntidade_' || ed.emdid || '\" colspan=\"7\" style=\"padding:0px;border: 5px red\"></td>
						</tr><script>{$_SESSION['emenda']['funcaoCarregaEntidade']}</script>' as tr";
		$cabecalho = array( "", "Ação", "GND", "Mod", "Fonte", "Valor (R$)" );
		$tamanho = '100%';

	}else{

		$cabecalho = array( "Ação", "GND", "Mod", "Fonte", "Valor (R$)" );
		$tamanho = '95%';

	}
	
	if( $_SESSION['exercicio'] < 2015 ){
		$imgImpositivo = "<img border=\"0\" title=\"Impositivos\" src=\"../imagens/valida5.gif\" style=\"cursor: pointer\" onclick=\"abreEmpedimentoEmenda('||ed.emdid||', \''||ed.emdvalor||'\', \'\', \'emenda\');\" alt=\"Ir\"/>";
	}

	$sql = "SELECT
	{$mais}
				'<center>
					<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"verificaIndicacoesSIOP( ' || ed.emeid || ', \'D\', ' || ed.emdid || ' );\">
					$imgImpositivo					
				 </center>' as editar,
				ed.gndcod||' - '||gn.gnddsc as gnd, 
			    ed.mapcod||' - '||map.mapdsc as mod,
			    ed.foncod||' - '||fon.fondsc as fonte,
				ed.emdvalor as valor
				{$tr_mais}
			FROM
				emenda.emendadetalhe ed
				left join public.gnd gn
			        on gn.gndcod = ed.gndcod
	                and gn.gndstatus = 'A'
			    left join public.modalidadeaplicacao map
			        on map.mapcod = ed.mapcod
			    left join public.fonterecurso fon
			        on fon.foncod = ed.foncod 
	                and fon.fonstatus = 'A'
			WHERE
				ed.emeid = {$emeid}";

				$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', $tamanho);

}

/**
 * Lista as entidade com detalhe das emendas
 *
 * @param integer $emdid
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function listaDetalheEntidade( $emdid ){

	global $db;

	$perfil = pegaPerfil( $_SESSION["usucpf"] );

	$sql = "(
		 		SELECT DISTINCT
		 			'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"abreDetalheEmenda(' || de.emdid || ');\"></center>' as acao,
		 			ee.enbcnpj,
					(select ug.ungcod from public.unidadegestora ug where ug.ungcnpj = ee.enbcnpj limit 1) as gestora,
					ee.enbnome as nome,
					ee.estuf as uf,
					de.edevalor as valor
				FROM
					emenda.emendadetalheentidade de
				INNER JOIN
					emenda.entidadebeneficiada ee ON ee.enbid = de.enbid
				WHERE
					emdid = {$emdid} AND edestatus = 'A'
				ORDER BY
					ee.enbnome 
			)
			
			UNION ALL 
			
			(
				SELECT DISTINCT
					'',
					'',
					'',
					'',
					'<p align=\"right\"><b>Valor Restante</b> <i>(a distribuir)</i></p>',
					ed.emdvalor - sum(de.edevalor) as valor
				FROM 
					emenda.emendadetalheentidade de
				INNER JOIN
					emenda.emendadetalhe ed ON ed.emdid = de.emdid
				WHERE
					de.emdid = {$emdid} AND edestatus = 'A'
				GROUP BY
					ed.emdvalor
					
			)";

	$cabecalho = array( $nomeAcao, "CNPJ", 'UG', "Entidade Beneficiada", "UF", "Valor (R$)" );
	$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');

}

/**
 * Busca ps dadps da ememda
 *
 * @param integer $emdid
 * @return array
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function pegaDadosEmenda( $emeid ){

	global $db;

	verificaEmenda( $emeid );

	$sql = "SELECT
				emecod as numero,
				CASE WHEN ee.autid is not null THEN 
					case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end
				ELSE 'Não Informado' END as autor,
				pu.unidsc as unidade,
				ef.fupfuncionalprogramatica as funcional,
				ef.fupdsc as subtitulo,
				ee.resid as responsavel
			FROM
				emenda.emenda ee
			LEFT JOIN
				emenda.autor ea ON ea.autid = ee.autid
			INNER JOIN
				emenda.v_funcionalprogramatica ef ON ef.acaid = ee.acaid
			INNER JOIN
				public.unidade pu ON pu.unicod = ef.unicod
			WHERE
				emeid = {$emeid}";

	return $db->pegaLinha( $sql );

}

/**
 * Atualiza a emenda
 *
 * @param integer $emdid
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function atualizaEmenda( $dados ){

	global $db;

	$resp = recuperaResponsaveis( $_SESSION["usucpf"] );

	if ( $resp && !empty($dados["resid"]) ){
		if( in_array( $dados["resid"], $resp ) ){

			$sql = "UPDATE emenda.emenda SET resid = {$dados["resid"]}
					WHERE emeid = {$dados["emeid"]}";

			$db->executar( $sql );
			$db->commit();
			$db->sucesso( "principal/cadastroEmendas" );
		}else{

			print "<script>"
			. "		alert('Você não possui permissão neste responsável!');"
			. "		history.back(-1);"
			. "</script>";

			die;

		}
	}else{

		$dados["resid"] = !empty($dados["resid"]) ? $dados["resid"] : 'null';

		$sql = "UPDATE emenda.emenda SET resid = {$dados["resid"]}
				WHERE emeid = {$dados["emeid"]}";
			
		$db->executar( $sql );
		$db->commit();
		$db->sucesso( "principal/cadastroEmendas" );
			
	}

}

/**
 * Busca os dados dos detalhes da emenda
 *
 * @param integer $emdid
 * @return array
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function pegaDadosDetalheEmenda( $emdid, $boCampo = true ){

	global $db;
	$campos = '';
	if( $boCampo ){
		$campos = "ed.gndcod||' - '||gn.gnddsc as gnd1, 
			    ed.mapcod||' - '||map.mapdsc as mod,
			    ed.foncod||' - '||fon.fondsc as fonte,
				ed.emdvalor as valor,";
	}

	$sql = "SELECT
				emecod as numero,
				ee.autid,
				CASE WHEN ee.autid is not null THEN 
					case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end
				ELSE 'Não Informado' END as autor,	
				ea.autnome,
				pu.unidsc as unidade,
				ef.fupfuncionalprogramatica as funcional,
				ef.fupdsc as subtitulo,
				er.resid as resid,
				CASE WHEN ee.resid is not null THEN er.resdsc ELSE 'Não informado' END as responsavel,
				ed.gndcod as gnd,
				/*ed.foncod as fonte,*/
				ed.mapcod as mapcod,
				ed.emdliberacaosri,
				$campos
				ee.emevalor,
				ee.emeid,
				ee.emetipo,
				ee.etoid,
				ef.acacod
			FROM
				emenda.emenda ee
			INNER JOIN
				emenda.emendadetalhe ed ON ed.emeid = ee.emeid
			LEFT JOIN
				emenda.autor ea ON ea.autid = ee.autid
			LEFT JOIN
				emenda.responsavel er ON er.resid = ee.resid
			LEFT JOIN
				emenda.v_funcionalprogramatica ef ON ef.acaid = ee.acaid
			LEFT JOIN
				public.unidade pu ON pu.unicod = ef.unicod
			left join public.gnd gn
		        on gn.gndcod = ed.gndcod
                and gn.gndstatus = 'A'
		    left join public.modalidadeaplicacao map
		        on map.mapcod = ed.mapcod
		    left join public.fonterecurso fon
		        on fon.foncod = ed.foncod 
                and fon.fonstatus = 'A' 
			WHERE
				ed.emdid = {$emdid}";

	return $db->pegaLinha( $sql );

}

/**
 * Lista as entidade já detalhadas na emenda
 *
 * @param integer $emdid
 * @return mixed
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function listaEntidadesDetalhe( $emdid, $habilita ){

	global $db;

	$sql = "SELECT DISTINCT
			    ed.edeid as id,
			    ee.enbcnpj as cnpj,
			    (select ug.ungcod from public.unidadegestora ug where ug.ungcnpj = ee.enbcnpj limit 1) as gestora,
			    ee.enbnome as nome,
			    ee.estuf as uf,
			    ed.edevalor as valor,
			    coalesce(ed.edevalorreducao, 0) edevalorreducao,
			    case when ed.edevalorreducao is null then 0 else (ed.edevalor - coalesce(ed.edevalorreducao, 0)) end as reduzido,
			    ed.usucpfalteracao,
			    to_char(ed.ededataalteracao, 'DD/MM/YYYY') as ededataalteracao,
			    usu.usunome,
			    usu2.usufuncao,
				ed.edecpfresp,
				ed.edenomerep,
				ed.edemailresp,
				ed.edetelresp,
				ed.ededddresp,
				ed.ededisponivelpta,
				(SELECT count(pa.ptrid) FROM emenda.ptemendadetalheentidade pe
					INNER JOIN emenda.planotrabalho pa ON pa.ptrid = pe.ptrid
				 WHERE pa.ptrstatus = 'A' AND edeid = ed.edeid ) as existepta
			FROM
			    emenda.emendadetalheentidade ed
				inner join emenda.entidadebeneficiada ee ON ee.enbid = ed.enbid
				left join seguranca.usuario usu on usu.usucpf = ed.usucpfalteracao
            	left join seguranca.usuario usu2 on usu2.usucpf = ed.edecpfresp --and usu.usustatus = 'A'
			WHERE
			    ed.emdid = {$emdid} AND
			    ed.edestatus = 'A'
			ORDER BY
			    nome asc";
	
	$dados = $db->carregar( $sql );
	$dados = ($dados ? $dados : array() );

	$perfilEmenda = pegaPerfilArray($_SESSION['usucpf']);
	
	$origemEmenda = $db->pegaUm("select etoid from emenda.emenda e
                		inner join emenda.emendadetalhe ed on ed.emeid = e.emeid where ed.emdid = $emdid");
	
	if( empty($origemEmenda) && $_SESSION['exercicio'] < '2014' ) $origemEmenda = 1;
	
	$arCPF = array('', '');

	for ( $i = 0; $i < count($dados); $i++ ){
		

		if( possuiPermissao() && ( in_array( SUPER_USUARIO, $perfilEmenda ) || in_array( AUTOR_EMENDA, $perfilEmenda ) || in_array( ADMINISTRADOR_INST, $perfilEmenda ) || in_array( GESTOR_EMENDAS, $perfilEmenda ) || in_array( LIBERAR_SENHA, $perfilEmenda ) ) && $habilita == 'S' ) {
			if ( (int)$dados[$i]["existepta"] < 1 ){ 
				if( in_array(AUTOR_EMENDA, $perfilEmenda) || in_array( ADMINISTRADOR_INST, $perfilEmenda ) || in_array( SUPER_USUARIO, $perfilEmenda )){
					//if( $dados[$i]["ededisponivelpta"] == 'N' ){
						$acao  = '';
						if( (int)$origemEmenda == 1 ) $acao  = "<img src='/imagens/alteracao.gif' title='Vincular Ações do PAR' style='cursor: pointer;' onclick='vinculaSubAcoes({$dados[$i]["id"]});'>&nbsp;";
						$acao .= "<img src='/imagens/alterar.gif' title='Alterar' style='cursor: pointer;' onclick='alterarEntidadeDetalhe({$dados[$i]["id"]})'>&nbsp;";
						if( $_SESSION['exercicio'] < 2015 ) $acao .= "<img src='/imagens/excluir.gif' title='Excluir' style='cursor: pointer;' onclick='excluirEntidadeDetalhe({$dados[$i]["id"]})'>";
					/*} else {
						$acao  = '';
						if( (int)$origemEmenda == 1 ) $acao  = "<img src='/imagens/alteracao.gif' title='Vincular Ações do PAR' style='cursor: pointer;' onclick='vinculaSubAcoes({$dados[$i]["id"]});'>&nbsp;";
						$acao .= "<img src='/imagens/alterar_01.gif' style='cursor: pointer;' onclick=''>&nbsp;";
						$acao .= "<img src='/imagens/excluir_01.gif' style='cursor: pointer;' onclick=''>";
					}*/
				}else{
					$acao  = '';
					if( (int)$origemEmenda == 1 ) $acao  = "<img src='/imagens/alteracao.gif' title='Vincular Ações do PAR' style='cursor: pointer;' onclick='vinculaSubAcoes({$dados[$i]["id"]});'>&nbsp;";
					$acao .= "<img src='/imagens/alterar.gif' title='Alterar' style='cursor: pointer;' onclick='alterarEntidadeDetalhe({$dados[$i]["id"]})'>&nbsp;";
					
					if( in_array($_SESSION['usucpf'], $arCPF) ){
						if( $_SESSION['exercicio'] < 2015 ) $acao .= "<img src='/imagens/excluir.gif' title='Excluir' style='cursor: pointer;' onclick='excluirEntidadeDetalhe({$dados[$i]["id"]})'>";
					} else {
						$acao .= "<img src='/imagens/excluir_01.gif' title='Excluir' style='cursor: pointer;'>";
					}
				}
			} else {
				$acao = '';
				if( in_array(AUTOR_EMENDA, $perfilEmenda) || in_array( ADMINISTRADOR_INST, $perfilEmenda ) || in_array( SUPER_USUARIO, $perfilEmenda )){ 
					if( (int)$origemEmenda == 1 ) $acao  = "<img src='/imagens/alteracao.gif' title='Vincular Ações do PAR' style='cursor: pointer;' onclick='vinculaSubAcoes({$dados[$i]["id"]});'>&nbsp;";
				}
				$acao .= "<img src='/imagens/alterar_01.gif' style='cursor: pointer;' onclick=''>&nbsp;";
				$acao .= "<img src='/imagens/excluir_01.gif' style='cursor: pointer;' onclick=''>";
			}
		}else{
			if( possuiPermissao() && ( in_array( SUPER_USUARIO, $perfilEmenda ) || in_array( ADMINISTRADOR_INST, $perfilEmenda ) || in_array( GESTOR_EMENDAS, $perfilEmenda ) || in_array( AUTOR_EMENDA, $perfilEmenda ) ) ){
				$acao  = '';
				
				if( (int)$origemEmenda == 1 ) $acao  = "<img src='/imagens/alteracao.gif' title='Vincular Ações do PAR' style='cursor: pointer;' onclick='vinculaSubAcoes({$dados[$i]["id"]});'>&nbsp;";
				if( in_array( SUPER_USUARIO, $perfilEmenda ) ){
					$acao .= "<img src='/imagens/alterar.gif' title='Alterar' style='cursor: pointer;' onclick='alterarEntidadeDetalhe({$dados[$i]["id"]})'>&nbsp;";
					if( in_array($_SESSION['usucpf'], $arCPF) ){
						if( $_SESSION['exercicio'] < 2015 )  $acao .= "<img src='/imagens/excluir.gif' title='Excluir' style='cursor: pointer;' onclick='excluirEntidadeDetalhe({$dados[$i]["id"]})'>";
					} else {
						$acao .= "<img src='/imagens/excluir_01.gif' title='Excluir' style='cursor: pointer;'>";
					}
				} else { 
					$acao .= "<img src='/imagens/alterar_01.gif'>&nbsp;";
					$acao .= "<img src='/imagens/excluir_01.gif'>";
				}
			} else { 
				$acao  = "<img src='/imagens/alterar_01.gif'>&nbsp;";
				$acao .= "<img src='/imagens/excluir_01.gif'>";
			}
		}
		
		if( possuiPermissao() && ( in_array( SUPER_USUARIO, $perfilEmenda ) || in_array( ADMINISTRADOR_INST, $perfilEmenda ) || in_array( GESTOR_EMENDAS, $perfilEmenda ) || in_array( AUTOR_EMENDA, $perfilEmenda ) ) ){
			//$acao .= "&nbsp;<img src='/imagens/editar_nome.gif' title='Redução com base em 1,2% da RCL de 2013' style='cursor: pointer;' onclick='mostraRecurso({$dados[$i]["id"]});'>&nbsp;";
		}
		
		if( $_SESSION['exercicio'] > 2014 ){
			$acao .= "&nbsp;<img border=\"0\" title=\"Impositivos\" src=\"../imagens/valida5.gif\" style=\"cursor: pointer\" onclick=\"abreEmpedimentoEmenda('".$emdid."', '".$dados[$i]["valor"]."', '".$dados[$i]["id"]."', 'emenda');\" alt=\"Ir\"/>";
		}

		$sql = "SELECT
					ini.ininome
				FROM
				    emenda.iniciativadetalheentidade iede
	            	inner join emenda.iniciativa ini	
	                	on iede.iniid = ini.iniid
	                    and iede.idestatus = 'A'           
				WHERE
				    iede.edeid = ".$dados[$i]["id"]." 
				ORDER BY
				    ininome asc";

		$arIninome = $db->carregarColuna( $sql );

		$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";

		$totalbeneficiado += $dados[$i]["valor"];
		$dados[$i]["valor"] = !empty($dados[$i]["valor"]) ?  number_format($dados[$i]["valor"] , 2, ',', '.') : '&nbsp;';
		$dados[$i]["edevalorreducao"] = number_format($dados[$i]["edevalorreducao"] , 2, ',', '.');
		$dados[$i]["reduzido"] = number_format($dados[$i]["reduzido"] , 2, ',', '.');

		if( $dados[$i]['usunome'] ){
			$usuario = $dados[$i]['usunome'];
			$align = 'justify';
		} else {
			$usuario = formatar_cpf( $dados[$i]["usucpfalteracao"] );
			$align = 'center';
		}

		if( $dados[$i]["edecpfresp"] || $dados[$i]["edenomerep"] || $dados[$i]["edetelresp"] || $dados[$i]["edemailresp"]){
			$html = "<td style=\"text-align: center;\" width=\"15%\">".formatar_cpf( $dados[$i]["edecpfresp"] )."</td>
					<td style=\"text-align: center;\" width=\"35%\">".$dados[$i]["edenomerep"]."</td>
					<td style=\"text-align: center;\" width=\"15%\">(".$dados[$i]["ededddresp"].") ".$dados[$i]["edetelresp"]."</td>
					<td style=\"text-align: center;\" width=\"15%\">".$dados[$i]["usufuncao"]."</td>
					<td style=\"text-align: center;\" width=\"30%\">".$dados[$i]["edemailresp"]."</td>";
		} else {
			$html = "<td style=\"text-align: left; color: red;\" colspan=\"4\">Os Dados do responsável da instituição não foram informados!</td>";
		}
		
		$boEdeid = '';
		if( $_SESSION['usucpf'] == '' ) $boEdeid = '<td>'.$dados[$i]["id"].'</td>';

		print "<tr bgcolor=".$cor." onmouseout=\"this.bgColor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\">"
		. "	<td align='center'><img style=\"cursor:pointer\" id=\"img_dimensao_{$dados[$i]["id"]}\" src=\"/imagens/mais.gif\" onclick=\"carregaDetalheEmendaEntidade(this.id, '{$dados[$i]["id"]}')\"> "
				.formatar_cnpj($dados[$i]["cnpj"])."</td>".$boEdeid."
			<td>".$dados[$i]["nome"]."</td>
			<td align='center'>".$dados[$i]["gestora"]."</td>
			<td align='center'>".$dados[$i]["uf"]."</td>
			<td align='center'>".$dados[$i]["valor"]."</td>";
		if( $_SESSION['exercicio'] >= '2014' ){
			print "<td align='center'>".$dados[$i]["edevalorreducao"]."</td>
			<!--<td align='center'>".$dados[$i]["reduzido"]."</td>-->";
		}
		print "<td align='justify'>".implode(',<br>', $arIninome)."</td>
			<td align='$align'>".$usuario."</td>
			<td align='center'>".$dados[$i]["ededataalteracao"]."</td>
			<td align='center'>".$acao."</td>
		</tr>
				<tr style=\"display:none\" id=\"listaDetalheEntidade_".$dados[$i]["id"]."\" bgcolor='{$cor}'>
		            <td id=\"trV_".$dados[$i]["id"]."\" colspan=10 >
						<table width=\"75%\">
							<tr>
								<td width=\"05%\"style=\"text-align: right;\"><img border=\"0\" src=\"../imagens/seta_filho.gif\"></td>".$html."
							</tr>
						</table>
					</td>
				</tr>";		
	}

	if( sizeof($dados) > 0 ){
		print('<tr >
				<td class="subtituloesquerda" colspan="12"><b>Total de Registros: '.sizeof($dados).'</b></td>
				<td/>
			   </tr>');
	}

	$sql = "SELECT emdvalor
			FROM emenda.emendadetalhe
			WHERE emdid = {$emdid}";

	$totalemenda = $db->pegaUm( $sql );

	$totaldistribuir = $totalemenda - $totalbeneficiado;

	$totalbeneficiado = number_format($totalbeneficiado , 2, ',', '.');
	$totalemenda = number_format($totalemenda , 2, ',', '.');
	$totaldistribuir = number_format($totaldistribuir , 2, ',', '.');

	print "<tr>"
	. "	<td colspan='4' align='right' bgcolor='#D0D0D0' width='20%'><b>Total da emenda</b></td>"
	. "	<td colspan='6' style='font-weight: bold;' width='80%'>"
	. "		R$ " . $totalemenda
	. "	</td>"
	. "</tr>"
	. "<tr>"
	. "	<td colspan='4' align='right' bgcolor='#D0D0D0'><b>(-) Já distribuído</b></td>"
	. "	<td colspan='6' style='font-weight: bold; color: red;'>"
	. "		R$ " . $totalbeneficiado
	. "	</td>"
	. "</tr>"
	. "<tr>"
	. "	<td colspan='4' align='right' bgcolor='#D0D0D0'><b>(=) Restante</b></td>"
	. "	<td colspan='6' style='font-weight: bold; color: blue;'>"
	. "		R$ " . $totaldistribuir
	. "	</td>"
	. "</tr>";

}

/**
 * Busca os dados detalhados de uma entidade e preenche os campos
 *
 * @param integer $edeid
 * @return array
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function buscaDadosEntidadeDetalhe( $edeid ){

	global $db;

	$sql = "SELECT
				ed.edeid as id,
				eb.enbid,
				eb.enbcnpj as cnpj,
				eb.enbnome as nome,
				eb.estuf as uf,
				ed.edevalor as valor,
				--ed.edeobjeto as objeto,
				ed.usucpfalteracao,
				usu.usunome,
				usu2.usufuncao,
				ed.ededataalteracao,
				ed.edecpfresp,
				ed.edenomerep,
				ed.edemailresp,
				ed.edetelresp,
				ed.ededddresp				
			FROM
				emenda.emendadetalheentidade ed
			INNER JOIN
				emenda.entidadebeneficiada eb ON eb.enbid = ed.enbid
			left join seguranca.usuario usu
            	on usu.usucpf = ed.usucpfalteracao
            left join seguranca.usuario usu2
            	on usu2.usucpf = ed.edecpfresp
                --and usu.usustatus = 'A'
			WHERE
				edeid = {$edeid}";

	$dados = $db->pegaLinha($sql);

	$sql = "SELECT
				ini.iniid as codigo,
				ini.ininome as descricao,
				iede.ideid
			FROM
			    emenda.iniciativadetalheentidade iede
            	inner join emenda.iniciativa ini	
                	on iede.iniid = ini.iniid
                    and iede.idestatus = 'A'           
			WHERE
			    iede.edeid = ".$edeid." 
			ORDER BY
			    ininome asc";

	$arIninome = $db->carregar( $sql );
	$iniciativa = '';
	$html.= '<select style="width: 250px;" class="CampoEstilo" onkeydown="javascript:combo_popup_remove_selecionados( event, \'edeobjeto\' );"
					ondblclick="javascript:combo_popup_abre_janela( \'edeobjeto\', 400, 400 );" 
					onclick="javascript:combo_popup_alterar_campo_busca( this );" id="edeobjeto" name="edeobjeto[]" size="5" multiple="multiple" 
					tipo="combo_popup" maximo="0">';
	if( !$arIninome ){
		$html.= '<option value="">Duplo clique para selecionar da lista</option>';
	} else {
		$ideid = '';
		foreach ($arIninome as $v) {
			$html.= '<option value="'.$v['codigo'].'">'.$v['descricao'].'</option>';
			$ideid.= '<input type="hidden" value="'.$v['ideid'].'" name="ideid['.$v['codigo'].']" id="ideid['.$v['codigo'].']"/>';
		}
	}

	$html.= '</select>';
	$dados["objeto"] = $html;
	$dados["ideid"] = ($ideid ? $ideid : '<input type="hidden" value="" name="ideid[]" id="ideid[]"/>');
	$dados["cnpj"]   = formatar_cnpj( $dados["cnpj"] );
	$dados["valor"]  = number_format( $dados["valor"] , 2, ',', '.' );
	$dados["nome"] 	 = iconv("ISO-8859-1", "UTF-8", $dados["nome"]);
	$dados["usucpfalteracao"]   = iconv("ISO-8859-1", "UTF-8", $dados['usunome']);
	$dados["ededataalteracao"]  = formata_data( $dados["ededataalteracao"] );
	$dados["edecpfresp"]   = formatar_cpf( $dados["edecpfresp"] );
	$dados["usufuncao"]   = utf8_encode( $dados["usufuncao"] );

	echo simec_json_encode($dados);

}

function validaEmendaVinculoPTA( $post ){
	$retIniciaitva = verificaEmendaDetalheExisteVinculoPTA( $post['emdid'], $post['enbid'] );
	$retIniciaitva = ( $retIniciaitva ? $retIniciaitva : array() );
	$iniciativa = '';
	foreach ($retIniciaitva as $v) {
		if( !in_array( $v['iniid'], $post['edeobjeto'] ) ){
			$iniciativa = ( $iniciativa == '' ? "\t*".$v['ininome']."\n" : $v['ininome']."\n\t*" );
		}
	}

	echo $iniciativa;
}

function validaEmendaEntidadeVinculoPTA( $post ){
	ob_clean();

	if( $post['boentidade'] != 'true' ){
		$retIniciaitva = verificaEmendaDetalheExisteVinculoPTA($post['emdid'], '', true );
	} else {
		$retIniciaitva = verificaEmendaDetalheExisteVinculoEntidade($post['emdid']);
	}
	$retIniciaitva = $retIniciaitva ? $retIniciaitva : array();

	$iniciativa = '';
	foreach ($retIniciaitva as $v) {
		if( !in_array( $v['iniid'], $post['iniid'] ) ){
			$iniciativa = ( $iniciativa == '' ? "\t*".$v['ininome']."\n" : $v['ininome']."\n\t*" );
		}
	}
	echo $iniciativa;
}

function liberaEntidadeDetalhe( $dados ){

	global $db;

	if ( $dados["liberado"] ){
		foreach( $dados["liberado"] as $chave=>$edeid ){
			$sql = "UPDATE emenda.emendadetalheentidade SET ededisponivelpta = 'N' WHERE  edeid = {$edeid}";
			$db->executar( $sql );
		}
	}

	if ( $dados["edeid"] ){
		foreach( $dados["edeid"] as $chave=>$edeid ){
			$sql = "UPDATE emenda.emendadetalheentidade SET ededisponivelpta = 'S' WHERE edeid = {$edeid}";
			$db->executar( $sql );
		}
	}

	$db->commit();
	$db->sucesso("principal/liberaEmenda");

}

function buscaEntidadePorCNPJ( $enbcnpj, $tipo = 'normal', $funid = '' ){

	global $db;

	// trata o campo CNPJ
	$enbcnpj = str_replace(".", "", $enbcnpj);
	$enbcnpj = str_replace("/", "", $enbcnpj);
	$enbcnpj = str_replace("-", "", $enbcnpj);

	if ( $tipo == 'popup' ){

		$sql = "SELECT
					e.enbid, 
					e.enbnome 
				FROM emenda.entidadebeneficiada e
				INNER JOIN entidade.funcaoentidade fe ON e.entid = fe.entid
				WHERE e.enbcnpj = '{$enbcnpj}' 
				AND e.enbstatus = 'A' ". (!empty($funid) ? "AND fe.funid = $funid " : '') ." 
				AND fe.funid IS NOT NULL 
				ORDER BY e.enbnome";

		$enbid = $db->carregar( $sql );

	}else if( $tipo == 'populabase' ){

		$sql = "SELECT
					e.enbid 
				FROM emenda.entidadebeneficiada e
				INNER JOIN entidade.funcaoentidade fe ON e.entid = fe.entid 
				WHERE e.enbcnpj = '{$enbcnpj}' 
				AND e.enbstatus = 'A'
				AND fe.funid IS NOT NULL";

		$enbid = $db->pegaUm( $sql );
		if( $enbid ) dadosEntidadeSelecionada($enbid);

	}else if ( $tipo == 'normal' ){

		$sql = "SELECT
					e.enbid 
				FROM emenda.entidadebeneficiada e
				INNER JOIN entidade.funcaoentidade fe ON e.entid = fe.entid 
				WHERE e.enbcnpj = '{$enbcnpj}' 
				AND e.enbstatus = 'A'
				AND fe.funid IS NOT NULL";

		$enbid = $db->carregarColuna( $sql );
			
	}

	return $enbid;

}

function buscarEntidadeBeneficiadaPorCNPJ($cnpj) {
	global $db;

	$cnpj = str_replace(array(".","/","-"), "", $cnpj);

	$sql = "SELECT entid, enbstatus, enbano, enbid, enbsituacaohabilita, enbdataalteracao,
       			   enbnome, enbcnpj, muncod, estuf
  			FROM emenda.entidadebeneficiada 
  			WHERE enbcnpj = '".$cnpj."' AND 
  				  enbano = '".$_SESSION["exercicio"]."' AND
  				  muncod IS NOT NULL AND
  				  enbstatus = 'A' and 
  				  muncod <> '0'";

	$enbentidade = $db->pegaLinha($sql);
	if( $enbentidade ) $enbentidade["enbnome"] = iconv("ISO-8859-1", "UTF-8", $enbentidade["enbnome"]);
	return $enbentidade;
}

function buscaEntidadePorCNPJBase( $dados ){
	global $db;

	// trata o campo CNPJ
	$enbcnpj = $dados['enbcnpj'];
	$enbcnpj = str_replace(".", "", $enbcnpj);
	$enbcnpj = str_replace("/", "", $enbcnpj);
	$enbcnpj = str_replace("-", "", $enbcnpj);

	$sql = "SELECT enbid, enbnome, enbcnpj as cnpj, estuf
			FROM emenda.entidadebeneficiada
			WHERE enbcnpj = '{$enbcnpj}' order by enbid limit 1 ";	

	$dadosBase = $db->pegaLinha( $sql );

	if($dados || $dadosBase){
		$dadosBase["cnpj"]   = formatar_cnpj( $enbcnpj );
		$dadosBase["enbnome"] = iconv("ISO-8859-1", "UTF-8", $dados["enbnome"]);
		$dadosBase["estuf"] = ($dadosBase['estuf'] ? $dadosBase['estuf'] : $dados['uf']);
		$dadosBase["enbid"] = ($dadosBase['enbid'] ? $dadosBase['enbid'] : '');
		echo simec_json_encode($dadosBase);
	} else {
		echo 'naoexiste';
	}

	die;

}

function dadosEntidadeSelecionada( $enbid ){

	global $db;

	$sql = "SELECT enbid, enbnome, enbcnpj as cnpj, estuf
			FROM emenda.entidadebeneficiada
			WHERE enbid = '{$enbid}'";

	$dados = $db->pegaLinha($sql);

	$dados["cnpj"]   = formatar_cnpj( $dados["cnpj"] );
	$dados["enbnome"] = iconv("ISO-8859-1", "UTF-8", $dados["enbnome"]);

	echo simec_json_encode($dados);

}

/**
 * Insere os detalhes de uma entidade
 *
 * @param array $dados
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function inserirEntidadeDetalhe ( $dados ){

	global $db;
	// trata o campo CNPJ
	$dados["enbcnpj"] = str_replace(array(".","/","-"), "", $dados["enbcnpj"]);
	
	$boEntidadeDetalhe = 0;
	if( empty($dados['edeid']) ){
		$boEntidadeDetalhe = $db->pegaUm("select count(v.edeid) from emenda.emendadetalheentidade v 
										where v.enbid in (select enbid from emenda.entidadebeneficiada where enbcnpj = '{$dados['enbcnpj']}')
											and v.edestatus = 'A' 
											and v.emdid = {$dados['emdid']}");
	}
	if( $boEntidadeDetalhe < 1 ){
		// trata o valor do detalhe
		$dados["edevalor"] = str_replace(".", "", $dados["edevalor"]);
		$dados["edevalor"] = str_replace(",", ".", $dados["edevalor"]);
		
		$dadosEmail = $dados;
	
		$dados["edecpfresp"] = str_replace(array(".","-"), "", $dados["edecpfresp"]);
		
		/*$boEntidade = true;
		if( $dados['acacod'] != '0048' ){
			if($dados['mapcod']  == '30'){
				$sql = "select count(iueid) as total from par.instrumentounidadeentidade where iuecnpj = '{$dados["enbcnpj"]}' and iutid in (1, 3)";
				$arrEntidade = $db->pegaLinha($sql);
				$boEntidade = ($arrEntidade['total'] > 0 ? true : false);
				$label = 'SECRETARIA DE EDUCAÇÃO';
			} elseif($dados['mapcod']  == '40'){
				$sql = "select count(iueid) as total from par.instrumentounidadeentidade where iuecnpj = '{$dados["enbcnpj"]}' and iutid in (2)";
				$arrEntidade = $db->pegaLinha($sql);
				$boEntidade = ($arrEntidade['total'] > 0 ? true : false);
				$label = 'PREFEITURA MUNICIPAL';
			}
		}
		
		if( !$boEntidade ){
			
			print "<script>"
			. "		alert('So é possível distribuir recurso para $label, pois o Modalidade é: {$dados['mod']}');"
			. "		history.back(-1);"
			. "</script>";
			die;
		}*/
	
		$enbid = $dados["enbid"];
	
		$exclusao = !empty($dados["edeid"]) ? " AND ee.edeid <> {$dados["edeid"]}" : "";
	
		// busca o total já beneficiado
		$sql = "SELECT sum(ee.edevalor) as totalbeneficiado
				FROM emenda.emendadetalheentidade ee
				INNER JOIN emenda.emendadetalhe ed ON  ed.emdid = ee.emdid
				WHERE ee.emdid = {$dados["emdid"]} AND ee.edestatus = 'A' {$exclusao}";
	
		$totalbeneficiado = $db->pegaUm( $sql );
	
		// busca o total da emenda
		$sql = "SELECT emdvalor
				FROM emenda.emendadetalhe
				WHERE emdid = {$dados["emdid"]}";
	
		$totalemenda = $db->pegaUm( $sql );
	
		if ( ($totalbeneficiado + $dados["edevalor"]) > $totalemenda ){
	
			// se o valor for maior do que o total da emenda, exibe erro.
			print "<script>"
			. "		alert('O valor distribuído não pode ultrapassar o valor total da emenda!');"
			. "		history.back(-1);"
			. "</script>";
			die;
	
		} else {
	
			if(!$enbid) $enbid = $db->pegaUm("SELECT enbid FROM emenda.entidadebeneficiada WHERE enbcnpj='".$dados['enbcnpj']."' and enbano = '".$_SESSION['exercicio']."'");
	
			if(!$enbid) {
	
				$sql = "INSERT INTO emenda.entidadebeneficiada(
	            												enbstatus, 
	            												enbano, 
	            												enbdataalteracao, 
	            												enbnome, 
	            												enbcnpj, 
	            												muncod, 
	            												estuf)
	    				VALUES ('A', 
	    						'".$_SESSION['exercicio']."', 
	    						NOW(), 
	    						'".$dados['enbnome']."', 
	    						'".$dados['enbcnpj']."', 
	    						'".(integer)$dados['muncod']."', 
	    						'".$dados['estuf']."') RETURNING enbid";
				//dbg($sql,1);
				$enbid = $db->pegaUm($sql);
	
			}else{
	
				$sql = "UPDATE emenda.entidadebeneficiada
	   					SET enbdataalteracao=NOW(), enbnome='".$dados['enbnome']."', muncod='".(integer)$dados['muncod']."', estuf='".$dados['estuf']."'
	 					WHERE enbid='".$enbid."'";
				//dbg($sql,1);
				$db->executar( $sql );
	
			}
	
			if( $dados['etoid'] == 1 ){#PAR
				$conteudo = '<p><b>Prezado(a),</b></p>
O Ministério da Educação informa que a '.$dados['enbnome'].' foi indicada como beneficiária da emenda '.$dados['emecod'].'/'.date('Y').'<br> 
do parlamentar '.$dados['autor'].'.  O aceite da proposta de emenda deverá ser realizado no módulo PAR/SIMEC, até 06/07/'.date('Y').'.
Posteriormente, é necessário o preenchimento dos dados da subação, alertando para o prazo de até 16/07/'.date('Y').'. para devolução ao FNDE.
Qualquer dúvida, tratar com o setor de Emendas do FNDE (2022-5922/5935/4430/5801).
<p>Favor não responder a este e-mail, o mesmo é gerado automaticamente.</p>';
			} elseif( $dados['etoid'] == 2 ){#DESCENTRALIZAÇÃO
				$conteudo = '<p><b>Prezado(a),</b></p>
A instituição '.$dados['enbnome'].' foi indicada como beneficiária da emenda '.$dados['emecod'].' do parlamentar '.$dados['autor'].'. <br>
A apresentação do respectivo termo de cooperação deverá ser realizada no módulo de Termos de Cooperação do SIMEC até 28/02/'.date('Y').'
<p>Favor não responder a este e-mail, o mesmo é gerado automaticamente.</p>';
			}elseif( $dados['etoid'] == 4 ){#UNIVERSIDADES E INSTITUTOS
				$conteudo = '<p><b>Prezado(a) Reitor(a),</b></p>
O Ministério da Educação informa que a emenda '.$dados['emecod'].', alocada nesta UO, encontra-se disponível para cadastramento da proposta <br>
simplificada da execução da emenda ou indicação da inviabilidade de execução até 24/03/'.date('Y').'.
<p>Favor não responder a este e-mail, o mesmo é gerado automaticamente.</p>';
			} else {
				$conteudo = '<p><b>Prezado(a),</b></p>
O Ministério da Educação informa que a '.$dados['enbnome'].' foi indicada como beneficiária da emenda '.$dados['emecod'].'/'.date('Y').' do parlamentar '.$dados['autor'].'.<br>
O prazo para aceite da emenda e cadastro do plano de trabalho será até 16/07/'.date('Y').' e deverá ser realizada no módulo EMENDAS/SIMEC.               
Qualquer dúvida, tratar com o setor de Emendas do FNDE (2022-4430/5801/4235/5990)
Para esclarecimento de dúvidas técnicas quanto aos itens de equipamento, tratar na SESU/MEC (2022-8169/8080).
<p>Favor não responder a este e-mail, o mesmo é gerado automaticamente.</p>';
			}
			
			$complementoConteudo = '<p><b>Prezado(a),</b></p>
									Para emenda '.$dados["emecod"].' foi indicado a seguinte entidade:<br><br>'.montaCorpoEmailEntidade($dadosEmail);
	
			$remetente = array('nome' => 'SIMEC - MÓDULO EMENDAS', 'email' => 'noreply@simec.gov.br');
			//$cc = "barbara.silva@fnde.gov.br";
			
			if ( !empty($dados["edeid"]) ){
	
				$atualizaEnbid = !empty($enbid) ? "enbid = {$enbid}, " : "";
	
				// caso esteja alterando um detalhe, realiza um update
				$sql = "UPDATE
							emenda.emendadetalheentidade
						SET 
							edevalor  = '{$dados["edevalor"]}', 
							$atualizaEnbid
							--edeobjeto = '{$dados["edeobjeto"]}',
							usucpfalteracao = '{$_SESSION['usucpf']}',
							ededataalteracao = 'now()',
							edecpfresp = '{$dados["edecpfresp"]}',
							edenomerep = '{$dados["edenomerep"]}',
							edemailresp = '{$dados["edemailresp"]}',
							ededddresp = '{$dados["ededddresp"]}',
							edetelresp = '{$dados["edetelresp"]}'
						WHERE 
							edeid = {$dados["edeid"]}";
	
							if($db->executar( $sql )){
	
								$sql = "SELECT edeemailenviado FROM emenda.emendadetalheentidade WHERE edeid = {$dados['edeid']}";
								$edeemailenviado = $db->pegaUm($sql);
	
								//if($edeemailenviado != 't' && $_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao" ){
								
								$retorno = enviar_email($remetente, array($dados["edemailresp"]), 'SIMEC - EMENDAS', $conteudo, $cc, null);
								if($_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao" ){
									$retornoPlano = enviar_email($remetente, array('planodemetas@mec.gov.br'), 'SIMEC - EMENDAS', $complementoConteudo, $cc, null);
								} else {
									$retornoPlano = true;
								}
								
								if($retorno && $retornoPlano){
									//Ativa usuario automatico no modulo de emendas
									//$ativa = ativaUsuarioAutomatico($dados, $enbid);
	
									//if($ativa){
										$sql = "UPDATE
										emenda.emendadetalheentidade
										SET edeemailenviado = 't' 
										WHERE edeid = {$dados['edeid']}";
										$db->executar($sql);
									//}
	
								}
								//}
	
							}
	
								
			} else{
	
				$sql = "INSERT INTO emenda.emendadetalheentidade ( emdid, enbid, edevalor, usucpfalteracao, ededataalteracao, edecpfresp, edenomerep, edemailresp, ededddresp, edetelresp, edestatus )
						VALUES ( {$dados["emdid"]}, {$enbid}, '{$dados["edevalor"]}', '{$_SESSION['usucpf']}', 'now()', '{$dados["edecpfresp"]}', '{$dados["edenomerep"]}', '{$dados["edemailresp"]}', '{$dados["ededddresp"]}', '{$dados["edetelresp"]}', 'A' ) RETURNING edeid";
	
				$edeid = $db->pegaUm( $sql );
	
				if($edeid){
	
					$sql = "SELECT edeemailenviado FROM emenda.emendadetalheentidade WHERE edeid = {$edeid}";
					$edeemailenviado = $db->pegaUm($sql);
	
					//if($edeemailenviado == 'f' && $_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao"){
					
					if($_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao" ){
						$retorno = enviar_email($remetente, array($dados["edemailresp"]), 'SIMEC - EMENDAS', $conteudo, $cc, null);
						$retornoPlano = enviar_email($remetente, array('planodemetas@mec.gov.br'), 'SIMEC - EMENDAS', $complementoConteudo, $cc, null);
					} else {
						$retornoPlano = true;
						$retorno = true;
					}
					
					if($retorno && $retornoPlano){
	
						//Ativa usuario automatico no modulo de emendas
						//$ativa = ativaUsuarioAutomatico($dados, $enbid);
	
						//if($ativa){
								
							$sql = "UPDATE
										emenda.emendadetalheentidade
										SET edeemailenviado = 't' 
										WHERE edeid = {$edeid}";
							$db->executar($sql);
						//}
	
					}
				}
				//}
	
			}
			$edeid = ($dados['edeid'] ? $dados['edeid'] : $edeid);
			$boPTA = $db->pegaUm("select count(pedid) from emenda.ptemendadetalheentidade pt
									inner join emenda.planotrabalho pr on pr.ptrid = pt.ptrid where pt.edeid = $edeid and pr.ptrstatus= 'A'");					
			if( $boPTA == 0 ){
				if( $dados['edeobjeto'][0]){
					$db->executar("DELETE FROM emenda.emendapariniciativa where edeid = $edeid and iniid not in (".implode(', ', $dados['edeobjeto']).")");
				}
				
				$retorno = insereIniciativaDetalheEntidade( $dados, $edeid );
				if( $retorno == true ) $db->sucesso("principal/cadastraDetalheEmenda");
				else $db->insucesso( 'Falha na operação' );
			
			} else {
				$db->commit();
				echo "<script>
						alert('Dados da entidade alterado com sucesso! Mas não foi possivel desvincular a iniciativa, pois ja existe PTA criado.');
						window.location.href = window.location; 
					</script>";
			}
		}
	} else {
		echo "<script>
					alert('Não é possivel distribuir recurso para uma entidade já indicada a emenda.');
					window.location.href = window.location; 
				</script>";
	}
}

function montaCorpoEmailEntidade($dados){
	extract($dados);
	$html = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td colspan="2" style="text-align: center;"> <b>Dados da Entidade</b> </td>
				</tr>
				<tr>
					<td style="width: 33%; text-align: right;">CNPJ:</td>
					<td>'.$enbcnpj.'</td>
				</tr>
				<tr>
					<td style="text-align: right;">Entidade Beneficiada:</td>
					<td>'.$enbnome.'</td>
				</tr>
				<tr>
					<td style="text-align: right;">UF:</td>
					<td>'.$estuf.'</td>
				</tr>
				<tr>
					<td colspan="2" style="text-align: center;"> <b>Dados do Responsável</b> </td>
				</tr>
				<tr>
					<td style="text-align: right;">CPF:</td>
					<td>'.$edecpfresp.'</td>
				</tr>
				<tr>
					<td style="text-align: right;">Nome:</td>
					<td>'.$edenomerep.'</td>
				</tr>
				<tr>
					<td style="text-align: right;">(DDD) Telefone:</td>
					<td>('.$ededddresp.') '.$edetelresp.'</td>
				</tr>
				<tr>
					<td style="text-align: right;">Função/Cargo:</td>
					<td>'.$edefuncaoresp.'</td>
				</tr>
				<tr>
					<td style="text-align: right;">E-mail:</td>
					<td>'.$edemailresp.'</td>
				</tr>
			</table>';
	return $html;
}

function ativaUsuarioAutomatico( $dados, $enbid = null ){
	global $db;

	//id modulo sistema
	$sisid = 57;

	$cpf = $dados["edecpfresp"];
	$usunome = $dados["edenomerep"];
	$usuemail = $dados["edemailresp"];
	$usufoneddd = $dados["ededddresp"];
	$usufonenum = $dados["edetelresp"];


	//variavel ativa usuario
	$suscod = 'A';

	//gera senha usuario
	$senhageral = $db->gerar_senha();

	//perfil Instituição Beneficiada
	$pflcod = 274;


	//pega dados entidade beneficiada
	$entid = $db->pegaUm("SELECT distinct entid FROM emenda.entidadebeneficiada where enbcnpj='".$dados['enbcnpj']."'");

	$orgcod 	= "";
	$unicod     = "";
	$ungcod     = "";
	$regcod     = $dados['estuf'];
	$orgao      = $dados['enbnome'];
	$muncod     = $dados['muncod'];
	if($muncod) $muncod = ltrim($muncod,"0");
	$tpocod     = "4";
	$carid		= "9";
	$usufuncao  = $dados["edefuncaoresp"];


	// verifica se o cpf já está cadastrado no simec
	$sql = "SELECT usucpf, ususenha FROM seguranca.usuario WHERE usucpf = '$cpf'";
	$usuario = $db->pegaLinha( $sql );

	if(!$usuario['usucpf']){

		$entid = $entid ? $entid : 'null';

		// insere informações gerais do usuário
		$sql = sprintf(
				"INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, carid, entid, unicod, usuchaveativacao, regcod,
					ususexo, ungcod, ususenha, suscod, orgao,
					muncod, tpocod
				) values (
					'%s', '%s', '%s', '%s', '%s',
					'%s', '%s', %s, '%s', '%s',
					'%s', '%s', '%s', '%s', '%s', 
					'%s', '%s', %s
				)",
		$cpf,
		str_to_upper( $usunome ),
		strtolower( $usuemail ),
		$usufoneddd,
		$usufonenum,
		$usufuncao,
		$carid,
		$entid,
		$unicod,
		'f',
		$regcod,
		$ususexo,
		$ungcod,
		md5_encrypt_senha( $senhageral, '' ),
		'P',
		$orgao,
		$muncod,
		$tpocod
		);

		$db->executar( $sql );

	}
	else{
		$sql = "UPDATE seguranca.usuario
				SET carid = $carid, 
					usufuncao = '$usufuncao',
					usuemail = '$usuemail',
					usufoneddd = '$usufoneddd', 
					usufonenum = '$usufonenum'
				WHERE usucpf = '$cpf'";
		$db->executar($sql);
			
		$senhageral = md5_decrypt_senha( $usuario['ususenha'], '' );
	}

	// verifica se o usuário já está cadastrado no módulo selecionado
	$sql = sprintf("SELECT usucpf, sisid, suscod FROM usuario_sistema WHERE usucpf = '%s' AND sisid = %d",$cpf,$sisid);
	$modulo = $db->pegaLinha( $sql );

	if(!$modulo['usucpf']){
		// vincula o usuário com o módulo
		$sqlu = sprintf(
	    		"INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '%s', %d, %d )",
		$cpf,
		$sisid,
		$pflcod
		);
		$db->executar( $sqlu );

	}


	if(!$modulo['suscod'] || $modulo['suscod'] == 'P' || $modulo['suscod'] == 'B'){

		//Ativa o usuário
		$justificativa = "Ativação automática de usuário pelo Módulo Emendas";
		$suscod = "A";
		$db->alterar_status_usuario( $cpf, $suscod, $justificativa, $sisid );

	}

	// cadastra o perfil Instituição Beneficiada
	$existe = $db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf = '$cpf' and pflcod=".$pflcod);
	if(!$existe){
		$sqlp = sprintf(
			"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )",
		$cpf,
		$pflcod
		);
		$db->executar( $sqlp );
	}

	// verifica se o perfil possui entidade
	if($enbid){
		$existeu = $db->pegaUm("SELECT enbid FROM emenda.usuarioresponsabilidade WHERE rpustatus = 'A' and usucpf = '$cpf' and enbid=".$enbid);
		if(!$existeu){
			$sqlp = sprintf(
				"INSERT INTO emenda.usuarioresponsabilidade ( pflcod, usucpf, enbid, rpustatus, rpudata_inc ) 
				VALUES ( %d, '%s', %d, 'A', now() )",
			$pflcod,
			$cpf,
			$enbid
			);
			$db->executar( $sqlp );
		}
	}


	// envia o email de confirmação da conta aprovada
	$remetente = array("nome" => "SIMEC - MÓDULO EMENDAS","email" => $usuemail);
	$destinatario = $usuemail;
	$assunto = "Cadastro no SIMEC - MÓDULO EMENDAS";
	$conteudo = "
		<br/>
		<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
		<br/>
		<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
		<br/>
		";
	$conteudo .= sprintf(
	"%s %s, <p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
	'Prezado(a)',
	$usunome,
	$senhageral
	);

	$conteudo .= "<br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.";

	//if($_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao"){
	enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	//}


	return true;


}

function insereIniciativaDetalheEntidade( $post, $edeid ){
	global $db;

	$edeid = ( $edeid ? $edeid : $post["edeid"] );
	$ideid = $post['ideid'];
	$sql = "UPDATE emenda.iniciativadetalheentidade SET idestatus = 'I' WHERE edeid = {$edeid} and idestatus = 'A'";
	$db->executar( $sql );
	$post['edeobjeto'] = ( $post['edeobjeto'] ? $post['edeobjeto'] : array() );
	foreach ($post['edeobjeto'] as $key => $iniid) {
		if( $ideid[$iniid] ){
			$sql = "UPDATE emenda.iniciativadetalheentidade SET
					  iniid = $iniid,
					  idestatus = 'A'
					WHERE 
					  ideid = {$ideid[$iniid]}
					  and edeid = {$edeid}";

			$db->executar( $sql );
		} else {
			$sql = "INSERT INTO emenda.iniciativadetalheentidade( edeid, iniid, idestatus)
				VALUES ( $edeid, $iniid, 'A')";

			$db->executar( $sql );
		}
	}
	return $db->commit();
}

/**
 * Exclui os dados de detalhes de uma entidade
 *
 * @param integer $edeid
 * @author Fernando Araújo Bagno da Silva
 * @since 11/09/2009
 *
 */
function excluirEntidadeDetalhe( $edeid ){
	global $db;
	
	$sql = "SELECT count(pa.ptrid) FROM emenda.ptemendadetalheentidade pe
					INNER JOIN emenda.planotrabalho pa ON pa.ptrid = pe.ptrid
				 WHERE pa.ptrstatus = 'A' AND edeid = $edeid";

	$existePTA = $db->pegaUm( $sql );

	$perfilEmenda = pegaPerfilArray($_SESSION['usucpf']);

	if( $existePTA == 0 || ( in_array( SUPER_USUARIO, $perfilEmenda ) || in_array( GESTOR_EMENDAS, $perfilEmenda ) ) ){
		$sql = "UPDATE emenda.emendadetalheentidade SET edestatus = 'I' WHERE edeid = {$edeid}";
		$db->executar($sql);

		$sql = "SELECT enbid FROM emenda.emendadetalheentidade WHERE edeid = {$edeid}";
		$enbid = $db->pegaUm( $sql );

		/*$sql = "UPDATE emenda.entidadebeneficiada SET enbstatus = 'I' WHERE enbid = {$enbid}";
		$db->executar( $sql );*/

		$db->commit();
		$db->sucesso( "principal/cadastraDetalheEmenda" );
	} else {
		echo "<script>
				alert('Não é possivel excluir está entidade, pois existe PTA vinculado a ela!');
				window.location.href = 'emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A';
			  </script>";
	}
}


function listaEntidadeLiberacao( $filtros ){
	global $db;
	$perfil = pegaPerfilArray($_SESSION["usucpf"]);

	if( in_array( ADMINISTRADOR_INST, $perfil) && !in_array( LIBERAR_SENHA, $perfil) ){
		$resid 		 = recuperaResponsaveis( $_SESSION["usucpf"], $perfil );
		$filtroResid = count($resid) > 0 ? " AND (er.resid in (" . implode( ", ", $resid ) . ") OR er.resid is null)" : "";
	}
	$filtroEntid = " ent.enbcnpj, ";
	if( $db->testa_superuser() ) $filtroEntid = " ent.enbid ||' - '|| ent.enbcnpj, ";
	
	$sql = "SELECT DISTINCT
				CASE WHEN ededisponivelpta = 'N' AND  emenda.f_verificardisponibilidadepta(ede.edeid, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ie WHERE ie.emdid = ed.emdid) > 0 THEN true ELSE false END ), ebc.ebccontrapartida, er.rescontrapartidapadrao, ee.emedescentraliza) is null
					 THEN '<center><input type=\"checkbox\" id=\"edeid[' || ede.edeid || ']\" name=\"edeid[]\" value=\"' || ede.edeid || '\" /></center>'
					 WHEN ededisponivelpta = 'N' AND  emenda.f_verificardisponibilidadepta(ede.edeid, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ie WHERE ie.emdid = ed.emdid) > 0 THEN true ELSE false END ), ebc.ebccontrapartida, er.rescontrapartidapadrao, ee.emedescentraliza) is not null
					 THEN '<center><input type=\"checkbox\" id=\"edeid[' || ede.edeid || ']\" name=\"edeid[]\" value=\"' || ede.edeid || '\" disabled=\"disabled\"/></center>'
					 WHEN ededisponivelpta = 'S' AND ( (SELECT count(1) FROM emenda.ptemendadetalheentidade ped INNER JOIN emenda.planotrabalho ptr ON ped.ptrid = ptr.ptrid AND ptr.ptrstatus = 'A' WHERE ped.edeid = ede.edeid ) > 0 )
					 THEN '<center><input type=\"checkbox\" id=\"edeid[' || ede.edeid || ']\" name=\"edeid[]\" value=\"' || ede.edeid || '\" disabled=\"disabled\" checked /></center>'
					 WHEN ededisponivelpta = 'S' 
					 THEN '<center><input type=\"checkbox\" id=\"edeid[' || ede.edeid || ']\" name=\"edeid[]\" value=\"' || ede.edeid || '\" checked/><input type=\"hidden\" name=\"liberado[]\" id=\"liberado[' || ede.edeid || ']\"  value=\"' || ede.edeid || '\"/></center>'
				END as acao,
				case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end as autor,
				ee.emecod as numero,
				(CASE WHEN ee.emetipo = 'E' THEN 'Emenda'
					ELSE 'Complemento' END) as tipoemenda,
				ef.fupfuncionalprogramatica::varchar||' ' as funcional,
				ed.gndcod||' - '||gn.gnddsc as gnd, 
			    ed.mapcod||' - '||map.mapdsc as mod,
			    ed.foncod||' - '||fon.fondsc as fon,
				ed.emdvalor as valoremenda,
				$filtroEntid
				ent.enbnome as entidade,
				ede.edevalor as valorentidade,
				CASE WHEN ee.resid is not null THEN er.resdsc ELSE 'Não Informado' END as responsavel,
				ee.emeano,
				CASE WHEN emenda.f_verificardisponibilidadepta(ede.edeid, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ie WHERE ie.emdid = ed.emdid) > 0 THEN true ELSE false END ), ebc.ebccontrapartida, er.rescontrapartidapadrao, ee.emedescentraliza) is not null 
					 THEN emenda.f_verificardisponibilidadepta(ede.edeid, er.resid, (CASE WHEN (select count(1) from emenda.iniciativaemendadetalhe ie WHERE ie.emdid = ed.emdid) > 0 THEN true ELSE false END ), ebc.ebccontrapartida, er.rescontrapartidapadrao, ee.emedescentraliza) 
					 ELSE '<center>-</center>' END as pendencia
			FROM emenda.emenda ee
			INNER JOIN emenda.autor ea ON ea.autid = ee.autid
			LEFT JOIN emenda.v_funcionalprogramatica ef ON ef.acaid = ee.acaid
			LEFT JOIN public.unidade pu ON pu.unicod = ef.unicod
			INNER JOIN emenda.emendadetalhe ed ON ed.emeid = ee.emeid 
			left JOIN emenda.emendadetalheentidade ede ON ede.emdid = ed.emdid
			left JOIN emenda.entidadebeneficiada ent ON ent.enbid = ede.enbid 
			LEFT JOIN emenda.responsavel er ON er.resid = ee.resid
			LEFT JOIN emenda.entbeneficiadacontrapartida ebc on ebc.enbid = ent.enbid
					AND ebc.ebcexercicio = '{$_SESSION["exercicio"]}'
			LEFT JOIN
				(select max(iedid) as iedid, emdid from emenda.iniciativaemendadetalhe GROUP BY emdid) ie ON ie.emdid = ed.emdid
			inner join public.gnd gn on gn.gndcod = ed.gndcod 
				and gn.gndstatus = 'A'
		    inner join public.modalidadeaplicacao map on map.mapcod = ed.mapcod
		    left join public.fonterecurso fon on fon.foncod = ed.foncod 
		    	and fon.fonstatus = 'A'
			WHERE
				ede.edestatus = 'A' 
				AND ee.emeano >= '{$_SESSION["exercicio"]}' " . ( !empty($filtros['filtro']) ? " AND " . implode(" AND ", $filtros['filtro']) : "" ) . " 
				{$filtroResid}
			ORDER BY
				responsavel, autor, numero, entidade";

				$cabecalho = array( "Disponibilizar", "Autor", "Código", "Tipo Recurso", "Funcional Prg", "GND", "Mod", "Fon", "Valor Recurso (R$)", "CNPJ", "Entidade", "Valor Entidade (R$)", "Responsável", "Ano", "Pendências" );
				$db->monta_lista($sql, $cabecalho, 100000, 30, 'N', 'center', '', 'formLibera');
				//$db->monta_lista_simples($sql, $cabecalho, 100000, 30, 'N', '95%', '', '', '', '', true);
}

//---- FIM MANTER EMENDAS----

function carregaEmendaDetalhePTA_Ajax($post){
	global $db;

	$ptrid = $post['ptrid'];
	$emecod = $post['emecod'];
	$autid = $post['autid'];

	$filtro = "";

	if($emecod){
		$filtro.= " AND vede.emecod = '".$emecod."'";
	}
	if($autid){
		$filtro.= " AND aut.autid = '".$autid."'";
	}

	$sql = "SELECT DISTINCT
			    vede.emecod, 
			    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome, 
			    fup.fupdsc, 
			    vede.gndcod||' - '||gn.gnddsc as gndcod1, 
			    vede.mapcod||' - '||map.mapdsc as modalidade,
			    vede.foncod||' - '||fon.fondsc as fonte,
			    ped.pedvalor
			FROM emenda.planotrabalho ptr
			    INNER JOIN emenda.ptemendadetalheentidade  ped
			        ON ptr.ptrid = ped.ptrid
			    INNER JOIN emenda.v_emendadetalheentidade vede
			    	ON vede.edeid = ped.edeid
			    left JOIN emenda.v_funcionalprogramatica fup
			        ON (vede.acaid = fup.acaid)
			    inner JOIN emenda.autor aut
			        ON aut.autid = vede.autid
                left join public.gnd gn
			        on gn.gndcod = vede.gndcod
	                and gn.gndstatus = 'A'
			    left join public.modalidadeaplicacao map
			        on map.mapcod = vede.mapcod
			    left join public.fonterecurso fon
			        on fon.foncod = vede.foncod 
	                and fon.fonstatus = 'A'
			WHERE ptr.ptrid = $ptrid
			    AND vede.edestatus = 'A' " . $filtro;

	//$arDados = $db->carregar($sql);

	echo '<table class="tabela" cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none;">
			<tr>
				<td bgcolor="#e9e9e9" align="center" style=""><b>Recurso(s) Vinculado(s) ao Plano de Trabalho</b></td>
			</tr>
		</table>';
	$cabecalho = array("Recurso", "Autor", "Funcional Programática", "GND", "Mod", "Fonte", "Valor");
	$db->monta_lista_simples($sql, $cabecalho, 20, 5);

	$sql = "SELECT
				ini.iniid,
				ini.ininome
			FROM 
				emenda.ptiniciativa pti
                inner join emenda.iniciativa ini
                	on ini.iniid = pti.iniid
                    and ini.inistatus = 'A'
			WHERE 
				pti.ptrid = $ptrid";

	echo '<table class="tabela" cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none;">
			<tr>
				<td bgcolor="#e9e9e9" align="center" style=""><b>Iniciativa(s) Vinculada(s) ao Plano de Trabalho</b></td>
			</tr>
		</table>';

	$cabecalho = array("Código", "Iniciativa");
	$db->monta_lista_simples($sql, $cabecalho, 20, 5);
	/*?>
	 <table width="95%" id="tb_tabela" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	 <thead>
		<tr>
		<td align="Center" class="title" width="5%"
		style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
		onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Emenda</strong></td>
		<td align="Center" class="title" width="25%"
		style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
		onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Autor</strong></td>
		<td align="Center" class="title" width="45%"
		style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
		onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Funcional Programática</strong></td>
		<td align="Center" class="title" width="5%"
		style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
		onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>GND</strong></td>
		<td align="Center" class="title" width="5%"
		style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
		onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Mod</strong></td>
		<td align="Center" class="title" width="5%"
		style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
		onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte</strong></td>
		<td align="Center" class="title" width="10%"
		style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
		onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor</strong></td>
		</tr>
		</thead><?php
		if($arDados){
		foreach ($arDados as $key => $value) {
		$key % 2 ? $cor = "#f7f7f7" : $cor = "";
		?>
		<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
		<td style="text-align: center; color: rgb(0, 102, 204);"><?=$value['emecod']; ?></td>
		<td><?=$value['autnome']; ?></td>
		<td><?=$value['fupdsc']?></td>
		<td style="text-align: center; color: rgb(0, 102, 204);"><?=$value['gndcod']; ?></td>
		<td style="text-align: center; color: rgb(0, 102, 204);"><?=$value['mapcod']; ?></td>
		<td style="text-align: center; color: rgb(0, 102, 204);"><?=$value['foncod']; ?></td>
		<td style="text-align: center; color: rgb(0, 102, 204);">R$ <?=number_format($value['pedvalor'],2,',','.')?></td>
		</tr>
		<?
		}
		} else{
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
		print '</table>';
		}
		echo "</table>";*/
}

// ---- FIM MANTER CONTRAPARTIDA MÍNIMA ----

function cabecalhoHabilitacao($entid){
	global $db;

	$sql = "  SELECT
				  hen.henid,
				  hen.hencnpj,
				  hen.henrazaosocial,
				  hen.mundescricao,
				  hen.estuf,
				  hen.henprocesso,
				  hen.hendocumenta,
				  hen.hendivdocumenta,
				  hen.hensituacao,
				  hen.hendatacadastramento,
				  hen.hendataimportacao
				FROM 
				  emenda.habilitaentidade hen
	              inner join entidade.entidade ent
				  	on ent.entnumcpfcnpj = hen.hencnpj
				WHERE
				  ent.entid = $entid";

	$arDadosEntidade = $db->pegaLinha($sql);

	$arDadosEntidade['hencnpj'] = substr($arDadosEntidade['hencnpj'],0,2) . "." .
	substr($arDadosEntidade['hencnpj'],2,3) . "." .
	substr($arDadosEntidade['hencnpj'],5,3) . "/" .
	substr($arDadosEntidade['hencnpj'],8,4) . "-" .
	substr($arDadosEntidade['hencnpj'],12,2);

	$cabecalho = "<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
				 <tbody>
				 	<tr>
				 		<td colspan=\"4\" class=\"subtitulocentro\">Dados da Entidade Habilitação</td>
				 	</tr>
					<tr>
						<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">CNPJ:</td>
						<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$arDadosEntidade['hencnpj']}</td>
						<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">Razão Social:</td>
						<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$arDadosEntidade['henrazaosocial']}</td>
					</tr>
					<tr>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Município:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$arDadosEntidade['mundescricao']}</td>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">UF:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$arDadosEntidade['estuf']}</td>
					</tr>
					<tr>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Número do processo:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$arDadosEntidade['henprocesso']."</td>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Numero do Documenta:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$arDadosEntidade['hendocumenta']}</td>
					</tr>
					<tr>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Situação:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$arDadosEntidade['hensituacao']."</td>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">DV Documenta:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$arDadosEntidade['hendivdocumenta']."</td>
					</tr>
				 </tbody>
				</table>";

	return $cabecalho;
}

// ---- FUNCOES DAS ANALISES DO PTA ----

function pegaUnidadeAnalisador( $usucpf, $perfil ){

	global $db;

	if( is_array($perfil) )
	$filtro = "pflcod in (" . implode( ',', $perfil).")";
	else
	$filtro = "pflcod = " . $perfil;

	$sql = "SELECT uniid FROM emenda.usuarioresponsabilidade
			WHERE usucpf = '{$usucpf}' AND rpustatus = 'A' AND $filtro";

	$unidades = $db->carregarColuna( $sql );

	return $unidades;

}


function verificaExistePTA( $ptrid ){

	global $db;
	if($ptrid){
		$sql  = "SELECT ptrid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}";
		$dado = $db->pegaUm( $sql );
	}

	return !$dado ? false : true;

}

function filtroPtaAnalise( $dados ){

	// trata o campo CNPJ
	if ( !empty( $dados["enbcnpj"] ) ){

		$dados["enbcnpj"] = str_replace(".", "", $dados["enbcnpj"]);
		$dados["enbcnpj"] = str_replace("/", "", $dados["enbcnpj"]);
		$dados["enbcnpj"] = str_replace("-", "", $dados["enbcnpj"]);

	}

	if( !empty($dados['ptrnumprocessoempenho']) ){
		$dados["ptrnumprocessoempenho"] = str_replace(".", "", $dados["ptrnumprocessoempenho"]);
		$dados["ptrnumprocessoempenho"] = str_replace("/", "", $dados["ptrnumprocessoempenho"]);
		$dados["ptrnumprocessoempenho"] = str_replace("-", "", $dados["ptrnumprocessoempenho"]);
	}

	if( $dados['autid'] || $dados['emecod'] ){

		$filt.= !empty( $dados["emecod"] ) ? " AND e.emecod = '{$dados["emecod"]}'" : "";
		$filt.= !empty( $dados["autid"] ) ? " AND e.autid = {$dados["autid"]}" : "";

		$filtro.= "AND 0 <> (SELECT count(1) FROM emenda.autor a
					  INNER JOIN emenda.emenda e ON e.autid = a.autid
					  INNER JOIN emenda.emendadetalhe ed ON ed.emeid = e.emeid
					  INNER JOIN emenda.emendadetalheentidade ede ON ed.emdid = ede.emdid
					  INNER JOIN emenda.ptemendadetalheentidade pede ON ede.edeid = pede.edeid
					  WHERE pede.ptrid = ptr.ptrid
					  ".$filt.")";
	}
	
	if( !empty($dados['empenhado']) ){
		if($dados['empenhado'] == 'S'){
			$filtro.= " and exe.semid = 4 ";
		}else{
			$filtro.= " and exe.semid <> 4 ";
		}
	}
	
	$filtro .= ( $dados["situacaorp"] == "1" ) ? " AND pmcdatarap is not null" : "";
	$filtro .= ( $dados["situacaorp"] == "2" ) ? " AND pmcdatarap is null" : "";
	$filtro .= ( $dados["situacao"] == "1" ) ? " AND ptmin.pmcdataconversaosiafi is not null" : "";
	$filtro .= ( $dados["situacao"] == "2" ) ? " AND ptmin.pmcdataconversaosiafi is null" : "";
	$filtro .= ( $dados["sitconvenio"] == "1" ) ? " AND ptmin.pmcnumconveniosiafi is not null" : "";
	$filtro .= ( $dados["sitconvenio"] == "2" ) ? " AND ptmin.pmcnumconveniosiafi is null" : "";
	$filtro .= !empty( $dados["esdid"] ) ? " AND doc.esdid = {$dados["esdid"]}" : "";
	$filtro .= !empty( $dados["resid"] ) ? " AND ptr.resid = {$dados["resid"]}" : "";
	$filtro .= !empty( $dados["enbcnpj"] ) ? " AND ent.enbcnpj = '{$dados["enbcnpj"]}'" : "";
	$filtro .= !empty( $dados["enbnome"] ) ? " AND UPPER( removeacento(ent.enbnome) ) ilike '%".removeAcentos( str_to_upper( trim($dados["enbnome"]) ) )."%'" : "";
	$filtro .= !empty( $dados["estuf"] ) ? " AND mun.estuf = '{$dados["estuf"]}'" : "";
	$filtro .= !empty( $dados["mundescricao"] ) ? " AND UPPER( removeacento(mun.mundescricao) ) ilike '%".removeAcentos( str_to_upper( trim($dados["mundescricao"]) ) )."%'" : "";
	$filtro .= !empty( $dados["uniid"] ) ? " AND ana.uniid = {$dados["uniid"]}" : "";
	$filtro .= !empty( $dados["ptrid"] ) ? " AND ptr.ptrcod = '".substr( trim(str_replace(' ', '', $dados["ptrid"])), 0, 10)."'" : "";
	$filtro .= !empty( $dados["ptridchave"] ) ? " AND ptr.ptrid = {$dados["ptridchave"]}" : "";
	$filtro .= !empty( $dados["iniid"] ) ? " AND pti.iniid = {$dados["iniid"]}" : "";
	$filtro .= !empty( $dados["espid"][0] )
	?
					" AND esp.espid " . (( $dados['espid_campo_excludente'] == null || $dados['espid_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " (" . implode( ", ", $dados["espid"] ) . ")" 
					:
					"";
					$filtro .= !empty( $dados["nomearq"] ) ? " AND aptr.abtid = {$dados["nomearq"]}" : "";
					$filtro .= !empty( $dados["nomearqrap"] ) ? " AND rap.rapid = {$dados["nomearqrap"]}" : "";

					$filtro .= !empty( $dados["ptrnumconvenio"] ) ? " AND ptr.ptrnumconvenio = '{$dados["ptrnumconvenio"]}' AND ptr.ptranoconvenio = '{$_SESSION['exercicio']}' " : "";
					$filtro .= !empty( $dados["ptrnumprocessoempenho"] ) ? " AND ptr.ptrnumprocessoempenho = '{$dados["ptrnumprocessoempenho"]}'" : "";


					if( $dados['liberadocasacivil'] ){
						$filtro.= "AND 0 <> (Select count(emdliberado)
							from emenda.emendadetalheentidade ede 
							inner join emenda.ptemendadetalheentidade ped on ede.edeid = ped.edeid 
							where ped.ptrid = ptr.ptrid
							and emdliberado = '". $dados['liberadocasacivil'] ."')";
					}

					if( $dados['pendencia'] == 'S' ){
						$filtro.= " and emenda.verificapendencias(ptr.ptrid) <> ''";
					} else if( $dados['pendencia'] == 'N' ){
						$filtro.= " and emenda.verificapendencias(ptr.ptrid) = ''";
					}
					
	return $filtro;
}

function listaPtaAnalise( $filtros = '', $boExportar = false ){

	global $db;

	if(pegaPerfil($_SESSION["usucpf"]) == ADMINISTRADOR_INST){
		$resid 		 = recuperaResponsaveis( $_SESSION["usucpf"] );
		$filtroResid = count($resid) > 0 ? " AND (res.resid in (" . implode( ", ", $resid ) . ") OR res.resid is null)" : "";
	}

	if( !empty($_POST["iniid"]) || !empty($_POST['espid']) ){
		$filtroIniciativa = "INNER JOIN emenda.v_ptiniciativa pti ON ptr.ptrid = pti.ptrid ";
	}

	if( is_array( $_POST['espid'] ) ){
		$filtroEspef = "INNER JOIN emenda.iniciativaresponsavel ir on ir.resid = res.resid
			INNER JOIN emenda.iniciativa ini on ini.iniid = ir.iniid
			INNER JOIN emenda.iniciativaespecificacao ine on ine.iniid = ini.iniid
			INNER JOIN emenda.especificacao esp on esp.espid = ine.espid
			inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
            INNER JOIN emenda.ptiniciativaespecificacao ptie on ptie.iceid = ine.iceid
            	and ptie.ptiid = pti.ptiid
				and ptie.ptestatus = 'A'";
	}

	if( !empty($_POST["uniid"]) ){
		$filtroUnidade = "INNER JOIN emenda.analise ana ON ptr.ptrid = ana.ptrid ";
	}
	
	if( !empty($_POST['empenhado']) ){
		$filtroExecFinanceira = ' inner join emenda.execucaofinanceira exe on exe.ptrid = ptr.ptrid ';
	}
	
	//$filtroPorAno = " AND ptr.ptrid  in (SELECT ptrid FROM emenda.analise WHERE anastatus = 'A' and anatipo = 'D' and analote = 1) ";
//	if( $_SESSION['exercicio'] == '2011' ) $filtroPorAno = " AND esd.esdid <> 52 ";

	$sql = "SELECT DISTINCT
				CASE WHEN doc.esdid is not null 
					THEN '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"selecionaPlanoTrabalho(' || ptr.ptrid || ', ' || doc.esdid || ');\"></center>'
					ELSE '<center><img src=\"../imagens/alterar_01.gif\" border=\"0\"></center>' END as acoes,
				ptr.ptrcod,
				ptr.ptrexercicio,
				ptr.ptrid,
				ent.enbcnpj,
				ent.enbnome,
				ent.estuf,
				mun.mundescricao,
				res.mdedescricao as resassunto,
				coalesce( (select sum(vp.ptivalortotal) from emenda.v_ptiniciativa vp where vp.ptrid = ptr.ptrid) ,0) as valorTotal,
				CASE WHEN doc.esdid is not null THEN esd.esddsc ELSE 'Não Informado' END as situacao,
				doc.esdid,
				(Select exfnumempenhooriginal from emenda.execucaofinanceira where exfstatus = 'A' and ptrid = ptr.ptrid order by exfid limit 1) as exfnumempenhooriginal,
				--(Select semid from emenda.execucaofinanceira where exfstatus = 'A' and ptrid = ptr.ptrid order by exfid limit 1) as semid,
				--(Select orbnumsolicitacao from emenda.ordembancaria ob inner join emenda.execucaofinanceira exf on ob.exfid = exf.exfid where exf.exfstatus = 'A' and exf.ptrid = ptr.ptrid order by exf.exfid limit 1) as orbnumsolicitacao,
				
				(Select se.semdesc from emenda.execucaofinanceira ef 
                		inner join emenda.situacaoempenho se on ef.semid = se.semid where ef.exfstatus = 'A' and ef.ptrid = ptr.ptrid limit 1) as semdesc,
                
                (Select sp.spgdescricao from emenda.execucaofinanceira ef
                		inner join emenda.ordembancaria ob on ob.exfid = ef.exfid
                		inner join emenda.situacaopagamento sp on ob.spgcodigo = sp.spgcodigo 
                	where ef.exfstatus = 'A' and ef.ptrid = ptr.ptrid order by ob.orbdatainclusao desc limit 1) as spgdescricao,
				
				(Select case when emdliberado = 'S' then 'Sim' else 'Não' end from emenda.emendadetalheentidade ede inner join emenda.ptemendadetalheentidade ped on ede.edeid = ped.edeid where ped.ptrid = ptr.ptrid order by emdliberado desc limit 1) as emdliberado 	
			FROM  emenda.planotrabalho ptr
			LEFT JOIN  emenda.entidadebeneficiada ent ON ptr.enbid = ent.enbid 
            LEFT JOIN territorios.municipio mun on mun.muncod = ent.muncod
			--INNER JOIN emenda.responsavel res ON ptr.resid = res.resid
			inner join emenda.modalidadeensino res on res.mdeid = ptr.mdeid and res.resid = ptr.resid
			INNER JOIN workflow.documento doc ON ptr.docid = doc.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			$filtroExecFinanceira
			$filtroIniciativa
			$filtroEspef
			$filtroUnidade
			WHERE
			    ptr.ptrstatus = 'A' 
			    AND ptr.ptrexercicio = ".$_SESSION['exercicio']."
			    AND esd.esdid not in (".EM_ELABORACAO.", ".EM_ELABORACAO_IMPOSITIVO.")
			    {$filtros}
		    	{$filtroResid}
		    	AND ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')
			GROUP BY 
			    ptr.ptrid,
			    ent.enbnome,
			    ent.estuf,
			    res.mdedescricao,
			    mun.mundescricao,
			    esd.esddsc,
			    doc.esdid,
			    ent.enbcnpj,
			    exfnumempenhooriginal,
			    --semid,
			    ptr.ptrcod,
			    emdliberado,
			    ptr.ptrexercicio
			ORDER BY 
				ptr.ptrcod,
				ent.estuf,
				ent.enbnome";

		    	$arDados = $db->carregar($sql);

		    	$arConfEstDoc = array();
		    	$arConfEstDoc[EM_ANALISE_DE_DADOS] = 'D';
		    	$arConfEstDoc[EM_ANALISE_DE_MERITO] = 'M';
		    	$arConfEstDoc[EM_ANALISE_TECNICA] = 'T';
		    	$arConfEstDoc[EM_ANALISE_JURIDICA] = 'J';

		    	$i = 0;

		    	if( $arDados ){
		    		$registro = array();
		    		$arDadosArray = array();

		    		foreach ($arDados as $key => $value) {

		    			$esdid = $value['esdid'];
		    			$anatipo = $arConfEstDoc[$esdid];
		    			$temStatus = 'Pendente';

		    			if( pegarEstadoAtual($value['ptrid']) == EM_EMPENHO ){
		    				
		    				$sql = "SELECT count(em.empnumeroprocesso)
									FROM par.subacaoemendapta sep
										inner join par.subacaodetalhe sd on sd.sbdid = sep.sbdid
										inner join par.subacao s on s.sbaid = sd.sbaid
										inner join par.empenhosubacao e on e.sbaid = s.sbaid -- se tiver nesta tabela teve empenho
										inner join par.empenho em on em.empid = e.empid and empstatus = 'A' and eobstatus = 'A'
										inner join emenda.ptemendadetalheentidade ped on ped.ptrid = sep.ptrid
										inner join emenda.v_emendadetalheentidade ved on ved.edeid = ped.edeid
									WHERE 
										sep.ptrid = {$value['ptrid']}
										and sep.sepstatus = 'A'
									    and ved.edestatus = 'A'";
									    
							$boEmpenhoPar = $db->pegaUm( $sql );
		    				
							if( $boEmpenhoPar != 0 ){
								$temStatus = 'Empenho Efetivado';
							} else {
		    					$temStatus = (empty($value['semdesc']) ? 'Pendente' : $value['semdesc'] );
							}
		    				
		    				
		    				/*if( !empty($value['exfnumempenhooriginal']) && $value['semid'] == '4' ){
		    				 $temStatus = 'Concluído';
		    				 }*/
		    			} elseif( pegarEstadoAtual($value['ptrid']) == EM_LIBERACAO_RECURSO ){
		    				$temStatus =  (empty($value['spgdescricao']) ? 'Pendente' : $value['spgdescricao'] );
		    				/*if( !empty($value['orbnumsolicitacao']) ){
		    				 $temStatus = 'Concluído';
		    				 }*/
		    			} else {
		    				/*$sql = "select distinct case when anadataconclusao is null then 'Pendente' else 'Concluído' end as status
		    				 from emenda.analise where anatipo = '$anatipo'
		    				 and ptrid = {$value['ptrid']}
		    				 and anastatus = 'A'
		    				 and analote = (select max(analote) from emenda.analise where anatipo = '$anatipo' and ptrid = {$value['ptrid']})
		    				 order by status desc
		    				 limit 1";*/

		    				$sql = "SELECT DISTINCT
							CASE WHEN ana.anadataconclusao is null THEN 'Pendente' 
						    	ELSE 'Concluído' 
						    END as status
						FROM 
							emenda.analise ana 
						WHERE 
							ana.anatipo = '$anatipo'
						    and ana.ptrid = {$value['ptrid']}
						    and ana.anastatus = 'A'
						    and ana.analote = (SELECT max(analote) 
						    				FROM emenda.analise 
						                    WHERE anatipo = '$anatipo'
						                    	and anastatus = 'A' 
						                    	and ptrid = ana.ptrid)
						ORDER BY 
							status DESC LIMIT 1";

		    				$temStatus = $db->pegaUm($sql);
		    			}

		    			$sqlIni = "SELECT ini.ininome
						FROM emenda.ptiniciativa pti
						INNER JOIN emenda.iniciativa ini on ini.iniid = pti.iniid
						WHERE pti.ptrid = {$value['ptrid']}
						ORDER BY ini.ininome";

		    			$arIniNome = $db->carregarColuna($sqlIni);

		    			$arIniNome = ($arIniNome ? $arIniNome : array());

		    			//if($temStatus){
		    			$status = $temStatus;
		    			//}
		    			$arUnidade = array();
		    			if($status == 'Pendente'){
		    				if( !$boExportar ){
		    					$status =  "<font color=\"red\">$status</font>";
		    				}

		    				if($anatipo == 'M' || $anatipo == 'T'){

		    					$sql = "SELECT DISTINCT
							  uni.unisigla,
							  uni.uninome
							FROM 
							  emenda.unidades uni
							  inner join emenda.analise ana
							  	on ana.uniid = uni.uniid
							WHERE
								uni.unistatus = 'A'
							    and ana.ptrid = {$value['ptrid']}
							    and ana.anadataconclusao is null";

		    					$arUnidade = $db->carregarColuna($sql);
		    				}
		    			}

		    			if( !$boExportar ){
		    				if( $_SESSION['exercicio'] == '2010' ){
		    					$registro = array( "acao" => $value['acoes'],
									   "ptrid" => $value['ptrcod']."/".$value['ptrexercicio'],
									   "enbcnpj" => $value['enbcnpj'],
									   "enbnome" => $value['enbnome'],			
									   "estuf" => $value['estuf'],	
									   "mundescricao" => $value['mundescricao'],	
									   "resassunto" => $value['resassunto'],	
									   "valortotal" => $value['valortotal'],	
									   "situacao" => $value['situacao'],	
									   "status" => $status,
									   "unidade" => implode(',<br/>', $arUnidade),
									   "iniciativa" => implode(',<br/>', $arIniNome),
									   "emdliberado" => $value['emdliberado']
									  );
		    				} else {
		    					$registro = array( "acao" => $value['acoes'],
									   "ptrid" => $value['ptrcod']."/".$value['ptrexercicio'],
									   "enbcnpj" => $value['enbcnpj'],
									   "enbnome" => $value['enbnome'],			
									   "estuf" => $value['estuf'],	
									   "mundescricao" => $value['mundescricao'],	
									   "resassunto" => $value['resassunto'],	
									   "valortotal" => $value['valortotal'],	
									   "situacao" => $value['situacao'],	
									   "status" => $status,
									   "unidade" => implode(',<br/>', $arUnidade),
									   "iniciativa" => implode(',<br/>', $arIniNome)
									  );
		    				}
		    			} else {
		    				if( $_SESSION['exercicio'] == '2010' ){
		    					$registro = array(
									"ptrid" => $value['ptrcod']."/".$value['ptrexercicio'],
								    "enbcnpj" => $value['enbcnpj'],
								    "enbnome" => $value['enbnome'],			
								    "estuf" => $value['estuf'],	
								    "mundescricao" => $value['mundescricao'],	
								    "resassunto" => $value['resassunto'],	
								    "valortotal" => number_format($value['valortotal'], 2 , ',', '.'),	
								    "situacao" => $value['situacao'],	
								    "status" => $status,
								    "unidade" => implode(', ', $arUnidade),
								    "iniciativa" => implode(', ', $arIniNome),
									"emdliberado" => $value['emdliberado']
		    					);
		    				} else {
		    					$registro = array(
									"ptrid" => $value['ptrcod']."/".$value['ptrexercicio'],
								    "enbcnpj" => $value['enbcnpj'],
								    "enbnome" => $value['enbnome'],			
								    "estuf" => $value['estuf'],	
								    "mundescricao" => $value['mundescricao'],	
								    "resassunto" => $value['resassunto'],	
								    "valortotal" => number_format($value['valortotal'], 2 , ',', '.'),	
								    "situacao" => $value['situacao'],	
								    "status" => $status,
								    "unidade" => implode(', ', $arUnidade),
								    "iniciativa" => implode(', ', $arIniNome)
		    					);
		    				}
		    			}

		    			array_push($arDadosArray, $registro);
		    		}
		    	}
		    	if( !$boExportar ){
		    		if( $_SESSION['exercicio'] == '2010' ){
		    			$cabecalho = array("Ação", "Nº PTA", "CNPJ", "Órgão ou Entidade", "UF", "Município", "Nível de Ensino", "Valor Total", "Situação", "Status", "Unidade", "Iniciativa", "Limite Autorizado");
		    		} else {
		    			$cabecalho = array("Ação", "Nº PTA", "CNPJ", "Órgão ou Entidade", "UF", "Município", "Nível de Ensino", "Valor Total", "Situação", "Status", "Unidade", "Iniciativa");
		    		}
		    		$db->monta_lista_array($arDadosArray, $cabecalho, 20, 10, 'N','Center');
		    	} else {
		    		return $arDadosArray;
		    	}
}

function salvaAnaliseMerito( $dados ){

	global $db;

	/*if ( $dados["concluir"] == 'true' ){
		$concluir = ", anadataconclusao = 'now'";
		}*/

	$sql = "UPDATE
				emenda.analise 
			SET 
				anasituacaoparecer = '{$dados["anasituacao"]}',
				anaparecer = '{$dados["anaparecer"]}',
				usucpf = '{$_SESSION["usucpf"]}',
				anadataconclusao = 'now'
			WHERE
				anaid = {$dados["anaid"]}";

	$db->executar( $sql );
	insereDadosAnaliseHistorico($dados["anaid"], $dados['anaparecer'], $dados["anasituacao"], $_SESSION["usucpf"]);

	/*if(salvaAnexoPorAnaid( $dados["anaid"] )){
		return true;
	} else {*/
		return $db->commit();
	//}
}

function deletaFilhosPTA( $ptrid ){
	global $db;
	$sql = "DELETE FROM emenda.logerrows WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.anexo WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptmail WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.contacorrentehistorico WHERE cocid in ( select cocid FROM emenda.contacorrente WHERE ptrid in ($ptrid) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.contacorrente WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.anexo WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.analisehistorico WHERE anaid in (SELECT anaid FROM emenda.analise WHERE ptrid in ($ptrid))";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.analise WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptvigencia WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptpublicacao WHERE pmcid in ( select pmcid from emenda.ptminutaconvenio where ptrid in ($ptrid) ) ";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.intervenienteconvenio WHERE pmcid in ( select pmcid from emenda.ptminutaconvenio where ptrid in ($ptrid) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.documentosextras WHERE ptrid in ( $ptrid )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.reformulatipos WHERE refid in ( select refid from emenda.ptminreformulacao WHERE ptrid in ($ptrid) )";
	$db->executar( $sql );
	$sql = "update emenda.planotrabalho set refid = null WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptminreformulacao WHERE ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.objetominutaconvenio WHERE pmcid IN(SELECT pmcid FROM emenda.ptminutaconvenio where ptrid in ($ptrid))";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptminutaconvenio where ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.execfinanceirahistorico WHERE exfid in (select exfid from emenda.execucaofinanceira where ptrid in ($ptrid) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ordembancaria where exfid in (select exfid from emenda.execucaofinanceira where ptrid in ($ptrid) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.logerrows where exfid in (SELECT exfid FROM emenda.execucaofinanceira where ptrid in ($ptrid))";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.execucaofinanceira where ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptparcelainiciativa where prdid in (select prdid from emenda.ptparceladesembolso where ptrid in ($ptrid) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptparceladesembolso where ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptescolasbeneficiadas where ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptiniciativabeneficiario where ptiid in (select ptiid from emenda.ptiniciativa where ptrid in ($ptrid) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptiniesprecurso
			WHERE pteid IN (select pteid from emenda.ptiniciativaespecificacao 
									where ptiid in (select ptiid from emenda.ptiniciativa where ptrid in ($ptrid) ) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptemendadetalheentidade where ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptitemkit where pteid in (select pteid from emenda.ptiniciativaespecificacao where ptiid in (select ptiid from emenda.ptiniciativa where ptrid in ($ptrid) ) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptiniciativaespecificacao where ptiid in (select ptiid from emenda.ptiniciativa where ptrid in ($ptrid) )";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.ptiniciativa where ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.historicogeral where ptrid in ($ptrid)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.planotrabalho where ptrid in ($ptrid)";
	$db->executar( $sql );

	$db->commit();

	$arTipos = pegaTipoReformulacao( $ptrid );

	if(is_array($arTipos))
	{
		$sql = "SELECT
					aedid,
					docid
				FROM 
					workflow.historicodocumento 
				WHERE
					hstid = (SELECT
								max(hstid)
							FROM
								workflow.historicodocumento hwf
							INNER JOIN emenda.planotrabalho       pt ON pt.docid   = hwf.docid
							WHERE
								aedid in (495,500,502,707) AND ptrid = {$ptrid})";
		$dados = $db->pegaLinha($sql);
	}
	else
	{
		$sql = "SELECT
					aedid,
					docid
				FROM 
					workflow.historicodocumento
				WHERE
					hstid = (SELECT 
							max(hwf.hstid)
						 FROM 
						 	workflow.historicodocumento hwf
						 INNER JOIN workflow.acaoestadodoc    awf ON awf.aedid  = hwf.aedid
						 INNER JOIN workflow.estadodocumento esd1 ON esd1.esdid = awf.esdidorigem
						 INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = awf.esdiddestino
						 INNER JOIN emenda.planotrabalho       pt ON pt.docid   = hwf.docid
						 WHERE
							ptrid = {$ptrid} AND hstid < (SELECT
											min(hstid)
										FROM
											workflow.historicodocumento hwf
										INNER JOIN emenda.planotrabalho       pt ON pt.docid   = hwf.docid
										WHERE
											aedid in (290,359,366,415,441,494,508,551,601) AND ptrid = {$ptrid}))";
		$dados = $db->pegaLinha($sql);
	}

	if(is_array($dados)){
		$sql = "INSERT INTO workflow.historicodocumento(
		            aedid, 
		            docid, 
		            usucpf, 
		            htddata)
				VALUES (".$dados['aedid'].", ".$dados['docid'].", ".$_SESSION['usucpf'].", now())";
		$db->executar($sql);
	}

	return $db->commit();
}

function deletaReformulacaoPTA( $ptrid, $refjustificativaexclusao = '' ){
	global $db;
	require_once(APPRAIZ . 'includes/workflow.php');
	
	$ptridPai = pegaPaiPTA( $ptrid );
	$sql = "UPDATE emenda.ptminreformulacao SET refsituacaoreformulacao = 'E', refdataexclusaoreformulacao = now(), refjustificativaexclusao = '{$refjustificativaexclusao}', refcpfexclusao = '{$_SESSION['usucpforigem']}' WHERE ptrid = $ptrid; ";
	$sql.= "UPDATE emenda.planotrabalho SET ptrstatus = 'I' where ptrid = $ptrid; ";
	$sql.= "UPDATE emenda.ptvigencia set vigstatus = 'I' where ptrid = $ptrid; ";
	$sql.= "UPDATE emenda.planotrabalho SET ptrstatus = 'A' where ptrid = $ptridPai; ";
	$db->executar( $sql );
	
	$docid =  $db->pegaUm("SELECT docid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
	
	/*$sql = "SELECT ed.esdid, ed.esddsc, ac.aeddscrealizada, ac.aedid, ac.* 
		    FROM workflow.historicodocumento hd
		    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
		    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
		    WHERE hd.docid = {$docid} 
		    order by hd.hstid desc";
    
    $historicoDocumento = $db->carregar($sql);
    $retorno = '';
    
    $arrRetorno = array();
    foreach($historicoDocumento as $chave => $documento){    	
        if($documento['esdid'] == EM_SOLICITACAO_REFORMULACAO){
            $esdiddestino = $historicoDocumento[$chave+1]['esdid'];
            $aedid = $historicoDocumento[$chave+1]['aedid'];
            if( $esdiddestino != EM_SOLICITACAO_REFORMULACAO ) break;
        }
    }*/

    $esdiddestino = EM_SOLICITACAO_REFORMULACAO;
    $atual = wf_pegarEstadoAtual( $docid );
   /* ver($atual, $esdiddestino, $aedid, $arrRetorno, d );
    if( $atual['esdid'] == EM_REFORMULACAO_PROCESSO ){
    	$aedid = '1295';
    } else {*/
	    $sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdidorigem = ".$atual['esdid']." and esdiddestino = ".$esdiddestino;
	    $aedid = $db->pegaUm($sql);
	    
	    if( empty($aedid) ) {
	    	$sql = "SELECT aeddscrealizar, aeddscrealizada FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdiddestino = $esdiddestino limit 1";
	    	$arAcoes = $db->pegaLinha( $sql );
	    	
		    $sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada, 
		  					aedcondicao, aedobs, aedposacao, aedvisivel, aedicone, aedcodicaonegativa) 
					VALUES ({$atual['esdid']}, $esdiddestino, '{$arAcoes['aeddscrealizar']}', '{$arAcoes['aeddscrealizada']}',
		  					'', '', '', false, '', false) RETURNING aedid";
		    
		    $aedid = $db->pegaUm( $sql );
	    }
    //}
    $arDados = array( "url" => $_SESSION['favurl'], "ptrid" => $ptrid );
	if(wf_alterarEstado( $docid, $aedid, 'Excluir Reformulação', $arDados )) {
		return $db->commit();
	} else {
		return false;
	}
}

function insereAnaliseTecnicaReformulacao( $url = null, $ptrid = null, $refid = null ){
	global $db;

	$refid = $refid ? $refid : 'refid';
	//verifica se existe ptridpai para o plano de trabalho passado como parametro,
	//caso não exista replica a analise para o ptrid atual.
	$ptridpai = $db->pegaUm("select ptridpai from emenda.planotrabalho where ptrid = $ptrid and ptrstatus = 'A'");
	$ptridpai = ( $ptridpai ? $ptridpai : $ptrid );
	$analote = $db->pegaUm("select max(analote) from emenda.analise where ptrid = $ptrid and anastatus = 'A' and anatipo = 'T'");
	$analote = !empty($analote) ? $analote + 1 : 1;

	$sql = "INSERT INTO emenda.analise(ptrid, uniid, refid, anatipo, analote,
				anadatainclusao, anaidpai, anastatus, anaposterior) 
			(SELECT $ptrid, uniid, $refid, anatipo, $analote, now(), anaidpai, anastatus, anaposterior
			 FROM emenda.analise
			 WHERE ptrid = $ptridpai
			    and anastatus = 'A' and anatipo = 'T')";

	$db->executar( $sql );
	return $db->commit();
}

function ativaPlanoTrabalhoFilho($ptrid){
	global $db;

	$arTipos = pegaTipoReformulacao( $ptrid );

	$arCodigo = array();
	foreach ($arTipos as $v) {
		array_push( $arCodigo, $v['codigo'] );
	}

	$pta = $db->pegaLinha("SELECT ptridpai, refid FROM emenda.planotrabalho WHERE ptrid = $ptrid");

	$ptridpai = $pta['ptridpai'];
	$refid = $pta['refid'];

	insereTiposReformulacao( $ptrid, $refid );

	$sql = "UPDATE emenda.ptminreformulacao SET refsituacao = 'T', refsituacaoreformulacao = 'F' WHERE refstatus ='A' and refid = $refid";
	$db->executar( $sql );
	
	$vigNull = $db->pegaUm("SELECT count(vigid) FROM emenda.ptvigencia WHERE ptrid = $ptrid and vigdatafim is null");
	if( (int)$vigNull > 0 ){
		$sql = "UPDATE emenda.ptvigencia SET vigstatus = 'I' WHERE ptrid = $ptrid";
		$db->executar( $sql );
	}
	
	if( $ptridpai ){
		$sql = "UPDATE emenda.planotrabalho SET ptrstatus = 'I', usucpfalteracao = '".$_SESSION['usucpf']."', ptrdataalteracao = now() WHERE ptrid = $ptridpai";
		$db->executar( $sql );

		$sql = "UPDATE emenda.planotrabalho SET ptrsituacao = 'A', usucpfalteracao = '".$_SESSION['usucpf']."', ptrdataalteracao = now() WHERE ptrid = $ptrid";
		$db->executar( $sql );
	}
	$db->commit();
	/*if($db->commit()){
		return true;
		} else {
		return false;
		}*/
}

function inativarParecerAnalise( $tipo, $ptrid ){
	global $db;

	$sql = "UPDATE emenda.analise set anadataconclusao = null, anaparecer = null, anasituacaoparecer = null, usucpf = null
			WHERE ptrid = $ptrid AND anatipo = '{$tipo}'
            and analote = (SELECT max(analote) 
						    				FROM emenda.analise 
						                    WHERE anatipo = '{$tipo}'
						                    	and anastatus = 'A' 
						                    	and ptrid = $ptrid)";

	$db->executar( $sql );
	$db->commit();
	if( $tipo == 'T' )
	$db->sucesso('principal/analiseTecnicaPTA');
	else
	$db->sucesso('principal/analiseMeritoPTA');
}

function inserirParecerAnalise( $dados ){
	global $db;
	
	//$sql = "DELETE FROM emenda.analise WHERE ptrid = {$dados["ptridAnalise"]} AND anatipo = 'T' AND uniid IS NOT NULL AND (anastatus ='I' or anastatus = 'A') and anadataconclusao is null";
	$sql = "UPDATE emenda.analise set anastatus = 'I'
			WHERE ptrid = {$dados["ptrid"]} AND anatipo = 'T'
            and analote = (SELECT max(analote) 
						    				FROM emenda.analise 
						                    WHERE anatipo = 'T' 
						                    	and ptrid = {$dados["ptrid"]})";
	$db->executar($sql);

	$sql = "SELECT max(analote) FROM emenda.analise WHERE ptrid = {$dados["ptrid"]} AND anatipo = 'T' AND uniid IS NOT NULL";
	$analote = $db->pegaUm( $sql );

	$analote = !empty($analote) ? $analote + 1 : 1;
	
	$arUnidades = ( $dados["ckb_unidade"] ? $dados["ckb_unidade"] : array() );

	foreach( $arUnidades as $chave=>$valor ){

		$sql = "INSERT INTO emenda.analise( ptrid, uniid, anatipo, analote, anaidpai, anadatainclusao , anastatus)
									VALUES( {$dados["ptrid"]}, {$valor}, 'T', {$analote}, null, null , 'A')";

		$db->executar($sql);

	}
	return $db->commit();
}

function salvaAnaliseTecnica( $dados ){

	global $db;

	/*if ( $dados["concluir"] == 'true' ){
		$concluir = ", anadataconclusao = 'now'";
		}*/
	
	if($dados["anasituacao"] != 'E') $dados["anadisponibilizardiligencia"] = 'N';
	
	$anaparecersiconv = ($dados["anaparecersiconv"] ? $dados["anaparecersiconv"] : $dados["anaparecer"]);
	$anaparecersiconv = strip_tags($anaparecersiconv);
	$anaparecersiconv = trim(str_ireplace('&nbsp;', '', $anaparecersiconv));

	$sql = "UPDATE
				emenda.analise 
			SET 
				anaparecer = '{$dados["anaparecer"]}',
				anaparecersiconv = '{$anaparecersiconv}',
				anadisponibilizardiligencia = '{$dados["anadisponibilizardiligencia"]}',
				anaposterior = '{$dados["anaposterior"]}',";

	if ($dados["anasituacao"]) $sql .= "anasituacaoparecer = '{$dados["anasituacao"]}',";

	$sql .= "	usucpf = '{$_SESSION["usucpforigem"]}',
				anadataconclusao = 'now'
			WHERE
				anaid = {$dados["anaid"]}";

	$db->executar( $sql );
	insereDadosAnaliseHistorico($dados["anaid"], $dados['anaparecer'], $dados["anasituacao"], $_SESSION["usucpf"]);


	/*if(salvaAnexoPorAnaid( $dados["anaid"] )){
		return true;
	} else {*/
		return $db->commit();
	//}
}

function excluirAnexoPorAnaid(){
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$sql = "DELETE FROM emenda.anexo where arqid=".$_REQUEST['arqid'];
	$db->executar($sql);
	$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqid'];
	$db->executar($sql);
	$db->commit();

	$file = new FilesSimec();
	$file->excluiArquivoFisico($_REQUEST['arqid']);
}

function salvaAnexoPorAnaid( $anaid ){
	global $db;

	$ptrid = !$_SESSION['emenda']['ptrid'] ? $_SESSION['emenda']['ptridAnalise'] : $_SESSION['emenda']['ptrid'];

	if(!$anaid || !$ptrid){
		return false;
	}
	$sql = "select
				anxid,
				anxdsc
			from 
				emenda.anexo 
			where 
				anaid = $anaid 
			and 
				ptrid = $ptrid";
	$anx = $db->pegaLinha($sql);

	$boCommit = false;

	if($anx['anxid'] && $_POST['anxdsc'] != $anx['anxdsc']){
		$sql = "UPDATE emenda.anexo SET anxdsc = '{$_POST['anxdsc']}' where anxid = {$anx['anxid']}";
		$db->executar($sql);
	}else if(!$anx['anxid'] && $_FILES["arquivo"]["name"]){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos	= array("ptrid"  => $ptrid,
						"anxdsc" => "'{$_POST['anxdsc']}'",
						"anaid" => $anaid
		);
		$file = new FilesSimec("anexo", $campos ,"emenda");
		$arquivoSalvo = $file->setUpload();
		$boCommit = true;
	}elseif($anx['anxid'] && $_FILES["arquivo"]["name"]){
		//$boCommit = false; // Somente 1 anexo é permitido
	}

	return $boCommit;
}

function listaAnaliseMerito( $ptrid, $habilitado ){

	global $db;

	$arPerfil = pegaPerfilArray( $_SESSION["usucpf"] );

	if ( !$db->testa_superuser() ){
		$possiveis = pegaUnidadeAnalisador($_SESSION["usucpf"], ANALISTA_MERITO);
	}else{
		$possiveis = 'superuser';
	}
	
	$acao = ($habilitado && verificaPermissaoPerfil( 'analise', 'boolean' )) ? "<img src=\"/imagens/alterar.gif\" onclick=\"alteraAnaliseMerito( ' || ana.anaid || ' );\" style=\"cursor:pointer;\"/>" : "<img src=\"/imagens/alterar_01.gif\" />";
	if( in_array( SUPER_USUARIO, $arPerfil ) ){
		$acao .= " <img src=\"/imagens/excluir.gif\" onclick=\"excluirAnaliseDados( ' || ana.anaid || ' );\" style=\"cursor:pointer;\"/>";
	}
	$sql = "SELECT
				ana.uniid as id,
				CASE WHEN ana.analote = (SELECT max(analote) 
    					FROM emenda.analise 
                        WHERE anatipo = 'M' 
                        	and anastatus = 'A'
                        	and ptrid = ana.ptrid) THEN 
					CASE WHEN anx.anxid is not null 
						 			THEN '{$acao} <img border=\"0\" onclick=\"window.location.href=\'?modulo=principal/manterAnexos&acao=A&download=S&arqid=' || anx.arqid || '\'\" style=\"cursor:pointer\" id=\"img_anexo_' || anx.arqid || '\" title=\"Anexo\" src=\"../imagens/anexo.gif\"/>'
						 			ELSE '{$acao}' 
					END
				ELSE '<img src=\"/imagens/alterar_01.gif\" />' END as acao,
				analote as codigo, 
				CASE WHEN anasituacaoparecer = 'F' THEN 'Favorável'
					 WHEN anasituacaoparecer = 'D' THEN 'Desfavorável'
					 WHEN anasituacaoparecer = 'E' THEN 'Em diligência'
					 ELSE 'Aguardando' END as analise, 
				CASE WHEN anaparecer <> '' THEN anaparecer ELSE 'Não Informado' END as parecer,
				unisigla as unidade,
				CASE WHEN anadataconclusao is not null 
					 THEN '<b>' || to_char(anadataconclusao, 'DD/MM/YYYY HH24:MI:SS') || '</b>' 
					 ELSE '<span style=\"color:red;\"><b> Pendente </b></span>' END as concluido,
				CASE WHEN ana.usucpf <> '' 
					 THEN usunome 
					 ELSE 'Não Informado' END as analisado
			FROM 
				emenda.analise ana
			LEFT JOIN
				seguranca.usuario usu ON usu.usucpf = ana.usucpf
			INNER JOIN
				emenda.unidades uni ON uni.uniid = ana.uniid
			LEFT JOIN
				emenda.anexo anx ON anx.anaid = ana.anaid 
			WHERE 
				anatipo = 'M' AND ana.ptrid = {$ptrid}
				and ana.anastatus = 'A'
			ORDER BY
				analote desc";

	$dadosAnalise = $db->carregar( $sql );


	if( $dadosAnalise ){

		print '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">'
		. '	<tr>'
		. '		<td class="subtitulocentro">Ação</td>'
		. '		<td class="subtitulocentro">Código</td>'
		. '		<td class="subtitulocentro">Análise</td>'
		. '		<td class="subtitulocentro" width="550px">Parecer</td>'
		. '		<td class="subtitulocentro">Unidade</td>'
		. '		<td class="subtitulocentro">Analisado Por</td>'
		. '		<td class="subtitulocentro">Atualizado em</td>'
		. ' </tr>';

		for( $i = 0; $i < count( $dadosAnalise ); $i++ ){

			if ( $possiveis != 'superuser' ){
				$dadosAnalise[$i]["acao"] = ( in_array( ANALISTA_MERITO, $arPerfil) && in_array($dadosAnalise[$i]["id"],$possiveis) ) ? $dadosAnalise[$i]["acao"] : '<img src="/imagens/alterar_01.gif"/>';
			}

			$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";

			print '<tr bgColor="'.$cor.'">'
			. '	<td align="justify">'. $dadosAnalise[$i]["acao"] .'</td>'
			. '	<td align="center">'. $dadosAnalise[$i]["codigo"] .'</td>'
			. '	<td align="center">'. $dadosAnalise[$i]["analise"] .'</td>'
			. '	<td align="justify">'. $dadosAnalise[$i]["parecer"] .'</td>'
			. '	<td align="justify">'. $dadosAnalise[$i]["unidade"]  .'</td>'
			. '	<td align="center">'. $dadosAnalise[$i]["analisado"] .'</td>'
			. '	<td align="center">'. $dadosAnalise[$i]["concluido"] .'</td>';

		}

	}else{

		print '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">'
		. '	<tr><td colspan="7" style="color:red;" align="center"> Não foram encontratos registros. </td></tr>';

	}

	print '</table>';

}

function listaAnaliseTecnica( $ptrid, $habilitado ){

	global $db;

	$arPerfil = pegaPerfilArray( $_SESSION["usucpf"] );

	if( in_array( ANALISTA_TECNICO, $arPerfil) || in_array( GESTOR_EMENDAS, $arPerfil) || in_array( ADMINISTRADOR_INST, $arPerfil) ){
		$perfilHabilita = true;
	} else {
		$perfilHabilita = false;
	}
	
	$estadoAtual = pegarEstadoAtual( $ptrid );

	if ( !$db->testa_superuser() ){
		$arPossiveis = array(ANALISTA_TECNICO, ADMINISTRADOR_INST);
		$possiveis = pegaUnidadeAnalisador($_SESSION["usucpf"], $arPossiveis);
	}else{
		$possiveis = 'superuser';
	}
	$acao = ($habilitado && verificaPermissaoPerfil('analise', 'boolean') ) ? "<img src=\"/imagens/alterar.gif\" onclick=\"alteraAnaliseTecnica( ' || ana.anaid || ' );\" style=\"cursor:pointer;\"/>" : "<img src=\"/imagens/alterar_01.gif\" />";
	
	$ptrcod = pegaNumPTA( $ptrid );

	if( !empty($ptrcod) ) $filtro = "AND ana.ptrid in (select ptrid from emenda.planotrabalho where ptrcod = ".$ptrcod.")";
	else $filtro = " and ana.ptrid = $ptrid";

	$sql = "SELECT
				ana.uniid as id,
				CASE WHEN ana.analote = (SELECT max(analote) 
    					FROM emenda.analise 
                        WHERE anatipo = 'T' 
                        	and ptrid = ana.ptrid
                        	and anastatus = 'A') THEN 
					CASE WHEN anx.anxid is not null 
						 			THEN '{$acao} <img border=\"0\" style=\"cursor:pointer\" title=\"Anexo\" src=\"../imagens/anexo.gif\"/>'
						 			ELSE '{$acao}' 
					END
				ELSE '<img src=\"/imagens/alterar_01.gif\" />    <img src=\"/imagens/consultar.gif\" onclick=\"mostraParecer( ' || ana.anaid || ' );\" style=\"cursor:pointer;\"/>' END as acao,	
				analote as codigo, 
				CASE WHEN anasituacaoparecer = 'F' THEN 'Aprovado'
					 WHEN anasituacaoparecer = 'D' THEN 'Indeferido'
					 WHEN anasituacaoparecer = 'E' THEN 'Em diligência'
					 ELSE 'Aguardando' END as analise, 
				CASE WHEN anaparecer <> '' THEN anaparecer ELSE 'Não Informado' END as parecer,
				unisigla as unidade,
				CASE WHEN anadataconclusao is not null 
					 THEN '<b>' || to_char(anadataconclusao, 'DD/MM/YYYY HH24:MI:SS') || '</b>' 
					 ELSE '<span style=\"color:red;\"><b> Pendente </b></span>' END as concluido,
				CASE WHEN ana.usucpf <> '' 
					 THEN usunome 
					 ELSE 'Não Informado' END as analisado,
				(CASE WHEN ptr.ptridpai is null THEN
					'Original'
				ELSE
					'Reformulação'
				END) as tipoanalise,
				ptr.ptrid,
				ana.anaid,
				case when anastatus = 'A' then 'Ativo' else 'Inativo' end as status
			FROM 
				emenda.analise ana
				LEFT JOIN seguranca.usuario usu ON usu.usucpf = ana.usucpf
				INNER JOIN emenda.unidades uni ON uni.uniid = ana.uniid 
				LEFT JOIN emenda.anexo anx ON anx.anaid = ana.anaid
				INNER JOIN emenda.planotrabalho ptr ON ptr.ptrid = ana.ptrid
			WHERE 
				anatipo = 'T' 
				$filtro
				--AND ana.anastatus = 'A'
			group by
            	id, acao, codigo, ana.anasituacaoparecer, ana.anaparecer, uni.unisigla,
                ana.anadataconclusao, ana.usucpf, usu.usunome, ptr.ptridpai, ptr.ptrid,
                ana.anaid, ana.anastatus, ana.anadatainclusao
			ORDER BY
				ptr.ptrid desc, codigo desc, ana.anadatainclusao DESC";

				$dadosAnalise = $db->carregar( $sql );
				$ptaPai = pegaPaiPTA( $ptrid );

				if( $dadosAnalise ){

					print '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">'
					. '	<tr>'
					. '		<td class="subtitulocentro">Ação</td>'
					. '		<td class="subtitulocentro">Código</td>'
					. '		<td class="subtitulocentro">Análise</td>'
					. '		<td class="subtitulocentro" width="50%">Parecer</td>'
					. '		<td class="subtitulocentro">Unidade</td>'
					. '		<td class="subtitulocentro">Analisado Por</td>'
					. '		<td class="subtitulocentro">Atualizado em</td>'
					. '		<td class="subtitulocentro">Situação</td>'
					. ' </tr>';
					$arCPF = array('', '');
				
					for( $i = 0; $i < count( $dadosAnalise ); $i++ ){
						
						if( in_array($_SESSION['usucpf'], $arCPF) ){
							$excluir = "<a href=\"#\" onclick=\"excluirAnalise(".$dadosAnalise[$i]["anaid"].");\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"> </a>";
						}
							
						if ( $possiveis != 'superuser' && !$perfilHabilita ){
							$dadosAnalise[$i]["acao"] = ( $perfilHabilita && in_array($dadosAnalise[$i]["id"],$possiveis) ) ? $dadosAnalise[$i]["acao"] : '<img src="/imagens/alterar_01.gif"/>';
						}
							
						$acoes = $dadosAnalise[$i]["acao"];
						if( !empty($ptaPai) && $dadosAnalise[$i]["tipoanalise"] == 'Original' ){
							$acoes = "<img src=\"/imagens/alterar_01.gif\" />    <img src=\"/imagens/consultar.gif\" onclick=\"mostraParecer( {$dadosAnalise[$i]["anaid"]} );\" style=\"cursor:pointer;\"/>";
						}
						
						if( $dadosAnalise[$i]["tipoanalise"] == 'Reformulação' ){							
							$arrTipo = pegaTipoReformulacao( $dadosAnalise[$i]['ptrid'] );
							$tipoanalise = $arrTipo[0]['descricao'];
						} else {
							$tipoanalise = 'Vigencia do PTA original';							
						}
							
						$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";
						
						$boRefExc = $db->pegaUm("select count(pt.refid) from emenda.ptminreformulacao pt 
												where 	
													pt.ptrid = {$dadosAnalise[$i]["ptrid"]}
													and pt.refsituacaoreformulacao = 'E'");
						
						if( $boRefExc == 0 ){
							print '<tr bgColor="'.$cor.'">'
								. '	<td align="justify">'. $acoes.' '.$excluir .'</td>'
								. '	<td align="center">'. $dadosAnalise[$i]["codigo"] .'</td>'
								. '	<td align="center">'. $dadosAnalise[$i]["analise"].'</td>'
								. '	<td align="justify">'. delimitador($dadosAnalise[$i]["parecer"]) .'</td>'
								. '	<td align="justify">'. $dadosAnalise[$i]["unidade"]  .'</td>'
								. '	<td align="center">'. $dadosAnalise[$i]["analisado"] .'</td>'
								. '	<td align="center">'. $dadosAnalise[$i]["concluido"] .'</td>'
								. '	<td align="center">'. $dadosAnalise[$i]["status"] .'</td>';
						}
					}

				}else{

					print '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">'
					. '	<tr><td colspan="7" style="color:red;" align="center"> Não foram encontratos registros. </td></tr>';

				}

				print '</table>';

}
function delimitador($texto){
	if(strlen($texto) > 500){
		if( strpos($texto, '</table>') ){
			$texto = substr($texto,0, strpos($texto, '</table>') + 8).'...';
		} else {
			$texto = substr($texto,0,500).'...';			
		}
	}
	return $texto;
}
function salvaAnaliseDados( $dados ){

	global $db;

	/*if ( $dados["concluir"] == 'true' ){
		$concluir = ", anadataconclusao = 'now'";
		}*/

	$sql = "UPDATE
				emenda.analise 
			SET

				usucpf = '{$_SESSION["usucpf"]}',
				anadataconclusao = 'now'
			WHERE
				anaid = {$dados["anaid"]}";

	$db->executar( $sql );

	$sql = "DELETE FROM emenda.analise WHERE ptrid = {$dados["ptrid"]} AND anatipo = 'M' AND uniid IS NOT NULL AND (anastatus ='I' or anastatus = 'A') AND anaidpai = {$dados["anaid"]} and anadataconclusao is null";
	$db->executar($sql);

	$sql = "SELECT max(analote) FROM emenda.analise WHERE ptrid = {$dados["ptrid"]} AND anatipo = 'M' AND uniid IS NOT NULL";
	$analote = $db->pegaUm( $sql );

	$analote = !empty($analote) ? $analote + 1 : 1;


	$arUnidades = ( $dados["uniid"] ? $dados["uniid"] : array() );

	foreach( $arUnidades as $chave=>$valor ){

		$sql = "INSERT INTO emenda.analise( ptrid, uniid, anatipo, analote, anaidpai, anadatainclusao , anastatus)
									VALUES( {$dados["ptrid"]}, {$valor}, 'M', {$analote}, {$dados["anaid"]}, 'now' , 'I')";

		$db->executar($sql);

	}

	/*if(salvaAnexoPorAnaid( $dados["anaid"] )){
		return true;
	} else {*/
		return $db->commit();
	//}
}
function salvaAnaliseDados2011( $dados ){

	global $db;

	/*if ( $dados["concluir"] == 'true' ){
		$concluir = ", anadataconclusao = 'now'";
		}*/

	/*$sql = "UPDATE
				emenda.analise 
			SET

				usucpf = '{$_SESSION["usucpf"]}',
				anadataconclusao = 'now'
			WHERE
				ptrid = {$dados["ptridAnalise"]}";

	$db->executar( $sql );*/
	
	$ptrid = $_SESSION["emenda"]["ptridAnalise"];
	$sql = "DELETE FROM emenda.analisehistorico WHERE anaid in (SELECT anaid FROM emenda.analise WHERE ptrid = {$ptrid} AND anatipo = 'T' AND uniid IS NOT NULL AND (anastatus ='I' or anastatus = 'A') and anadataconclusao is null)";
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.analise WHERE ptrid = {$ptrid} AND anatipo = 'T' AND uniid IS NOT NULL AND (anastatus ='I' or anastatus = 'A') and anadataconclusao is null";
	$db->executar($sql);

	$sql = "SELECT max(analote) FROM emenda.analise WHERE ptrid = {$ptrid} AND anatipo = 'T' AND uniid IS NOT NULL";
	$analote = $db->pegaUm( $sql );

	$analote = !empty($analote) ? $analote + 1 : 1;


	$arUnidades = ( $dados["uniid"] ? $dados["uniid"] : array() );

	foreach( $arUnidades as $chave=>$valor ){

		$sql = "INSERT INTO emenda.analise( ptrid, uniid, anatipo, analote, anaidpai, anadatainclusao , anastatus)
									VALUES( {$ptrid}, {$valor}, 'T', {$analote}, null, null , 'I')";

		$db->executar($sql);

	}
	return $db->commit();
}

function listaAnaliseDados( $ptrid ){

	global $db;

	$residPTA = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
	$arPerfil = array(ADMINISTRADOR_INST, SUPER_USUARIO, GESTOR_EMENDAS, ANALISTA_FNDE);

	$arPerfil = pegaPerfilArray( $_SESSION["usucpf"] );
	
	$estadoAtual = pegarEstadoAtual( $ptrid );
	
	if( emenda_possuiPerfil($arPerfil) && verificaPermissaoPerfil( 'analise', 'boolean' ) ){
		if( in_array( ADMINISTRADOR_INST, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) || in_array( ANALISTA_FNDE, $arPerfil ) ){
			$residPossiveis = recuperaResponsaveis( $_SESSION["usucpf"], $arPerfil );

			if ( in_array( $residPTA, $residPossiveis ) || in_array( SUPER_USUARIO, $arPerfil ) ){
				if( in_array( $estadoAtual, array(VINCULACAO_UNIDADES_GESTORAS_IMPOSITIVO, EM_ANALISE_DE_DADOS)) ){
					$acao = "<img src=\"/imagens/alterar.gif\" onclick=\"alteraAnaliseDados( ' || ana.anaid || ' );\" style=\"cursor:pointer;\"/>";
				} else {
					$acao = "<img src=\"/imagens/alterar_01.gif\" />";
				}
			}else{
				if( empty($residPossiveis) ){
					$acao = "<img src=\"/imagens/alterar.gif\" onclick=\"alteraAnaliseDados( ' || ana.anaid || ' );\" style=\"cursor:pointer;\"/>";
				} else{
					$acao = "<img src=\"/imagens/alterar_01.gif\" />";
				}
			}
		} else {
			if( in_array( $estadoAtual, array(VINCULACAO_UNIDADES_GESTORAS_IMPOSITIVO, EM_ANALISE_DE_DADOS)) ){
				$acao = "<img src=\"/imagens/alterar.gif\" onclick=\"alteraAnaliseDados( ' || ana.anaid || ' );\" style=\"cursor:pointer;\"/>";
			} else {
				$acao = "<img src=\"/imagens/alterar_01.gif\" />";
			}
		}
	} else {
		$acao = "<img src=\"/imagens/alterar_01.gif\" />";
	}

	if( in_array( SUPER_USUARIO, $arPerfil ) ){
		$acao .= " <img src=\"/imagens/excluir.gif\" onclick=\"excluirAnaliseDados( ' || ana.anaid || ' );\" style=\"cursor:pointer;\"/>";
	}

	$sql = "SELECT
				ana.anaid as id,
				ana.analote as codigo,
				CASE WHEN ana.analote = (SELECT max(analote) 
    					FROM emenda.analise 
                        WHERE anatipo = 'D' 
                        	and ptrid = ana.ptrid) THEN
					CASE WHEN anx.anxid is not null 
						 			THEN '{$acao} <img border=\"0\" onclick=\"window.location.href=\'?modulo=principal/manterAnexos&acao=A&download=S&arqid=' || anx.arqid || '\'\" style=\"cursor:pointer\" id=\"img_anexo_' || anx.arqid || '\" title=\"Anexo\" src=\"../imagens/anexo.gif\"/>'
						 			ELSE '{$acao}' 
					END
				 ELSE '<img src=\"/imagens/alterar_01.gif\" />' END as acao,


				CASE WHEN ana.anadataconclusao is not null 
					 THEN '<b>' || to_char(ana.anadataconclusao, 'DD/MM/YYYY HH24:MI:SS') || '</b>' 
					 ELSE '<span style=\"color:red; font-weight:bold;\">Pendente</span>' END as concluido,
				CASE WHEN ana.usucpf <> '' THEN usunome ELSE 'Não informado' END as analisado
			FROM 
				emenda.analise ana
			LEFT JOIN
				seguranca.usuario usu ON usu.usucpf = ana.usucpf
			LEFT JOIN
				emenda.anexo anx ON anx.anaid = ana.anaid
			WHERE 
				ana.anatipo = 'D' AND 
				ana.ptrid = {$ptrid}
			ORDER BY
				ana.analote DESC";

	$dadosAnalise = $db->carregar( $sql );

	if ( $dadosAnalise ){

		print '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">'
		. '	<tr>'
		. '		<td class="subtitulocentro">Ação</td>'
		. '		<td class="subtitulocentro">Código</td>'

		. '		<td class="subtitulocentro">Unidades</td>'
		. '		<td class="subtitulocentro">Vinculado Por</td>'
		. '		<td class="subtitulocentro">Atualizado em</td>'
		. ' </tr>';

		for( $i = 0; $i < count( $dadosAnalise ); $i++ ){

			$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";

			$sql = "SELECT
						unisigla 
					FROM 
						emenda.unidades uni
					INNER JOIN
						emenda.analise ana ON ana.uniid = uni.uniid 
					WHERE 
						anaidpai = {$dadosAnalise[$i]["id"]}";

			$dadosUnidades = $db->carregarColuna( $sql );

			$unidades = $dadosUnidades ? implode(", ", $dadosUnidades) : 'Não informado';

			print '<tr bgColor="'.$cor.'">'
			. '	<td align="center">'. $dadosAnalise[$i]["acao"] .'</td>'
			. '	<td align="center">'. $dadosAnalise[$i]["codigo"] .'</td>'

			. '	<td align="justify">'. $unidades .'</td>'
			. '	<td align="center">'. $dadosAnalise[$i]["analisado"] .'</td>'
			. '	<td align="center">'. $dadosAnalise[$i]["concluido"] .'</td>';

		}
	}else{

		print '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">'
		. '	<tr><td colspan="7" style="color:red;" align="center"> Não foram encontratos registros. </td></tr>';

	}

	print '</table>';

}

function retornaDadosEmailAnalise($ptrid, $situacao, $tipo, $status = ''){
	global $db;

	$status = ($status ? $status = " and anastatus = 'A' " : '');

	$sql = "SELECT
				ana.ptrid,
			    ptr.enbid,
			    (SELECT to_char(max(anadataconclusao),'DD/MM/YYYY HH24:MI:SS') FROM emenda.analise WHERE ptrid = ana.ptrid) as dataconclusao,
			    ana.anaparecer
			FROM
				emenda.analise ana
			    inner join emenda.planotrabalho ptr
			    	on ptr.ptrid = ana.ptrid
			WHERE
				ana.ptrid = $ptrid
			    and ana.anasituacaoparecer = '$situacao'
			    and ana.anatipo = '$tipo' 
			    $status
			    and ana.analote = (SELECT max(analote) FROM emenda.analise 
			    					WHERE ptrid = ana.ptrid
			                              and anasituacaoparecer = '$situacao'
			                              and anatipo = '$tipo' 
			                              $status)";

			                              return $db->carregar($sql);
}

function pegaNumPTA( $ptrid ){
	global $db;

	$sql = "SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = $ptrid and ptrstatus = 'A'";
	return $db->pegaUm( $sql );
}

function enviaEmailEntidade($ptrid, $tipo, $situacao){
	global $db;

	$arDados = retornaDadosEmailAnalise($ptrid, $situacao, $tipo);

	$ptrcod = pegaNumPTA( $ptrid );

	if($arDados){
		foreach ($arDados as $key => $valor) {
			if( $parecer == "" ){
				$parecer = "Parecer ".($key+1).": ".$valor['anaparecer'];
				$dataConclusao = $valor['dataconclusao'];
				$enbid = $valor['enbid'];
			} else {
				$parecer = $parecer . "<br>Parecer ".($key+1).": ".$valor['anaparecer'];
			}
		}

		if( $tipo == 'D' || $tipo == 'M' ){
			if( $situacao == 'E' ){
				$strMensagem = "Seu plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi analisado em $dataConclusao. A análise de dados/de mérito<br>teve posicionamento \"Em diligência\". Segue(m) parecer(es) para atendimento. O prazo para <br>atendimento é de 24 HORAS, contados do recebimento deste email.<br>$parecer";
			}
			if($tipo == 'M'){
				if($situacao == 'D'){
					$strMensagem = "Seu plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi analisado em $dataConclusao. A análise de mérito/técnica<br>teve posicionamento \"Desfavorável/Indeferido\". Segue(m) parecer(es) para conhecimento.<br>".$parecer;
				}
			}

		} else {
			if( $situacao == 'E' ){
				$strMensagem = "Seu plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi analisado em $dataConclusao. A análise técnica teve <br>posicionamento \"Em diligência\". Segue(m) parecer(es) para atendimento. O prazo para <br>atendimento é de 15 dias corridos, contados do recebimento deste email.<br>$parecer";
			}
			if($situacao == 'D'){
				$strMensagem = "Seu plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi analisado em $dataConclusao. A análise de mérito/técnica<br>teve posicionamento \"Desfavorável/Indeferido\". Segue(m) parecer(es) para conhecimento.<br>".$parecer;
			}
		}

		if($tipo == 'M' || $tipo == 'T'){
			if($situacao == 'F'){
				$strMensagem = "Seu plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi analisado em $dataConclusao e teve parecer<br>de mérito/técnico \"Favorável/Aprovado\".";
			}
		}

		$sql = "SELECT
						usu.usuemail
					FROM 
						emenda.usuarioresponsabilidade usr
	                    inner join seguranca.usuario usu
	                    	on usu.usucpf = usr.usucpf
					WHERE 
						usr.enbid = $enbid
						and usr.rpustatus = 'A'";

		$strEmailTo = $db->carregarColuna($sql);
		$strAssunto = "SIMEC - Emenda";

		return enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);
	}
	return false;
}

/**
 * Envia email para os responsavel pelo empenho da analise
 *
 * @param integer $ptrid
 */
function enviaEmailSituacaoEmpenho($ptrid){
	global $db;

	$sql = "SELECT DISTINCT
			    usu.usuemail
			FROM
				emenda.usuarioresponsabilidade usr
			    inner join seguranca.usuario usu
			    	on usu.usucpf = usr.usucpf
			WHERE
				usr.pflcod = ".EMPENHO."
			    and usr.rpustatus = 'A'";

	$strEmailTo = $db->carregarColuna($sql);

	$ptrcod = pegaNumPTA( $ptrid );

	$strMensagem = "O plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi disponibilizado para empenho em ".date('d/m/Y h:i:s').".";
	$strAssunto = 'SIMEC - Emenda';
	return enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);
}

function enviaEmailSituacaoConvenio($ptrid){
	global $db;

	$sql = "SELECT DISTINCT
			    usu.usuemail
			FROM
				emenda.usuarioresponsabilidade usr
			    inner join seguranca.usuario usu
			    	on usu.usucpf = usr.usucpf
			WHERE
				usr.pflcod = ".CONVENIO."
			    and usr.rpustatus = 'A'";

	$strEmailTo = $db->carregarColuna($sql);

	$ptrcod = pegaNumPTA( $ptrid );

	$strMensagem = "O plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi disponibilizado para convênio em ".date('d/m/Y h:i:s').".";
	$strAssunto = 'SIMEC - Emenda';
	return enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);
}

/**
 * envia email para todos os usuarios responsaveis pelas unidades
 *
 * @param string $url
 * @param integer $ptrid
 */
function enviaEmailPerfilUnidade($url, $ptrid, $tipo = null){
	global $db;

	$strAssunto = "SIMEC - Emenda";
	if( strstr($url, "emenda.php?modulo=principal/analiseDadosPTA&acao=A") || $tipo == 'M' ){
		$pflcod = ANALISTA_MERITO;
		$tipo = 'M';
	}
	if( strstr($url, "emenda.php?modulo=principal/informacoesGerais&acao=A") || $tipo == 'T' ){
		$pflcod = ANALISTA_TECNICO;
		$tipo = 'T';
	}
	if( strstr($url, "emenda.php?modulo=principal/minutaConvenioDOU&acao=A") || $tipo == 'T' ){
		$pflcod = ANALISTA_TECNICO;
		$tipo = 'T';
	}

	$ptrcod = pegaNumPTA( $ptrid );

	$strMensagem = "O plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi enviado para análise de mérito/técnica deste setor em ".date('d/m/Y h:i:s').".";

	$sql = "SELECT DISTINCT
			    usu.usuemail
			FROM
				emenda.analise ana
			    inner join emenda.usuarioresponsabilidade usr
			    	on usr.uniid = ana.uniid
			        and usr.rpustatus = 'A'
			        and usr.pflcod = $pflcod
			    inner join seguranca.usuario usu
			    	on usu.usucpf = usr.usucpf
			WHERE
				ana.ptrid = $ptrid
			    and ana.anatipo = '$tipo'
			    and ana.analote = (SELECT max(analote) FROM emenda.analise
							WHERE ptrid = ana.ptrid
						and anatipo = '$tipo'
						and anastatus = 'A')";

	$strEmailTo = $db->carregarColuna($sql);

	return enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);
}

function enviaEmailEntidadeConvenio( $url, $ptrid, $mailid = null ){
	global $db;

	$sql = "SELECT
		 	ptmc.mailtxtconteudo,
            ptr.enbid
		FROM 
			emenda.ptmail ptmc
            inner join emenda.planotrabalho ptr
            	on ptr.ptrid = ptmc.ptrid
		WHERE
			ptmc.ptrid = $ptrid
            and ptr.ptrstatus = 'A'";

	$arDados = $db->pegaLinha($sql);

	$arquivos = array();
	if( $mailid ){

		$sql = "SELECT
					anx.arqid,
                    arq.arqnome,
                    arq.arqextensao
				FROM
					emenda.anexo anx
                    inner join public.arquivo arq
                    	on arq.arqid = anx.arqid
                        and arq.arqstatus = 'A'
				WHERE
					anx.mailid = {$mailid}";

		$arq = $db->carregar($sql);
		$arq = $arq ? $arq : array();

		foreach ($arq as $v) {
			$caminho = APPRAIZ."arquivos/emenda/".floor($v['arqid']/1000).'/'.$v['arqid'];
			$filename = str_replace(" ", "_", $v['arqnome'].'.'.$v['arqextensao']);
			$arquivo = array("nome" => $filename,
	    			         "arquivo" => $caminho );

			$arquivos[] = $arquivo;
		}
	}
	$strMensagem = html_entity_decode( $arDados['mailtxtconteudo'] );

	$enbid = $arDados['enbid'];

	$sql = "SELECT
					usu.usuemail
				FROM 
					emenda.usuarioresponsabilidade usr
                    inner join seguranca.usuario usu
                    	on usu.usucpf = usr.usucpf
				WHERE 
					usr.enbid = '{$enbid}'
					and usr.rpustatus = 'A'";

	$strEmailTo = $db->carregarColuna($sql);
	$strEmailTo = $strEmailTo ? $strEmailTo : array();

	$strAssunto = "SIMEC - Emenda";
	//include_once APPRAIZ . 'includes/Email.php';
	//$boEmail = new Email();

	//$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
	if( empty($strEmailTo[0]) ){
		echo 'Não foi possível enviar e-mail: destinatário não informado.';
		die;
	} else {
		//$retorno = $boEmail->enviar($strEmailTo, $strAssunto, $strMensagem, $arquivos, false, true, array(), true, $remetente);
		$retorno = enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo, $arquivos);
	}

	if( $retorno ){
		$sql = "UPDATE
				  emenda.planotrabalho  
				SET 
				  ptrimprimirpta = true
				 
				WHERE 
				  ptrid = $ptrid";

		$db->executar($sql);

		if( $mailid ){
			$sql = "UPDATE
					  emenda.ptmail  
					SET 
					  	usucpf = '".$_SESSION['usucpf']."',
					  	maildataenvio = now()
					 
					WHERE 
					 	mailid = ".$mailid;
			$db->executar($sql);
			$db->commit();
			echo $retorno;
		} else {
			return $retorno;
		}
	} else {
		if( !empty($mailid) ){
			echo $retorno;
		} else {
			return $retorno;
		}
	}
}

function enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo, $anexos = array()){
	global $db;

	$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");

	if( $strEmailTo ){
		if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" ) return true;
			$strEmailTo = array('thiago.barbosa@mec.gov.br', 'wesley.silva@mec.gov.br');
			return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', $anexos);
		} else {
			return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', $anexos);
		}
	} else {
		return true;
	}
}

function buscaDadosAnaliseDados( $anaid ){

	global $db;

	$sql = "SELECT anasituacaoparecer, anaparecer
			FROM emenda.analise 
			WHERE anaid = {$anaid}";

	$dados = $db->carregar( $sql );

	$sql = "SELECT uniid
			FROM emenda.analise 
			WHERE anaidpai = {$anaid}";

	$dados["unidades"] = $db->carregarColuna( $sql );

	return $dados;

}

function buscaResidAdmAnalise( $ptrid, $usucpf ){

	global $db;

	$residPTA = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");

	$perfil = pegaPerfil( $usucpf );

	$residPossiveis = recuperaResponsaveis( $_SESSION["usucpf"] );
	if ( (( $perfil == ADMINISTRADOR_INST ) && in_array( $residPTA, $residPossiveis )) || $db->testa_superuser() ){
		return true;
	}else{
		return false;
	}
}

function salvarAnaliseGestor( $dados ){
	$dados['ptrnumprocessoempenho'] = str_replace(".","", $dados['ptrnumprocessoempenho']);
	$dados['ptrnumprocessoempenho'] = str_replace("/","", $dados['ptrnumprocessoempenho']);
	$dados['ptrnumprocessoempenho'] = str_replace("-","", $dados['ptrnumprocessoempenho']);
	$dados['ptrnumdocumenta'] = str_replace("/","", $dados['ptrnumdocumenta']);
	$dados['ptrnumdocumenta'] = str_replace("-","", $dados['ptrnumdocumenta']);

	global $db;
	$sql = "UPDATE emenda.planotrabalho SET ptrnumprocessoempenho = '{$dados["ptrnumprocessoempenho"]}', ptrnumdocumenta = '{$dados["ptrnumdocumenta"]}' WHERE ptrid = {$dados["ptrid"]}";
	$db->executar( $sql );

	//Início -- Salva / Atualiza as Unidades que realizarão a Análise Técnica do PTA
	$arrUnidades = $dados["ckb_unidade"];
	$arCheck = ($dados['check'] ? $dados['check'] : array());

	if($arrUnidades && is_array($arrUnidades) ){

		$sql = "delete from emenda.analise where ptrid = {$dados["ptrid"]} and anatipo = 'T' and anastatus = 'A' and uniid not in (".implode(",",$arrUnidades).") and anadataconclusao is null";
		$db->executar($sql);

		$analote = $db->pegaUm("select max(analote) from emenda.analise where ptrid = {$dados["ptrid"]} and anastatus = 'A' and anatipo = 'T'");
		$analote = !empty($analote) ? $analote + 1 : 1;

		foreach($arrUnidades as $unidade){

			$sql = "select anaid
					from emenda.analise 
					where ptrid = {$dados["ptrid"]} 
					and anatipo = 'T' 
					and uniid = {$unidade}
					and anastatus = 'A'
					and anadataconclusao is null";

			$anaid = $db->pegaUm($sql);

			if( empty($anaid) ){
				$sqlI = "insert into emenda.analise
						(ptrid, uniid, anatipo, analote, anadatainclusao, anastatus) 
					values 
						({$dados["ptrid"]}, {$unidade}, 'T', {$analote}, now(), 'A');";

				$db->executar($sqlI);
			}

		}
	} else {
		if( $arCheck ){
			$sql = "delete from emenda.analise where ptrid = {$dados["ptrid"]} and anatipo = 'T' and anastatus = 'I' and uniid in (".implode(",", $arCheck).")";
			$db->executar($sql);
		}
	}
	//Fim -- Salva / Atualiza as Unidades que realizarão a Análise Técnica do PTA

	$db->commit();
	$db->sucesso( 'principal/informacoesGerais', '' );
}

function insereDadosAnaliseHistorico($anaid, $anaparecer, $anasituacaoparecer, $usucpf){
	global $db;

	$sql = "INSERT INTO
			  emenda.analisehistorico(
			  anaid,
			  anaparecer,
			  anasituacaoparecer,
			  anadatainclusao,
			  usucpf
			) 
			VALUES (
			$anaid,
			  '$anaparecer',
			  '$anasituacaoparecer',
			  'now()',
			  '$usucpf'
			)";

			$db->executar($sql);
}

/*********************************************************/
/*************** FUNÇÕES DO WORKFLOW *********************/
/*********************************************************/

/*function validaPTAEstadoRetorno($ptrid){
	global $db;

	$sql = "SELECT
				ptrestadoretorno				  	
			FROM 
			  	emenda.planotrabalho
			WHERE
				ptrid = $ptrid";

	if( !empty($ptrid) ){
		$retorno = $db->pegaUm($sql);
		if( empty( $retorno ) ){
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

function validaEnvioPTAAnaliseTecnica( $url = null, $ptrid = null ){
	//if( validaPTAEstadoRetorno($ptrid) ){
		//return false;
		//} else {
	return true;
	//}
}

function alteraEstadoRetornoPTA( $url, $ptrid  ){
	global $db;

	$sql = "UPDATE
			  	emenda.planotrabalho  
			SET 
			  	ptrestadoretorno = null
			WHERE 
			  	ptrid = $ptrid";

	$db->executar($sql);

	$sql = "SELECT max(analote) FROM emenda.analise WHERE ptrid = {$ptrid} AND anatipo = 'T'";
	$analote = $db->pegaUm( $sql );

	$analote = !empty($analote) ? $analote + 1 : 1;

	$sql = "INSERT INTO emenda.analise( ptrid, anatipo, analote, anadatainclusao )
			VALUES ({$ptrid}, 'T', {$analote}, 'now')";

	$db->executar( $sql );

	$db->commit();

	return true;
}*/

/*** FIM FUNCOES WORKFLOW ***/

function validaInformacoesGerais($ptrid, $emecod = null){
	global $db;

	if( $emecod ){
		$filtro = " AND emecod = '$emecod'";
	} else {
		$filtro = '';
	}

	$sql = "SELECT DISTINCT
				exe.exfid
			FROM
				emenda.v_emendadetalheentidade vede
			    inner join emenda.ptemendadetalheentidade pted
			    	on pted.edeid = vede.edeid
			    inner join emenda.planotrabalho ptr
			    	on ptr.ptrid = pted.ptrid
			    inner join emenda.execucaofinanceira exe
			    	on exe.ptrid = ptr.ptrid
			    inner join emenda.v_funcionalprogramatica vfun
			    	on vfun.acaid = vede.acaid
			WHERE
				(vede.emeano = {$_SESSION['exercicio']} or vede.emetipo = 'X')
			    and exe.ptrid = $ptrid
			    $filtro
			    and vfun.acastatus = 'A'
			    and exe.exfstatus = 'A'
			    and ptr.ptrstatus = 'A'
			    and vede.edestatus = 'A'
			    --and semid is null ";

			    $exfid = $db->pegaUm($sql);
			    return ( $exfid ? true : false);
}

function montaExecucaoOrcamentariaPopAnalise($ptrid){
	global $db;

	$sql = "SELECT
				vede.acaid,
			    COALESCE(fup.fupfuncionalprogramatica, 'Dados Incompletos') as fupfuncionalprogramatica, 
			    vede.emecod,
			   	vede.emeid,
			    (CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
			        ELSE 'Complemento' END) as tipoemenda, 
			    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
			    vede.gndcod as gnd, 
			    vede.gndcod||' - '||gn.gnddsc as gndcod, 
                vede.mapcod||' - '||map.mapdsc as mapcod,
                vede.foncod||' - '||fon.fondsc as foncod, 
			    per.pedid, 
			    tpe.tpedsc,
			    tpe.tpeid, 
			    sum(per.pervalor) as pervalor,
			    pti.ptrid,
			    case when vede.emdliberado = 'S' then 'Sim' else 'Não' end as liberado
			FROM emenda.ptiniciativa pti
				inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
			 	inner join emenda.ptemendadetalheentidade ped on ped.ptrid = pti.ptrid
			 	inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid and per.pedid = ped.pedid
			   	inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
			   	inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
			   	inner join emenda.autor aut on aut.autid = vede.autid
			   	left join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
			  	left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
                left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
                left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
                --inner join emenda.execucaofinanceira exf on exf.ptrid = pti.ptrid and exf.pedid = ped.pedid and exf.exfstatus = 'A'
			WHERE 
			    pti.ptrid = $ptrid
			GROUP BY 
			    per.pedid, tpe.tpedsc, tpe.tpeid, vede.emecod,
			    vede.emeid, aut.autnome, vede.emerelator, vede.gndcod, vede.mapcod, 
			    vede.foncod, fup.fupfuncionalprogramatica, vede.acaid,
			    vede.emetipo, gn.gnddsc, map.mapdsc, fon.fondsc,
                vede.emdliberado, pti.ptrid
			ORDER BY
				vede.emeid, vede.gndcod, fup.fupfuncionalprogramatica,
				per.pedid, tpe.tpedsc, vede.emecod,
			    vede.mapcod, vede.foncod, vede.acaid";

	$arDados = $db->carregar($sql);
	$arDados = ($arDados ? $arDados : array());
	//ver($arDados);

	if( $_SESSION['exercicio'] == '2009' || $boReformulacao ){
		$liberaCasa = 'display: none;';
	}

	$html = '<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
				<tr><td>
				<table class="" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
					<thead>
						<tr bgcolor="">
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Funcional</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Tipo</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Código</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Autor</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>GND</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Mod</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Fonte</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Nível de Ensino</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
						</tr>
					</thead>';

	$boExf = verificaExistePIExf($ptrid);

	$arPliid = "";

	foreach ($arDados as $chave => $valor) {
		if( $chave == 0 ) echo $html; $html = '';
		$boEmepenhado = '';

		$sql = "SELECT  exf.semid, exf.exfid, exf.plicod as pliid, exf.ptres, se.semdesc, exf.exfvalor
				FROM emenda.execucaofinanceira exf 
					left join emenda.situacaoempenho se on se.semid = exf.semid and se.semstatus = 'A'
				WHERE
					exf.ptrid = ".$valor['ptrid']."
				    and exf.pedid = ".$valor['pedid']."
				    and exf.tpeid = ".$valor['tpeid']."
				    and exf.exfstatus = 'A'";
		//	ver($sql);
		$arExcFinanceira = $db->carregar( $sql );
		$arExcFinanceira = ($arExcFinanceira ? $arExcFinanceira :array());

		$_SESSION['emenda']['chave']++;
		$chave % 2 ? $cor = "" : $cor = "#ffffff";
			
		$arPTRES = array();
		if( $valor['acaid'] ){
			$arPTRES = recuperaArPTRES( $ptrid, $valor['acaid'], $valor['pedid'], $valor['tipoemenda'] );
		}

		$html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
				<td style="text-align: center; color: rgb(0, 102, 204);">'.$valor['fupfuncionalprogramatica'].'</td>
				<td>'.$valor['tipoemenda'].'</td>
				<td style="text-align: center; color: rgb(0, 102, 204);">'.$valor['emecod'].'</td>
				<td>'.$valor['autnome'].'</td>
				<td>'.$valor['gndcod'].'</td>
				<td>'.$valor['mapcod'].'</td>
				<td>'.$valor['foncod'].'</td>
				<td>'.$valor['tpedsc'].'</td>
				<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($valor['pervalor'],2,',','.').'</td>
			</tr>';

		$html.= '<tr bgcolor="'.$cor.'">
					<td colspan="10"><table width="90%" bgcolor="#f5f5f5" id="tabelaExecucaoOrcamentaria_'.$valor['emeid'].'_'.$chave.'" cellspacing="1" cellpadding="2" align="center">
					<tr bgcolor="">
						<td align="Center" class="title" width="18%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>PTRES</strong></td>
						<td align="Center" class="title" width="48%" colspan="2"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>PI</strong></td>
						<td align="Center" class="title" width="18%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Utilizado</strong></td>
						<td align="Center" class="title" width="22%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Situação Empenho</strong></td>
					</tr>';
		echo $html;

		if($arPTRES || $valor['acaid'] ){ #verifica se tem dados para montar o ptres ?>
	<tbody id="tbodyTabela_<?php echo $valor['emeid'].'_'.$chave; ?>">
	<?
	foreach ($arExcFinanceira as $keyF => $exf) {
		if( $exf['semid'] == 4 ){
			$boEmepenhado = 'disabled="disabled"';
		}
		$excluir = '/imagens/excluir.gif';
		$onclick = "onclick=\"removerLinha( 'linha_".($keyF+1)."', '".$valor['emeid']."_".$chave."', '".$exf['exfid']."' );\"";
		if( !empty($exf['semid']) || sizeof($arExcFinanceira) == 1 ){
			$excluir = '/imagens/excluir_01.gif';
			$onclick = "";
		}
		?>
		<tr bgcolor="<?=$cor; ?>" id="linha_<?=$keyF+1; ?>">
			<td style="text-align: center;"
			<?php echo ($arPTRES ? '' : 'colspan="5"') ?>><?
			$exfvalor = '';
			if( $arPTRES ){
				$arPliid = $exf['pliid'];
				foreach ($arPTRES as $key => $val) {
					if($val['exfid'] == $exf['exfid']){
						$exfvalor = $exf['exfvalor'];
						echo $val['descricao'];
						$arValor = $valor['acaid']."|".$val['codigo'];
						break;
					}
					$arValor = $valor['acaid']."|".$val['codigo'];
				}
			} else {
				echo '<span align="center" style="color: red;">Não existe PI cadastrado para este PTRES</span>';
			}
			?></td>
			<?
			if($arPTRES){
				?>
			<td id="ComboPTRES" style="text-align: center;" colspan="2"><?php //echo montaComboPTRES($arValor, $chave, $valor['emeid'].'_'.$chave, $arPliid, true,$boEmepenhado);?>
			<?php echo montaPTRES($arValor, $chave, $valor['emeid'].'_'.$chave, $arPliid, true,$boEmepenhado, $valor['tipoemenda']);?>
			</td>

			<?php
			if (!empty($exf['exfvalor'])){
				$exfvalor = number_format($exf['exfvalor'],2,',','.');
			}
			$habCampo = 'S';
			if( !empty($boEmepenhado) ) $habCampo = 'N';

			$empenho = ( !empty($exf['semdesc']) ? $exf['semdesc'] : 'Não Solicitado' );
			?>
			<td style="text-align: center"><?php //echo campo_texto( 'exfvalor['.$valor['emeid'].'_'.$chave.'][]', 'S', $habCampo, 'Valor Utilizado', 18, 13, '[###.]###,##', '','','','','id="exfvalor[]"','',$exfvalor,"this.value=mascaraglobal('[###.]###,##',this.value); populaHiddenExfValor(this.value, '".$valor['emeid'].'_'.$chave."', 0); validaTotalRecurso('".$valor['emeid'].'_'.$chave."');"); ?>
			<?php echo $exfvalor; ?></td>
			<td style="text-align: center"><?php echo $empenho; ?></td>

		</tr>
		<?php
}
}//fim foreach execucaofinanceira
} else {
	echo '<tr bgcolor='.$cor.'>';
	echo '<td colspan="5" align="center" style="color: red;">Dados do recurso incompleto!</td>';
	echo '</tr>';
}
?>

</table>
</td>
</tr>
<?
}
?>
<tr bgcolor="<?=$cor; ?>">
	<td colspan="9">
	<table width="80%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2"
		align="center">
		<?php
		if($arPTRES){
			$boHabilita = true;
		}
		if( $boExf[$chave]['exfnumsolempenho'] || !$boHabilita ){
			//$disabilita = 'disabled="disabled"';
		} else {
			if( empty($disabilita) ){
				if(!possuiPermissao()) {
					$disabilita = 'disabled="disabled"';
				} else {
					$disabilita = '';
				}
			}
			if($boHabilita){
				$disabilita = '';
			}
		}
		?>
	</table>
	</td>
</tr>
<div id="mostraTitle"></div>
</table>
<br />
</td>
</tr>

</table>
</form>
<div id="erro"></div>

		<?
}

function montaExecucaoOrcamentaria($ptrid, $boReformulacao = false ){
	global $db;

	$sql = "SELECT
				vede.acaid,
			    COALESCE(fup.fupfuncionalprogramatica, 'Dados Incompletos') as fupfuncionalprogramatica, 
			    vede.emecod,
			   	vede.emeid,
			    (CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
			        ELSE 'Complemento' END) as tipoemenda, 
			    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
			    vede.gndcod as gnd, 
			    vede.gndcod||' - '||gn.gnddsc as gndcod, 
                vede.mapcod||' - '||map.mapdsc as mapcod,
                vede.foncod||' - '||fon.fondsc as foncod, 
			    per.pedid, 
			    tpe.tpedsc,
			    tpe.tpeid, 
			    sum(per.pervalor) as pervalor,
			    pti.ptrid,
			    case when vede.emdliberado = 'S' then 'Sim' else 'Não' end as liberado
			FROM emenda.ptiniciativa pti
				inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
			 	inner join emenda.ptemendadetalheentidade ped on ped.ptrid = pti.ptrid
			 	inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid and per.pedid = ped.pedid
			   	inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
			   	inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
			   	inner join emenda.autor aut on aut.autid = vede.autid
			   	left join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
			  	left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
                left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
                left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
			WHERE 
			    pti.ptrid = $ptrid
			    and per.pervalor <> 0
			GROUP BY 
			    per.pedid, tpe.tpedsc, tpe.tpeid, vede.emecod,
			    vede.emeid, aut.autnome, vede.emerelator, vede.gndcod, vede.mapcod, 
			    vede.foncod, fup.fupfuncionalprogramatica, vede.acaid,
			    vede.emetipo, gn.gnddsc, map.mapdsc, fon.fondsc,
                vede.emdliberado, pti.ptrid
			ORDER BY
				vede.emeid, vede.gndcod, fup.fupfuncionalprogramatica,
				per.pedid, tpe.tpedsc, vede.emecod, 
			    vede.mapcod, vede.foncod, vede.acaid";

	$arDados = $db->carregar($sql);
	$arDados = ($arDados ? $arDados : array());

	$arPerfil = pegaPerfilArray($_SESSION['usucpf']);

	if( $_SESSION['exercicio'] == '2009' || $boReformulacao ){
		$liberaCasa = 'display: none;';
	}

	$html = '<div id="loader-container" style="display: none">
			   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
			</div>';
	$html.= '<form action="" method="post" id="formExecOrcamentaria" name="formExecOrcamentaria">';
	$html.= '<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
				<tr><td class="subtitulocentro"> Programação Orçamentária </td></tr>
				<tr><td style="text-align: center;"> <img border=\'0\' src=\'../imagens/obrig.gif\' title=\'Indica campo obrigatório.\' /> Indica os campos obrigatórios </td></tr>
				<tr><td>
				<table class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
					<thead>
						<tr bgcolor="dedfde">
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Funcional</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Tipo</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Código</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Autor</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>GND</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Mod</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Fonte</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Nível de Ensino</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; '.$liberaCasa.'"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Limite Autorizado</strong></td>
						</tr>
					</thead>';
		
	$boExf = verificaExistePIExf($ptrid);
	$arPliid = "";

	foreach ($arDados as $chave => $valor) {
		if( $chave == 0 ) echo $html; $html = '';
		$boEmepenhado = '';

		$tpeid = $valor['tpeid'].'_'.$chave;

		$sql = "SELECT  exf.semid, exf.exfid, exf.plicod as pliid, se.semdesc, exf.exfvalor, exf.ptres
				FROM emenda.execucaofinanceira exf 
					left join emenda.situacaoempenho se on se.semid = exf.semid and se.semstatus = 'A'
				WHERE
					exf.ptrid = ".$valor['ptrid']."
				    and exf.pedid = ".$valor['pedid']."
				    and exf.tpeid = ".$valor['tpeid']."
				    and exf.exfstatus = 'A'";

		$arExcFinanceira = $db->carregar( $sql );
		$arExcFinanceira = ($arExcFinanceira ? $arExcFinanceira :array());

		$_SESSION['emenda']['chave']++;
		$chave % 2 ? $cor = "" : $cor = "#ffffff";
			
		$arPTRES = array();
		if( $valor['acaid'] ){
			$arPTRES = recuperaArPTRES( $ptrid, $valor['acaid'], $valor['pedid'], $valor['tipoemenda'] );
				
			/*if( $arPTRES )
				$_SESSION['emenda']['arPtres'] = $arPTRES;
				$_SESSION['emenda']['acaid'] = $valor['acaid'];*/
		}
		$html.= '<input type="hidden" name="emeid[]" id="emeid_'.$valor['emeid'].'_'.$tpeid.'" value="'.$valor['emeid'].'_'.$tpeid.'">
			<input type="hidden" name="tpeid['.$valor['emeid'].'_'.$tpeid.']" id="tpeid_'.$valor['tpeid'].'" value="'.$valor['tpeid'].'">
			<input type="hidden" name="acaid['.$valor['emeid'].'_'.$tpeid.']" id="acaid_'.$valor['acaid'].'" value="'.$valor['acaid'].'">
			<input type="hidden" name="pedid['.$valor['emeid'].'_'.$tpeid.']" id="pedid_'.$valor['pedid'].'" value="'.$valor['pedid'].'">
			<input type="hidden" name="pervalor['.$valor['emeid'].'_'.$tpeid.']" id="pervalor_'.$tpeid.'" value="'.$valor['pervalor'].'"><br>';
		
		if( $valor['liberado'] == 'Não' ){
			$color = 'red';
		} else {
			$color = 'rgb(0, 102, 204)';
		}

		$html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
				<td style="text-align: center; color: rgb(0, 102, 204);">'.$valor['fupfuncionalprogramatica'].'</td>
				<td>'.$valor['tipoemenda'].'</td>
				<td style="text-align: center; color: rgb(0, 102, 204);">'.$valor['emecod'].'</td>
				<td>'.$valor['autnome'].'</td>
				<td>'.$valor['gndcod'].'</td>
				<td>'.$valor['mapcod'].'</td>
				<td>'.$valor['foncod'].'</td>
				<td>'.$valor['tpedsc'].'</td>
				<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($valor['pervalor'],2,',','.').'</td>
				<td style="'.$liberaCasa.'; color: '.$color.';">'.$valor['liberado'].'</td>
			</tr>';

		$html.= '<tr bgcolor="'.$cor.'">
					<td colspan="10"><table width="90%" bgcolor="#f5f5f5" id="tabelaExecucaoOrcamentaria_'.$valor['emeid'].'_'.$tpeid.'" cellspacing="1" cellpadding="2" align="center">
					<tr bgcolor="dedfde">
						<td width="02%"><img border="0" src="../imagens/seta_filho.gif"></td>
						<td align="Center" class="title" width="18%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>PTRES</strong></td>
						<td align="Center" class="title" width="48%" colspan="2"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>PI</strong></td>
						<td align="Center" class="title" width="18%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Utilizado</strong></td>
						<td align="Center" class="title" width="22%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Situação Empenho</strong></td>
					</tr>';
		echo $html;

		if($arPTRES || $valor['acaid'] ){ #verifica se tem dados para montar o ptres ?>
<tbody id="tbodyTabela_<?php echo $valor['emeid'].'_'.$tpeid; ?>">
<?
foreach ($arExcFinanceira as $keyF => $exf) {
	if( $exf['semid'] == 4 ){
		//$boEmepenhado = 'disabled="disabled"';
	}
	$excluir = '/imagens/excluir.gif';
	$onclick = "onclick=\"removerLinha( 'linha_".($keyF+1)."', '".$valor['emeid']."_".$tpeid."', '".$exf['exfid']."' );\"";
	if( !empty($exf['semid']) || sizeof($arExcFinanceira) == 1 ){
		$excluir = '/imagens/excluir_01.gif';
		$onclick = "";
	}
	?>
	<tr bgcolor="<?=$cor; ?>" id="linha_<?=$keyF+1; ?>">
		<td><img src="<?=$excluir; ?>" title="Excluir" alt="Excluir"
		<?=$onclick; ?>></td>
		<td style="text-align: center;"
		<?php echo ($arPTRES ? '' : 'colspan="5"') ?>><?
		$exfvalor = '';
		
		if( $arPTRES ){
			$arPliid = $exf['pliid'];
			foreach ($arPTRES as $key => $val) {
				if($key == 0){
					echo '<input type="hidden" name="exfid['.$valor['emeid'].'_'.$tpeid.'][]" id="exfid['.$key.']" value="'.$exf['exfid'].'">';
					echo '<select class="CampoEstilo" '.$boEmepenhado.' name="ptres[]" id="ptres" onchange="montaComboPTRES(this.value, \''.$keyF.'_'.$exf['exfid'].'\', \''.$valor['emeid'].'_'.$tpeid.'\', \''.$valor['tipoemenda'].'\');" style="width: 50%;">';
					echo '<option value=""></option>';
				}
				
				if(sizeof($arPTRES) == 1 || ($val['codigo'] == $exf['ptres'] )){
					$select = 'selected="selected" ';
					$exfvalor = $val['exfvalor'];
					$exfpres = $val['codigo'];
				} else {
					$select = '';
					$exfpres = $exf['ptres'];
				}
				$arValor = $valor['acaid']."|".$val['codigo'];
				echo '<option value="'.$arValor.'" '.$select.'>'.$val['descricao'].'</option>';
			}
			echo '</select><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>';
		} else {
			echo '<span align="center" style="color: red;">Não existe PI cadastrado para este PTRES</span>';
		}
		?></td>
		<?
		if($arPTRES){
			if (!empty($exf['exfvalor'])){
				$exfvalor = number_format($exf['exfvalor'],2,',','.');
			}
			?>
		<td id="ComboPTRES" style="text-align: center;" colspan="2"><?php echo montaComboPTRES($arValor, $keyF.'_'.$exf['exfid'], $valor['emeid'].'_'.$tpeid, $arPliid, true,$boEmepenhado, $valor['tipoemenda']);?></td>
		<input type="hidden" id="exfvalor_[<?=$valor['emeid'].'_'.$tpeid; ?>]_0" name="exfvalor_[<?=$valor['emeid'].'_'.$tpeid; ?>]_0" value="<?php echo $exfvalor; ?>">
		<input type="hidden" id="exfvalortotal[<?=$valor['emeid'].'_'.$tpeid; ?>]" name="exfvalortotal[<?=$valor['emeid'].'_'.$tpeid; ?>]" value="<?php echo $valor['pervalor']; ?>">
		
			<?php
			$habCampo = 'S';
			if( !empty($boEmepenhado) ) $habCampo = 'N';

			$empenho = ( !empty($exf['semdesc']) ? $exf['semdesc'] : 'Não Solicitado' );
			?>
		<td style="text-align: center"><?php echo campo_texto( 'exfvalor['.$valor['emeid'].'_'.$tpeid.'][]', 'S', $habCampo, 'Valor Utilizado', 18, 13, '[###.]###,##', '','','','','id="exfvalor[]"','',$exfvalor,"this.value=mascaraglobal('[###.]###,##',this.value); populaHiddenExfValor(this.value, '".$valor['emeid'].'_'.$keyF."', 0); validaTotalRecurso('".$valor['emeid'].'_'.$tpeid."');"); ?>
			<input type="hidden" name="ptres_pi[<?=$valor['emeid'].'_'.$tpeid; ?>][]" id="ptres_pi[<?=$valor['emeid'].'_'.$tpeid; ?>]" value="<?=$exfpres; ?>">
		</td>
		<td style="text-align: center"><?php echo $empenho; ?></td>

	</tr>
	<?php
}
}//fim foreach execucaofinanceira
if($arPTRES){
	if( empty($arExcFinanceira) ){
		$excluir = '/imagens/excluir_01.gif';
		$onclick = "";
		?>
	<tr bgcolor="<?=$cor; ?>" id="linha_<?=$chave+1; ?>">
		<td><img src="<?=$excluir; ?>" title="Excluir" alt="Excluir"
		<?=$onclick; ?>></td>
		<td style="text-align: center;"
		<?php echo ($arPTRES ? '' : 'colspan="5"') ?>><?
		$exfvalor = '';
		//if( $arPTRES ){
		$arPliid = '';
		foreach ($arPTRES as $key => $val) {
			if($key == 0){
				echo '<input type="hidden" name="exfid['.$valor['emeid'].'_'.$tpeid.'][]" id="exfid['.$key.']" value="">';
				echo '<select class="CampoEstilo" '.$boEmepenhado.' name="ptres[]" id="ptres" onchange="montaComboPTRES(this.value, '.$chave.', \''.$valor['emeid'].'_'.$tpeid.'\', \''.$valor['tipoemenda'].'\')" style="width: 50%;">';
				echo '<option value=""></option>';
			}
			if(sizeof($arPTRES) == 1 || $val['codigo'] == $valor['pliid']){
				$select = 'selected="selected" ';
				$exfvalor = '';
				$exfpres = $val['codigo'];
			} else {
				$select = '';
				$exfpres = '';
			}
			$arValor = $valor['acaid']."|".$val['codigo'];
			echo '<option value="'.$arValor.'" '.$select.'>'.$val['descricao'].'</option>';
		}
		echo '</select><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>';
		/*} else {
		 echo '<span align="center" style="color: red;">Não existe PI cadastrado para este PTRES</span>';
		 }*/
		?></td>
		<?
		//if($arPTRES){
		?>
		<td id="ComboPTRES" style="text-align: center;" colspan="2"><?php echo montaComboPTRES($arValor, $chave, $valor['emeid'].'_'.$tpeid, $arPliid, true,$boEmepenhado, $valor['tipoemenda']);?></td>
		<input type="hidden" id="exfvalor_[<?=$valor['emeid'].'_'.$tpeid; ?>]_0" name="exfvalor_[<?=$valor['emeid'].'_'.$tpeid; ?>]_0" value="<?php echo $exfvalor; ?>">
		<input type="hidden" id="exfvalortotal[<?=$valor['emeid'].'_'.$tpeid; ?>]" name="exfvalortotal[<?=$valor['emeid'].'_'.$tpeid; ?>]" value="<?php echo $valor['pervalor']; ?>">

			<?php
			/*if (!empty($exf['exfvalor'])){
			 $exfvalor = number_format($exf['exfvalor'],2,',','.');
			 }*/
			$habCampo = 'S';
			if( !empty($boEmepenhado) ) $habCampo = 'N';

			$empenho = ( 'Não Solicitado' );
			?>
		<td style="text-align: center"><?php echo campo_texto( 'exfvalor['.$valor['emeid'].'_'.$tpeid.'][]', 'S', $habCampo, 'Valor Utilizado', 18, 13, '[###.]###,##', '','','','','id="exfvalor[]"','',$exfvalor,"this.value=mascaraglobal('[###.]###,##',this.value); populaHiddenExfValor(this.value, '".$valor['emeid'].'_'.$tpeid."', 0); validaTotalRecurso('".$valor['emeid'].'_'.$tpeid."');"); ?>
			<input type="hidden" name="ptres_pi[<?=$valor['emeid'].'_'.$tpeid; ?>][]" id="ptres_pi[<?=$valor['emeid'].'_'.$tpeid; ?>]" value="<?=$exfpres; ?>">
		</td>
		<td style="text-align: center"><?php echo $empenho; ?></td>

	</tr>
	<?php
	//}
}
?>
<tbody id="tbodyTabela">
	<tr style="text-align: left;">
		<td colspan="4"><?php
		if( !$boReformulacao ){
			if( empty($boExf[$chave]['exfnumempenhooriginal']) && !in_array(CONSULTA_GERAL, $arPerfil ) && verificaPermissaoPerfil('analise', 'boolean') ){
				?> <span
			id="linkInserirLinha_<?php echo $valor['emeid'].'_'.$tpeid; ?>"
			onclick="return carregarRecurso( '', '', '', '', '', '<?php echo $valor['emeid'].'_'.$tpeid; ?>');"
			style="margin-left: 3%; cursor: pointer;"> <img align="top"
			style="border: medium none;" src="/imagens/gif_inclui.gif" /> Inserir
		Complemento </span> <?} else { ?> <span
			id="linkInserirLinha_<?php echo $valor['emeid'].'_'.$tpeid; ?>"
			style="margin-left: 3%; cursor: pointer;"> <img align="top"
			style="border: medium none;" src="/imagens/gif_inclui_d.gif" />
		Inserir Complemento </span> <?} 
}?></td>
	</tr>
	<?
} else {
	echo '<tr><td colspan="5" style="text-align: center"><span align="center" style="color: red;">Não existe PI cadastrado para este PTRES</span></td></tr>';
}
} else {
	echo '<tr bgcolor='.$cor.'>';
	echo '<td colspan="5" align="center" style="color: red;">Dados do recurso incompleto!</td>';
	echo '</tr>';
}
?>
	</table>
	</td>
	</tr>
	<?
}//fim do foreach
?>
	<tr bgcolor="<?=$cor; ?>">
		<td colspan="9">
		<table width="80%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2"
			align="center">
			<?php
			if($arPTRES){
				$boHabilita = true;
			}
			if( $boExf[$chave]['exfnumsolempenho'] || !$boHabilita ){
				//$disabilita = 'disabled="disabled"';
			} else {
				if( empty($disabilita) ){
					if(!possuiPermissao()) {
						$disabilita = 'disabled="disabled"';
					} else {
						$disabilita = '';
					}
				}
				if($boHabilita){
					$disabilita = '';
				}
			}
			if( in_array(CONSULTA_GERAL, $arPerfil ) ){
				$disabilita = 'disabled="disabled"';
			}
			?>
		</table>
		</td>
	</tr>
	<div id="mostraTitle"></div>
	</table>
	<br />
	</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center"><input type="button"
			value="Salvar" <?=(empty($disabilita) ? $disabilita = verificaPermissaoPerfil('analise'): $disabilita);?> name="btnSalvarExef"
			id="btnSalvarExef" onclick="salvarExecucaoOrcamentaria();"></td>
	</tr>
	</table>
	</form>
	<div id="erro"></div>

	<?
}

function verificaExistePIExf($ptrid, $plicod = null){
	global $db;

	if( $plicod){
		$filtro = " AND plicod = $plicod";
	} else {
		$filtro = '';
	}

	$sql = "SELECT
			  plicod,
			  exfnumsolempenho,
			  exfnumempenhooriginal
			FROM 
			  emenda.execucaofinanceira
			WHERE
			  exfstatus = 'A'
			  AND ptrid = $ptrid
			  $filtro
			ORDER BY
			  exfid";

			  return $db->carregar($sql);
}

function recuperaSQLComboPTRES( $arValor, $tipoEmenda ){
	global $db;

	$arValor = explode('|', $arValor);
	
	/*$sql = "SELECT DISTINCT
			    pi.pliid as codigo, 
			    pi.plicod, 
			    pi.plicod ||' - '||pi.plititulo as descricao
			FROM monitora.pi_planointerno pi
				inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
				inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
			WHERE 
			    pt.acaid = ".$arValor[0]."
			    and  pt.ptres = '".$arValor[1]."'
			    and pi.plisituacao in ('S', 'C')
			    and pi.plistatus = 'A'
			UNION    
			SELECT DISTINCT
			    pi.pliid as codigo, 
			    pi.plicod, 
			    pi.plicod ||' - '||pi.plititulo as descricao
			FROM 
			    monitora.planointerno pi
			WHERE 
			    pi.plisituacao = 'S'
			    --AND pi.acaid = ".$arValor[0]."
			    AND pi.pliptres = '".$arValor[1]."'
			    --AND ac.prgano = '".$_SESSION['exercicio']."'";*/
				
			
				/*$sql = "SELECT DISTINCT
				 pliid as codigo,
				 plicod,
				 plicod ||' - '||plititulo as descricao
				 FROM
				 monitora.planointerno pi
				 WHERE
				 plisituacao = 'S'
				 AND acaid = ".$arValor[0]."
	 AND pliptres = '".$arValor[1]."'";*/
	
	if( $tipoEmenda == 'Complemento' ) $filtro = " and pi.pliano >= '".$_SESSION['exercicio']."'";
	else $filtro = " and pi.pliano = '".$_SESSION['exercicio']."'";
	
	$sql = "SELECT DISTINCT 
			    pi.plicod as codigo, 
			    pi.plicod ||' - '||pi.plititulo as descricao
			FROM monitora.pi_planointerno pi
				inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
				inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
                left join emenda.execucaofinanceira exf on exf.plicod = pi.plicod and exf.exfstatus = 'A'
			WHERE 
			    pt.acaid = ".$arValor[0]."
			    and  pt.ptres = '".$arValor[1]."'
			    and pi.plisituacao in ('S', 'C', 'A')
			    and pi.plistatus = 'A'
			    $filtro
			order by pi.plicod";

	return $db->carregar($sql);
}

function montaPTRES($arValor, $id, $emeid, $pliid='', $boMontaSelect = true, $boEmepenhado = '', $tipoEmenda)
{
	global $db;
	if( $arValor ){
		$arPTRES = recuperaSQLComboPTRES($arValor, $tipoEmenda);
		if( $boMontaSelect ){
			if($arPTRES){
				foreach ($arPTRES as $key => $vPtres) {
						
					if(sizeof($arPTRES) == 1 || $vPtres['codigo'] == $pliid){
						$select = 'selected="selected"';
						if( !$pliid ){
							$pliid = $vPtres['codigo'];
						}

						echo $vPtres['descricao'];
					} else {
						$select = '';
					}
				}
			}

		}
	} else {
		echo '<span align="center" style="color: red;">Não foi encontrado PI para esta emenda</span>';
	}
}

function montaComboPTRES($arValor, $id, $emeid, $pliid='', $boMontaSelect = true, $boEmepenhado = '', $tipoEmenda = ''){
	global $db;
	if( $arValor ){
		$arPTRES = recuperaSQLComboPTRES($arValor, $tipoEmenda);
		$_SESSION['emenda']['planointerno'] = $arPTRES;
		if( $boMontaSelect ){
			$input = '';
			?>
	<select class="CampoEstilo" name="pi_<?=$id; ?>" <?=$boEmepenhado; ?>
		id="pi_<?=$id; ?>" style="width: 80%;"
		onchange="populaHiddenExecucaoOrcamentaria(this.value, '<?=$id;?>');">
		<?
		echo '<option value=""></option>';
		$arPliid = array();
		if($arPTRES){
			foreach ($arPTRES as $key => $vPtres) {
					
				if(sizeof($arPTRES) == 1 || $vPtres['codigo'] == $pliid){
					$select = 'selected="selected"';
					if( !$pliid ){
						$pliid = $vPtres['codigo'];
					}
				} else {
					$select = '';
				}
				if( !in_array( $vPtres['codigo'], $arPliid ) ) $arPliid[] = $vPtres['codigo'];
				echo '<option value="'.$vPtres['codigo'].'" '.$select.'>'.$vPtres['descricao'].'</option>';
				//$input .= '<input type="hidden" name="pi['.$emeid.'][]" id="pi['.$id.']" value="'.$vPtres['codigo'].'">';
			}
		}
		$arValor = explode('|', $arValor);
		?>
	</select>
	<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif" />
	<input type="hidden" name="pi[<?=$emeid; ?>][]" id="pi[<?=$id; ?>]" value="<?=$pliid; ?>">
		<? echo $input;
		/*<input type="text" name="pi[<?=$emeid; ?>][]" id="pi[<?=$id; ?>]" value="<?=$pliid; ?>">*/
	/*foreach ($arPliid as $pliid) {
		echo '<input type="hidden" name="pi['.$emeid.'][]" id="pi['.$id.']" value="'.$pliid.'">';
	}*/
	
}
} else {
	echo '<span align="center" style="color: red;">Não foi encontrado PI para esta emenda</span>';
}
}

function montaComboFuncionalProgramaticaOrçamento($resid,$boPTA = false){
	global $db;

	if( $resid ){
		$sql = "SELECT
				  unicod
				FROM 
				  emenda.responsavel
				WHERE
				  resid in ($resid, 3)";

		$unicod = $db->carregarColuna($sql);

		$sql = "SELECT
					  acaid as codigo,
					  fupfuncionalprogramatica ||' - '|| trim(replace(fupdsc, '\n', ' ')) as descricao
					FROM 
					  emenda.v_funcionalprogramatica
					WHERE
						acastatus = 'A'
					    --AND prgano <= '{$_SESSION['exercicio']}'
					    AND unicod in ('".(implode("', '", $unicod))."')
					ORDER BY 
						acaid";

		campo_popup('acaid',$sql,'Selecione','','400x800','100', array(
		array("codigo" => "acacod",
																		  "descricao" => "<b>Ação Orçamentária:</b>","string" => "1"),
		array("codigo" => "funcod",
																		  "descricao" => "<b>Função:</b>","string" => "1"),
		array("codigo" => "sfucod",
																		  "descricao" => "<b>SubFunção:</b>","string" => "1"),
		array("codigo" => "loccod",
																		  "descricao" => "<b>Localizador:</b>","string" => "1"),
		array("codigo" => "prgano",
																		  "descricao" => "<b>Ano:</b>","string" => "1")),
		1,true,$boPTA
		);
	} else {
		$sql = array();
		$db->monta_combo("acaid", $sql, "N", "", '', '', '', '350', 'N','acaid');
	}
}

function salvarExecucaoOrcamentaria($post){
	global $db;
	
	//$arPi = array_values($post['pi']);
	$arPi = $post['pi'];
	validaSessionPTA( $_SESSION["emenda"]["ptridAnalise"] );

	//ver($post,d);
	//ver( array_unique( $post['emeid'] ), $post );

	$arEmeid = array_unique( $post['emeid'] );

	foreach( $arEmeid as $chave => $emeid ){
	
		if( isset( $post['pi'][$emeid] ) ){
			foreach( $post['pi'][$emeid] as $key => $pi ){
								
				if( $post['exfid'][$emeid][$key] ){
					$sql = "UPDATE
							  emenda.execucaofinanceira  
							SET 
							  plicod = '$pi',
							  exfdataalteracao = now(),
							  usucpf = '".$_SESSION['usucpf']."',
							  exfvalor = ".retiraPontos($post['exfvalor'][$emeid][$key]).",
							  tpeid = ".$post['tpeid'][$emeid].",
							  ptres = '".$post['ptres_pi'][$emeid][$key]."'
							WHERE 
							  exfid = ".$post['exfid'][$emeid][$key]."
							  --and exfnumsolempenho is null";
					
					$db->executar($sql);
				} else {
					/*if( validaPIExecucaoOrcamentaria($pi) ){
						echo 'Falha na operação, Não pode inserir PI dublicado no banco';
						break;
						die;
						} else {*/
					$sql = "INSERT INTO
								  emenda.execucaofinanceira(
								  tpeid,
								  plicod,
								  ptrid,
								  pedid,
								  exfvalor,
								  exfdatainclusao,
								  usucpf,
								  ptres
								) 
								VALUES (
								  ".$post['tpeid'][$emeid].",
								  '$pi',
								  {$_SESSION["emenda"]["ptridAnalise"]},
								  {$post['pedid'][$emeid]},
								  ".retiraPontos($post['exfvalor'][$emeid][$key]).",
								  now(),
								  '".$_SESSION['usucpf']."',
								  '{$post['ptres_pi'][$emeid][$key]}'
								)";

								  $db->executar($sql);
								  //}
				}
			}
		}
	}
	echo $db->commit();
}
function carregaExecucaoOrcamentaria(){
	global $db;

	$sql = "SELECT
		  exe.exfid,
		  ptr.ptres as pliptres,
		  exe.pliid,
		  exe.exfvalor,
          vede.emeid as eme,
          exe.exfnumempenhooriginal,
          vede.gndcod
		FROM 
		  	emenda.execucaofinanceira exe
            inner join monitora.pi_planointerno pli on pli.plicod = exe.plicod
			inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
			inner join monitora.ptres ptr on ptr.ptrid = plpt.ptrid and ptr.ptres = exe.ptres
            inner join emenda.ptemendadetalheentidade pede
            	on pede.pedid = exe.pedid
            inner join emenda.v_emendadetalheentidade vede
            	on vede.edeid = pede.edeid
		WHERE
			exe.ptrid = ".$_SESSION['emenda']['ptridAnalise']."
			and exe.exfstatus = 'A'
            and vede.edestatus = 'A'
        ORDER BY
		    vede.emeid,
		    vede.gndcod,
		    exe.exfvalor";

	$arDadosExef = $db->carregar($sql);
	$arDadosExef = $arDadosExef ? $arDadosExef : array();

	return $arDadosExef;
}
function validaPIExecucaoOrcamentaria($pi){
	global $db;

	$sql = "SELECT
			  	pliid
			FROM 
			  	emenda.execucaofinanceira
			WHERE
				ptrid = ".$_SESSION['emenda']['ptridAnalise']."
				and pliid = $pi
				and exfstatus = 'A'";

	$pliid = $db->pegaUm($sql);
	if(!empty($pliid) ){
		return true;
	} else {
		return false;
	}
}

function verificaEmendaExisteVinculoPTA($emeid){
	global $db;

	$sql = "SELECT DISTINCT
			    1 as existe
			FROM
			    emenda.emenda eme
			    inner join emenda.emendadetalhe emd
			    	on emd.emeid = eme.emeid
			    inner join emenda.emendadetalheentidade ede
			    	on ede.emdid = emd.emdid
			    inner join emenda.ptemendadetalheentidade pede
			    	on pede.edeid = ede.edeid
			WHERE
				eme.emeid = $emeid";

	$retorno = $db->pegaUm($sql);

	if( $retorno ){
		$retorno = 'disabled="disabled"';
	} else {
		$retorno = '';
	}

	return $retorno;
}

function verificaEmendaDetalheExisteVinculoPTA($emdid, $enbid = null){
	global $db;

	if( $enbid ){
		$filtro = "and ede.enbid = $enbid";
	}

	$sql = "SELECT DISTINCT
			    ini.iniid,
			    ini.ininome
			FROM emenda.emendadetalhe emd
				inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid and ede.edestatus = 'A'
				inner join emenda.ptemendadetalheentidade ped on ped.edeid = ede.edeid
				inner join emenda.ptiniciativa pti on pti.ptrid = ped.ptrid
				inner join emenda.iniciativa ini on ini.iniid = pti.iniid and ini.inistatus = 'A'
				inner join emenda.iniciativaemendadetalhe ied on ied.iniid = ini.iniid and ied.emdid = emd.emdid and ied.iedstatus = 'A'
                inner join emenda.planotrabalho ptr on ptr.ptrid = ped.ptrid and ptr.ptrstatus = 'A'
			WHERE
				emd.emdid = $emdid
				$filtro
			ORDER BY
				ini.ininome";

				return $db->carregar($sql);
}
function verificaEmendaDetalheExisteVinculoEntidade($emdid){
	global $db;
	$sql = "SELECT DISTINCT
			    ini.iniid,
			    ini.ininome
			FROM emenda.emendadetalhe emd
				inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid and ede.edestatus = 'A'
                inner join emenda.iniciativadetalheentidade ide on ide.edeid = ede.edeid and ide.idestatus = 'A'
                inner join emenda.iniciativa ini on ini.iniid = ide.iniid and ini.inistatus = 'A'
			WHERE
				emd.emdid = $emdid
			ORDER BY
				ini.ininome";

	return $db->carregar($sql);
}


/*********************************************************************
 * FUNCOES DOS MODELOS DE DOCUMENTOS By: FERNANDO BAGNO - 28/11/2009 *
 *********************************************************************/

function emendaCadastraDocumento( $dados ){
	global $db;

	if( strpos($dados["mdoconteudo"], '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
		$dados["mdoconteudo"] = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $dados["mdoconteudo"] );
	} else {
		$dados["mdoconteudo"] = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $dados["mdoconteudo"] );
	}

	if( $dados["mdoid"] ){

		$sql = "UPDATE
					emenda.modelosdocumentos
				SET
					tpdcod = {$dados["tpdcod"]}, 
					mdonome = '{$dados["mdonome"]}', 
					mdoconteudo = '{$dados["mdoconteudo"]}'
				WHERE
					mdoid = {$dados["mdoid"]} ";



	}else{

		$sql = "INSERT INTO emenda.modelosdocumentos( tpdcod,
													  mdonome, 
													  mdoconteudo, 
													  mdostatus, 
													  usucpf, 
													  mdodatainclusao )
											  VALUES( {$dados["tpdcod"]},
											  		  '{$dados["mdonome"]}',
											  		  '{$dados["mdoconteudo"]}',
											  		  'A',
											  		  '{$_SESSION["usucpf"]}',
											  		  'now()' )";

	}

	$db->executar( $sql );
	$db->commit();

	$db->sucesso( "principal/modeloDocumentos", "" );

}

function emendaExcluiDocumento( $mdoid ){

	global $db;

	$sql = "UPDATE emenda.modelosdocumentos SET mdostatus = 'I' WHERE mdoid = {$mdoid}";

	$db->executar( $sql );
	$db->commit();

	$db->sucesso( "principal/modeloDocumentos", "" );

}

function insereIntervenienteVinculadoMec( $ptrid, $pmcid ){
	global $db;

	$sql = "SELECT DISTINCT
				uni.entidinterveniente,
			    uni.entiddirigente,
			    ico.itcid
			FROM
				emenda.analise ana
			    inner join emenda.unidades uni
			    	on uni.uniid = ana.uniid
			    left join emenda.ptminutaconvenio pmc
			    	on pmc.ptrid = ana.ptrid
			        and pmc.pmcstatus = 'A'
			    left join emenda.intervenienteconvenio ico
			    	on ico.pmcid = pmc.pmcid
			        and ico.itcstatus = 'A'
			WHERE 
				ana.ptrid = $ptrid
			    and uni.unisigla ilike '%mec%'
			    and uni.unistatus = 'A'
			    and ana.anastatus = 'A'";

	$arInter = $db->carregar($sql);
	
	if( is_array( $arInter ) && $arInter[0]['entidinterveniente'] ){
		if( empty($arInter[0]['itcid']) ){
			foreach ($arInter as $valor) {
				$sql = "INSERT INTO
						  emenda.intervenienteconvenio(
						  entidinterveniente,
						  entiddirigente,
						  pmcid,
						  itcstatus
						) 
						VALUES (
						  '".$valor['entidinterveniente']."',
						  '".$valor['entiddirigente']."',
						  $pmcid,
						  'A'
						)";

						  $db->executar($sql);
			}
			return $db->commit();
		}
	}
}

function alterarDadosMinuta( $post ){
	global $db;

	$vigdatafim = ( $post['vigdatafim'] ? "'".formata_data_sql( $post['vigdatafim'] )."'" : 'null');

	if( $post['vigid'] ){
		$sql = "UPDATE emenda.ptvigencia SET vigdias = ".$post['vigdias'].", vigdatafim = ".$vigdatafim." WHERE vigid = ".$post['vigid'];
		$db->executar( $sql );
	}
	$sql = "UPDATE
			  emenda.ptminutaconvenio  
			SET
			  pmcobjeto = '".$post['pmcobjeto']."',
			  pmcresolucao = '".$post['pmcresolucao']."'
			WHERE 
			  pmcid = ".$post['pmcid'];

	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );

	if( !empty($_POST['obcid'][0]) ){
		$sql = "DELETE FROM emenda.objetominutaconvenio WHERE pmcid = ".$post['pmcid'];
		$db->executar( $sql );
		foreach ($_POST['obcid'] as $obcid) {
			/*$sql = "SELECT omcid FROM emenda.objetominutaconvenio WHERE obcid = $obcid and pmcid = ".$post['pmcid'];
			 $omcid = $db->pegaUm( $sql );
			 if( empty($omcid) ){*/
			$sql = "INSERT INTO emenda.objetominutaconvenio( obcid, pmcid )
						VALUES ( $obcid, ".$post['pmcid'].")";
			$db->executar( $sql );
			//}
		}
	}
	return $db->commit();
}

function listaIntervenienteConvenio($pmcid, $arPerfil = array() ){
	global $db;

	if(!in_array( CONSULTA_GERAL, $arPerfil ) && verificaPermissaoPerfil('analise', 'boolean') ){
		$acoes = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"popUpManterInterveniente('||inc.itcid||', '||inc.pmcid||');\">
			  		   <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"excluirIntervenienteConvenio('||inc.itcid||', '||inc.pmcid||');\">";
	} else {
		$acoes = "<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">
			  		   <img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
	}

	$sql = "SELECT
			  '<center>$acoes</center>
			  		   <input type=\"hidden\" name=\"entidinterveniente[]\" id=\"entidinterveniente['||ent.entid||']\" value=\"'||ent.entid||'\">'
				as acoes,
			  ent.entnumcpfcnpj,
			  ent.entnome,
			  ent1.entnumcpfcnpj as cpfdirigente,
			  ent1.entnome as nomedirigente
			  
			FROM 
			  emenda.intervenienteconvenio inc
			  inner join entidade.entidade ent
			  	on inc.entidinterveniente = ent.entid
			  inner join entidade.entidade ent1
			    on inc.entiddirigente = ent1.entid
			WHERE
				inc.itcstatus = 'A'
			    AND inc.pmcid = ".$pmcid;

	$cabecalho = array("Acões","CNPJ","Interveniente","CPF","Dirigente");
	$db->monta_lista_simples( $sql, $cabecalho, 100000, 1, 'N', '100%');
}

function abrePTAconsolidado( $ptrid, $boSomenteTr = false, $tipo = '') {
	if($ptrid){
		if(!$boSomenteTr){
			$tabela = "<table align=\"center\" class=\"Tabela\" cellpadding=\"10\" cellspacing=\"1\">";
		}
		$tabela .= "<tr>
						<td bgcolor=\"#FFFFFF\" width=\"50%\"> 
							<a onclick=\"abrePTAconsolidado('$ptrid', 'pta');\" style=\"cursor: pointer;\">
								<img src=\"/imagens/print2.gif\" width=\"30px\" align=\"absmiddle\">Imprimir versão consolidada do PTA 
							</a>
						</td>";

		if( $tipo == 'convenio' ){
			$tabela .= "<td bgcolor=\"#FFFFFF\" style=\"text-align:right;\" width=\"50%\">
							<a onclick=\"abrePTAconsolidado('$ptrid', 'convenio');\" style=\"cursor: pointer;\">
								<img src=\"/imagens/print2.gif\" width=\"30px\" align=\"absmiddle\">Imprimir Convênio PTA
							</a>
						</td>
					</tr>";
		} else {
			$tabela .= "<td bgcolor=\"#FFFFFF\" style=\"text-align:right;\" width=\"50%\">
							<a onclick=\"abrePTAconsolidado('$ptrid', 'entidade');\" style=\"cursor: pointer;\">
								<img src=\"/imagens/print2.gif\" width=\"30px\" align=\"absmiddle\">Imprimir consolidação da entidade PTA
							</a>
						</td>
					</tr>";
		}
		if(!$boSomenteTr){
			$tabela .= "</table>";
		}
		echo $tabela;
	}
}
function abrePTAAnaliseRelatorio( $ptrid, $tipo, $boReformulacao = false, $boRelatorio = true ) {
	if( $ptrid ){
		$tabela = "<table align=\"center\" class=\"Tabela\" cellpadding=\"10\" cellspacing=\"1\">";
		$tabela .= "<tr>";
		if( $boRelatorio ){
			$tabela .= "<td bgcolor=\"#FFFFFF\">
							<a onclick=\"abrePTAAnaliseRelatorio('$ptrid', '$tipo');\" style=\"cursor: pointer;\">
								<img src=\"/imagens/print2.gif\" width=\"30px\" align=\"absmiddle\">Imprimir Análise PTA 
							</a>
						</td>";
		}
		if( $tipo == 'T' && $boReformulacao ){
			$tabela .= "
						<td bgcolor=\"#FFFFFF\" align='right'>
							<img onclick=\"visualizarTiposReformulacao( $ptrid );\" style=\"cursor: pointer;\" src=\"/imagens/consultar.gif\">
							<a onclick=\"visualizarTiposReformulacao( $ptrid );\" style=\"cursor: pointer;\"> Visualizar Tipo(s) de Reformulação </a>
						</td>";
		}
		$tabela .= "</tr></table>";
		echo $tabela;
	}
}

function montaRelatorioPorAnalise($ptrid, $tipo, $arAnalise){
	global $db;

	$ptrcod = pegaNumPTA( $ptrid );

	$sql = "SELECT
				vede.acaid,
			    COALESCE(fup.fupfuncionalprogramatica, 'Dados Incompletos') as fupfuncionalprogramatica, 
			    vede.emecod,
			   	vede.emeid,
			    (CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
			        ELSE 'Complemento' END) as tipoemenda, 
			    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
			    vede.gndcod as gnd, 
			    vede.gndcod||' - '||gn.gnddsc as gndcod, 
                vede.mapcod||' - '||map.mapdsc as mapcod,
                vede.foncod||' - '||fon.fondsc as foncod, 
			    per.pedid, 
			    tpe.tpedsc,
			    tpe.tpeid, 
			    sum(per.pervalor) as pervalor,
			    --per.pervalor,
			    pti.ptrid,
			    case when vede.emdliberado = 'S' then 'Sim' else 'Não' end as liberado
			FROM emenda.ptiniciativa pti
				inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
			 	inner join emenda.ptemendadetalheentidade ped on ped.ptrid = pti.ptrid
			 	inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid and per.pedid = ped.pedid
			   	inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
			   	inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
			   	inner join emenda.autor aut on aut.autid = vede.autid
			   	left join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
			  	left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
                left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
                left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
                inner join emenda.execucaofinanceira exf on exf.ptrid = pti.ptrid and exf.pedid = ped.pedid and exf.exfstatus = 'A'
			WHERE 
			    pti.ptrid = $ptrid
			    AND pervalor <> 0
			GROUP BY 
			    per.pedid, tpe.tpedsc, tpe.tpeid, vede.emecod,
			    vede.emeid, aut.autnome, vede.emerelator, vede.gndcod, vede.mapcod, 
			    vede.foncod, fup.fupfuncionalprogramatica, vede.acaid,
			    vede.emetipo, gn.gnddsc, map.mapdsc, fon.fondsc,
                vede.emdliberado, pti.ptrid
			ORDER BY
				vede.emeid, vede.gndcod, fup.fupfuncionalprogramatica,
				per.pedid, tpe.tpedsc, vede.emecod, 
			    vede.mapcod, vede.foncod, vede.acaid";

	/*$arDadosProgOrcamentaria = $db->carregar($sql);*/
	$arDadosProgOrcamentaria = ($arDadosProgOrcamentaria) ? $arDadosProgOrcamentaria : array();

	$boReformulacao = pegaPaiPTA( $ptrid );
	$boReformulacao = ( !empty($boReformulacao) ? true : false );

	if( $boReformulacao ){
		$arTipos = pegaTipoReformulacao( $ptrid );
		$arrayTipo = array();
		foreach ($arTipos as $v) {
			array_push( $arrayTipo, $v['descricao'] );
		}

		$label = ' Relatório ('.implode(' / ', $arrayTipo).') realizadas no PTA nº ';
	} else {
		if( $tipo == 'D' ){
			$label = ' Relatório das ANÁLISES VINCULAÇÃO UNIDADES GESTORAS realizadas no PTA nº ';
		} else if( $tipo == 'M' ){
			$label = ' Relatório das ANÁLISE DE MÉRITO realizadas no PTA nº ';
		} else if( $tipo == 'T' ){
			$label = ' Relatório das ANÁLISE TÉCNICA realizadas no PTA nº ';
		}
	}


	?>
	<table id="tblform0" width="95%" class="tabela" cellSpacing="1"
		cellPadding="3" align="center">
		<tr>
			<td style="text-align: right;">Impresso por: <?php 
					echo $_SESSION['usunome']."<br>&nbsp;";
				?></td>
		</tr>
		<tr>
			<td style="text-align: center; font-size: 18px;"><?php echo $label . $ptrcod.'/'.$_SESSION['exercicio'];?></td>
		</tr>
	</table>
	<?php

	$arCabecalho = cabecalhoPTARelatorioAnalise($ptrid);

	$arCabecalho['cnpj'] = substr($arCabecalho['cnpj'],0,2) . "." .
	substr($arCabecalho['cnpj'],2,3) . "." .
	substr($arCabecalho['cnpj'],5,3) . "/" .
	substr($arCabecalho['cnpj'],8,4) . "-" .
	substr($arCabecalho['cnpj'],12,2);
		
	echo "<table align=\"center\" width=\"95%\" class=\"tabela\" cellpadding=\"1\" cellspacing=\"3\">
					 <tbody>
					 	<tr>
					 		<td colspan=\"4\" class=\"subtitulocentro\">Dados do Plano de Trabalho</td>
					 	</tr>
						<tr>
							<td style=\"text-align: right; width:20.0%;\"><b>Número do PTA / Exercício:</b></td>
							<td style=\"width:30%; text-align: left; \">{$arCabecalho['ptrcod']} / {$_SESSION['exercicio']}</td>
							<td style=\"text-align: right; width:20.0%;\"><b>CNPJ:</b></td>
							<td style=\"width:30%; text-align: left;\">{$arCabecalho['cnpj']}</td>
						</tr>
						<tr>
							<td style=\"text-align: right;\"><b>Nível de Ensino do PTA:</b></td>
							<td style=\"text-align: left;\">{$arCabecalho['resassunto']}</td>
							<td style=\"text-align: right;\"><b>Nome do Órgão ou Entidade:</b></td>
							<td style=\"text-align: left;\">{$arCabecalho['orgao_entidade']}</td>
						</tr>
						<tr>
							<td style=\"text-align: right;\"><b>Valor do PTA:</b></td>
							<td style=\"text-align: left;\">R$ ".number_format($arCabecalho['valor'],2,',','.')."</td>
							<td style=\"text-align: right;\"><b>Município / UF:</b></td>
							<td style=\"text-align: left;\">{$arCabecalho['endereco']}</td>
						</tr>
					 </tbody>
					</table>";

	switch ($tipo) {
		case 'D':
			$tipoN = 'Análise de Dados';
			break;
		case 'M':
			$tipoN = 'Análise de Mérito';
			break;
		case 'T':
			$tipoN = 'Análise de Técnica';
			break;
	}

	$sql = "SELECT DISTINCT
			    uni.unisigla
			FROM 
			    emenda.unidades uni
				inner join emenda.analise ana 
			    	on ana.uniid = uni.uniid 
			WHERE 
			    ana.ptrid = $ptrid
			    and ana.anastatus = 'A'
			    and ana.anatipo = '$tipo'
			    --and ana.uniid = {$arAnalise['uniid']}
			    and ana.analote = (SELECT max(analote) 
						    				FROM emenda.analise 
						                    WHERE anatipo = '$tipo'
						                    	and anastatus = 'A' 
						                    	and ptrid = ana.ptrid)";

	$dadosUnidades = $db->carregarColuna( $sql );
	$unidades = $dadosUnidades ? implode(", ", $dadosUnidades) : 'Não informado';

	$sql = "SELECT
				ini.ininome
			FROM
				emenda.planotrabalho ptr
			    inner join emenda.ptiniciativa pti
			    	on pti.ptrid = ptr.ptrid
			    inner join emenda.iniciativa ini
			    	on ini.iniid = pti.iniid
			WHERE
				ini.inistatus = 'A'
			    and pti.ptrid = $ptrid
			    and ptr.ptrstatus = 'A'";

	$arIninome = $db->carregar($sql);
	$arIninome = ( $arIninome ? $arIninome : array() );

	?>
	<table id="tblform1" width="95%" class="tabela" cellSpacing="1"
		cellPadding="3" align="center">
		<tr>
			<td><b>Tipo</b></td>
			<!--  <td><b>Data</b></td> -->
			<td><b>Unidade</b></td>
		</tr>
		<tr>
			<td><?php echo $tipoN; ?></td>
			<!-- <td><?php echo $arAnalise['anadataconclusao']; ?></td> -->
			<td><?php echo $unidades; ?></td>
		</tr>
	</table>
	<table id="tblform2" width="95%" class="tabela" cellSpacing="1"
		cellPadding="3" align="center">
		<tr>
			<td><b>Iniciativa</b></td>
		</tr>
		<?php
		foreach ($arIninome as $valor){
			echo '<tr>';
			echo '<td>'.$valor['ininome'].'</td>';
			echo '</tr>';
		}
		?>
	</table>
	<?php montaExecucaoOrcamentariaPopAnalise($ptrid); ?>
	<table id="tblform2" width="95%" class="tabela" cellSpacing="1"
		cellPadding="3" align="center">
		<tr>
			<td><b>Parecer</b></td>
		</tr>
		<tr>
			<td style="height: 300px;" valign="top"><?php echo $arAnalise['anaparecer']; ?></td>
		</tr>
		<tr>
			<td style="height: 20px;">&nbsp;</td>
		</tr>
		<?php
		if( $tipo != 'D' ){
			?>
		<tr>
			<td style="text-align: center;">
			<table width='90%' align='center'>
				<tr>
				<?php
				echo "<td width='50%' height='60' align='center'>_______________________________________________<br>$arAnalise[usunome]<br>(Técnico)</td>";
				
				$sql = "  SELECT
								unrid, 
								unrcargo,
								unrnome,
								unrordem
							FROM 
							  emenda.unidnomeresp res
							WHERE
								uniid = ".$arAnalise['uniid']."
							    and unrstatus = 'A'								
							ORDER BY
								unrordem";
				
				if( $arAnalise['uniid'] ) $dados = $db->carregar($sql);
				$dados = ($dados) ? $dados : array();
				$i = 0;
					
				if( $boReformulacao && $arAnalise['uniid'] == 11 ){
					echo "<td width='50%' height='60' align='center'>_______________________________________________<br>MARCUS VINICIUS LEAL GONÇALVES<br>(Técnico DIGAP/Emenda)</td></tr><tr>";
					echo "<td width='50%' height='60' align='center'>_______________________________________________<br>Renilda Peres de Lima<br>(Diretora da DIGAP)</td></tr>";
				} 
					
				foreach ($dados AS $k => $v){
					echo "<td width='50%' height='50' align='center'>_______________________________________________<br>$v[unrnome]<br>($v[unrcargo])</td>";

					if (!($i % 2)) echo "</tr><tr>";

					$i++;
				}
					
				?>
				</tr>
				<tr>
					<?
					$arTipos = pegaTipoReformulacao( $ptrid );
					$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
				
					if( $arTipos[0]['codigo'] == 3 && $resid == 1 ){
						echo "<td width='50%' height='60' align='center'>_______________________________________________<br>ELAINE DE SOUSA HENRIQUE<br>(Coordenadora-Geral de Planejamento e Orçamento das IFES)</td>";
						echo "<td width='50%' height='60' align='center'>_______________________________________________<br>ADRIANA RIGON WESKA<br>(Diretora de Desenvolvimento da Rede de Desenvolvimento de IFES)</td>";
					}
					?>
				</tr>
			</table>
			</td>
		</tr>
		<?php
}
?>
	</table>
	<?php
}

function montaCabecalhoHistoricoPTA($ptrid) {
	global $db;

	$processoconjur = $db->pegaLinha("SELECT prcnumsidoc, prcnomeinteressado, esddsc FROM conjur.processoconjur prc
									  LEFT JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
    								  LEFT JOIN workflow.documento doc ON doc.docid = esp.docid 
    								  LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid  
									  WHERE prc.prcid='".$prcid."'");

	// efetuar select e retornar cabecalho
	$titulo_modulo = "CONJUR";
	monta_titulo( $titulo_modulo,'Consultória Jurídica');

	echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Nº do Processo :</td><td>".$processoconjur['prcnumsidoc']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Interessado :</td><td>".$processoconjur['prcnomeinteressado']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Localização em andamento :</td><td>".$processoconjur['esddsc']."</td>";
	echo "</tr>";
	echo "</table>";
}

function recuperaArPTRES( $ptrid, $acaid, $pedid, $tipoEmenda ){

	global $db;
	/*$sql = "SELECT DISTINCT
				CASE WHEN exf.pliid is null THEN  pt.ptres ELSE pli.pliptres END AS codigo,
				CASE WHEN exf.pliid is null THEN pt.ptres  ELSE pli.pliptres END AS descricao
			FROM monitora.pi_planointerno pi
				inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
				inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
				left join emenda.execucaofinanceira exf on exf.pliid = pi.pliid
					and exf.exfstatus = 'A'
					and exf.ptrid = $ptrid
					and exf.pedid = $pedid
					and pi.plisituacao in ('S', 'C')
				left join monitora.planointerno pli on exf.pliid = pli.pliid
			WHERE pt.acaid = ".$acaid."
			union
			SELECT DISTINCT 
				 pi.pliptres as codigo,
				 pi.pliptres as descricao
			FROM
				 monitora.planointerno pi
				 inner join emenda.execucaofinanceira exf on exf.pliid = pi.pliid
				  and exf.exfstatus = 'A'
			WHERE exf.ptrid = $ptrid
			and exf.pedid = $pedid
	";*/
	if( $tipoEmenda == 'Complemento' ) $filtro = " and pi.pliano >= '".$_SESSION['exercicio']."'";
	else $filtro = " and pi.pliano = '".$_SESSION['exercicio']."'";
	
	$sql = "SELECT DISTINCT
				exf.exfid,
				pt.ptres AS codigo,
				pt.ptres AS descricao
			FROM monitora.pi_planointerno pi
				inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
				inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
				left join emenda.execucaofinanceira exf on exf.plicod = pi.plicod
					and exf.exfstatus = 'A'
					and exf.ptrid = $ptrid
					and exf.pedid = $pedid
			WHERE pt.acaid = $acaid
		    	and pi.plisituacao in ('S', 'C', 'A')
		    	$filtro";

	return $db->carregar($sql);
}



function verificaPublicacaoDOU($pmcid){
	global $db;
	if(!$pmcid){
		return false;
	}
	$sql = "SELECT pubid FROM emenda.ptpublicacao WHERE pmcid = $pmcid AND pubstatus = 'A' and pubtxtpublicacao is not null AND pubdatapublicacao IS NOT NULL";
	if($pubid = $db->pegaUm($sql)){
		return true;
	}else{
		return false;
	}

}

function verificaExclusaoIniciativaPTA($ptrid){
	global $db;

	$sql = "SELECT
				count(pmcnumconveniosiafi) as total
			FROM 
				emenda.ptminutaconvenio 
			WHERE 
				ptrid = $ptrid
				and pmcstatus = 'A' 
				--and pmcnumconveniosiafi is null";

	$total = $db->pegaUm($sql);

	if( $total == 0 ){
		return true;
	} else {
		return false;
	}
}

function enviarEmailAnexosConvenio($post){
	global $db;
	enviaEmailEntidadeConvenio( '', $_SESSION['emenda']['ptridAnalise'], $post['mailid'] );
}

/*function carregaParcelaCronograma($iniciativa, $prdminuta, $tipo){
 global $db;

 $sql = "SELECT
 pti.ptiid,
 pri.priid,
 pri.privalor,
 pti.ptivalorconcedente,
 pti.ptivalorproponente
 FROM
 emenda.v_ptiniciativa pti
 LEFT JOIN
 emenda.ptparcelainiciativa pri
 INNER JOIN emenda.ptparceladesembolso
 prd ON prd.prdid = pri.prdid
 AND prd.prdtipo = '$tipo'
 AND prd.prdminuta = '$prdminuta'
 ON pri.ptiid = pti.ptiid
 WHERE
 pti.ptiid = ".$iniciativa."
 ORDER BY
 prd.prddata ASC";

 return $db->carregar($sql);
 }*/

/*function carregaIniciativaCronograma(){
 global $db;

 // carrega as iniciativas
 $sql = "SELECT DISTINCT
 pti.ptiid,
 ini.ininome
 FROM
 emenda.iniciativa ini
 INNER JOIN
 emenda.ptiniciativa pti ON pti.iniid = ini.iniid
 AND pti.ptrid = ".($_SESSION["emenda"]["ptrid"] ? $_SESSION["emenda"]["ptrid"] : $_SESSION["emenda"]["ptridAnalise"])."";

 $iniciativas = $db->carregar($sql);
 $iniciativas = ( $iniciativas ? $iniciativas : array() );
 return $iniciativas;
 }*/


/**
 * Carrega a quantidade de parcela da minuta e do pagamento
 *
 * @param integer $ptrid
 * @return record
 */
/*function verificaParcelaMinutaPagamento($ptrid){
 global $db;
 $sql = "SELECT
 count(ppd.prdid)
 FROM
 emenda.ptparceladesembolso ppd
 WHERE
 ppd.ptrid = $ptrid
 and ppd.prdminuta = 'G'
 union
 SELECT
 count(ppd.prdid)
 FROM
 emenda.ptparceladesembolso ppd
 WHERE
 ppd.ptrid = $ptrid

 and ppd.prdminuta = 'M'";

 return $db->carregarColuna($sql);
 }*/

#excluir essa funcionalidade quando mudar a estrutura de pasta do arquivo gerenciar fluxos workflow
function carregaDadosEmpenhoPagamento( $ptrid ){
	global $db;

	$sql = "SELECT distinct
				vede.acaid, 
			    fup.fupfuncionalprogramatica, 
			    vede.emecod, 
			    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome, 
			    vede.gndcod, 
				vede.mapcod, 
				vede.foncod,
				(CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
			       ELSE 'Complemento' END) as tipoemenda,
				per.pedid, 
				tpe.tpedsc,
				--per.perid,
				sum(per.pervalor) as pervalor, 
				pli.plicod, 
				pli.plititulo, 
				pt.ptres as pliptres, 
				esf.esfcod, 
				esf.esfdsc,
				(CASE WHEN vede.mapcod = '90' THEN '0 - federal'
			    	  WHEN vede.mapcod = '30' THEN '1 - estadual'
			          WHEN vede.mapcod = '40' THEN '2 - Municipal'
			          WHEN (vede.mapcod = '50' or vede.mapcod = '60') THEN '3 - Particular'
			     ELSE '' END) as esferaadm,
                 (CASE WHEN vede.gndcod = '3' THEN '3.3.'||vede.mapcod||'.41'
                 	   WHEN vede.gndcod = '4' THEN '4.4.'||vede.mapcod||'.42'
                  END) as naturezaDesp,
                 exf.exfid,
                 exf.exfcodfontesiafi,
                 upf.fondsc,
                 exf.exfnumsolempenho,
                 exf.exfnumempenhooriginal,
                 exf.exfvalor,
                 sip.spgcodigo,
                 sip.spgdescricao
			FROM 
				emenda.ptiniciativa pti
				inner join emenda.ptiniciativaespecificacao pte
			  		on pte.ptiid = pti.ptiid
			  		and pte.ptestatus = 'A'
				inner join emenda.ptiniesprecurso per
			  		on per.pteid = pte.pteid
				inner join emenda.ptemendadetalheentidade ped
			  		on ped.pedid = per.pedid
				inner join emenda.tipoensino tpe
					on pti.tpeid = tpe.tpeid
				inner join emenda.v_emendadetalheentidade vede
			  		on vede.edeid = ped.edeid
				inner join emenda.autor aut
			  		on aut.autid = vede.autid
				inner join emenda.v_funcionalprogramatica fup
			  		on fup.acaid = vede.acaid
				inner join emenda.execucaofinanceira exf
			  		on pti.ptrid = exf.ptrid
			  		and ped.pedid = exf.pedid 			        
				inner join monitora.pi_planointerno pli on pli.plicod = exf.plicod
                inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
                inner join monitora.ptres pt on pt.ptrid = plpt.ptrid and pt.ptres = exf.ptres
				inner join public.esfera esf 
			    	on fup.esfcod = esf.esfcod 
                left join financeiro.unidadeptresfonte upf
                	on upf.fontesiafi = exf.exfcodfontesiafi
                left join emenda.ordembancaria orb
                	left join emenda.situacaopagamento sip on sip.spgcodigo = orb.spgcodigo
                on orb.exfid = exf.exfid and orb.spgcodigo not in ('2','9')
			where 
				pti.ptrid = $ptrid 
                and exf.exfstatus = 'A'
                --and (orb.spgcodigo <> '2' or orb.spgcodigo is null)
                
			GROUP BY 
				per.pedid,
				tpe.tpedsc, 
				--per.perid,
				vede.emecod, 
				aut.autnome, 
				vede.emerelator,
				vede.gndcod, 
				vede.mapcod, 
				vede.foncod,
				tipoemenda, 
				fup.fupfuncionalprogramatica, 
				vede.acaid, 
				pli.plicod, 
				pli.plititulo, 
				pt.ptres, 
				esf.esfcod, 
				esf.esfdsc,
				exf.exfid,
				exf.exfcodfontesiafi,
				exf.exfnumsolempenho,
				exf.exfnumempenhooriginal,
				exf.exfvalor,
                upf.fondsc,
                sip.spgdescricao,
                sip.spgcodigo
			ORDER BY
				exf.exfid, 
				per.pedid, 
				tpe.tpedsc, 
				vede.emecod, 
				vede.gndcod, 
				vede.mapcod, 
				vede.foncod, 
				fup.fupfuncionalprogramatica, 
				vede.acaid";

	return $db->carregar($sql);
}

function somaDiasData($dias, $data){
	$arData = explode('/', $data);

	$dia = $arData[0];
	$mes = $arData[1];
	$ano = $arData[2];
	$dataFinal = mktime(24*$dias, 0, 0, $mes, $dia, $ano);
	$dataFormatada = date('d/m/Y',$dataFinal);
	return $dataFormatada;
}

function carregaOrdemBancariaPagamento( $orbid ){
	global $db;

	$sql = "SELECT
			  	ord.orbid,
			  	ord.exfid,
			  	ord.orbmesparcela,
			  	ord.orbanoparcela,
			  	ord.orbvalorparcela,
			  	ord.orbnumsolicitacao,
			  	sip.spgdescricao,
			  	exf.exfnumempenhooriginal,
				ord.orbnumordembancaria,
			  	ord.orbvalorpagamento,
			  	to_char(ord.orbdataemissao, 'DD/MM/YYYY HH24:MI:SS') as orbdataemissao,
			  	to_char(ord.orbdatapagamento, 'DD/MM/YYYY HH24:MI:SS') as orbdatapagamento,
			  	usu.usunome as usucpf,
			  	to_char(ord.orbdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as orbdatainclusao,
			  	to_char(ord.orbdataalteracao, 'DD/MM/YYYY HH24:MI:SS') as orbdataalteracao 
			FROM 
			  	emenda.ordembancaria ord
			    inner join emenda.situacaopagamento sip
			    	on sip.spgcodigo = ord.spgcodigo
			    inner join seguranca.usuario usu
    				on usu.usucpf = ord.usucpf
    			inner join emenda.execucaofinanceira exf
    				on exf.exfid = ord.exfid
			WHERE
				ord.orbid = $orbid";

	return $db->pegaLinha( $sql );
}

function pegaTipoReformulacao( $ptrid ){
	global $db;

	$refprorrogacaooficio = $db->pegaUm("select refprorrogacaooficio from emenda.ptminreformulacao where ptrid = $ptrid and refstatus = 'A'");

	$arTipos = array();
	$ptridAtual = $ptrid;
	$sql = "SELECT distinct
			    p.ptrid,
			    ((SELECT sum(pedvalor) FROM emenda.ptemendadetalheentidade pt1
                	inner join emenda.v_emendadetalheentidade ve1 on ve1.edeid = pt1.edeid WHERE ptrid = p.ptrid and ve1.edestatus = 'A') + p.ptrvalorproponente) as totalPTA,
			    p.ptrvalorproponente as ptrvalorproponente,
			    (select sum(e.ptequantidade) from emenda.ptiniciativaespecificacao e
				inner join emenda.ptiniciativa pti on pti.ptiid = e.ptiid where pti.ptrid = p.ptrid and e.ptestatus = 'A' ) as ptequantidade,
			    (select sum(e.ptevalorproponente) from emenda.ptiniciativaespecificacao e 
				inner join emenda.ptiniciativa pti on pti.ptiid = e.ptiid where pti.ptrid = p.ptrid and e.ptestatus = 'A' ) as ptevalorproponente,
			    (select sum(e.ptevalorunitario) from emenda.ptiniciativaespecificacao e 
				inner join emenda.ptiniciativa pti on pti.ptiid = e.ptiid where pti.ptrid = p.ptrid and e.ptestatus = 'A') as ptevalorunitario,
			    ptv.vigdatainicio,
			    ptv.vigdatafim,
			    ptv.vigtipo,
			    (SELECT sum(pedvalor) FROM emenda.ptemendadetalheentidade pt
                	inner join emenda.v_emendadetalheentidade ve on ve.edeid = pt.edeid WHERE ptrid = p.ptrid and ve.edestatus = 'A') as pedvalor,
			    (SELECT count(1) FROM emenda.ptiniciativaespecificacao e
				inner join emenda.ptiniciativa pti on pti.ptiid = e.ptiid where pti.ptrid = p.ptrid and e.ptestatus = 'A') as especificacao,
				
			    (SELECT sum(pib.ptbquantidaderural) + sum(pib.ptbquantidadeurbana) as total FROM emenda.ptiniciativabeneficiario pib
			        inner join emenda.iniciativabeneficiario ib on ib.icbid = pib.icbid and ib.icbstatus = 'A'
			        inner join emenda.ptiniciativa pti on pib.ptiid = pti.ptiid where pti.ptrid = p.ptrid) as beneficiario
			     
			FROM 
			    emenda.planotrabalho p
			    left join emenda.ptvigencia ptv on ptv.ptrid = p.ptrid and ptv.vigstatus = 'A' and ptv.vigtipo <> 'A'
			WHERE
				--(p.ptrid = (select ptridpai from emenda.planotrabalho where ptrid = $ptrid) or p.ptrid = $ptrid)
				 p.ptrcod = (select ptrcod from emenda.planotrabalho where ptrid = $ptrid)
			    and p.ptrid not in (SELECT ptrid FROM emenda.ptminreformulacao WHERE refsituacaoreformulacao = 'E' 
			    						and ptrid in ( SELECT p.ptrid FROM emenda.planotrabalho p 
			                            				WHERE ptrcod = (SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = $ptrid)))
			/*GROUP BY
				p.ptrid,
			    ptv.vigdatainicio,
			    pti.ptiid,
			    p.ptrvalorproponente,
			    ptv.vigdatafim,
			    ptv.vigtipo,
			    ptv.vigstatus,
			    ptv.vigid*/
			ORDER BY
			    p.ptrid desc
			limit 2";
	//ver($sql,d);
	$arDados = $db->carregar( $sql );
	$arDados = ( empty($arDados) ? array() : $arDados );
	sort( $arDados );

	$ptrid = array();
	$ptrvalorproponente = array();
	$ptequantidade = array();
	$ptevalorproponente = array();
	$ptevalorunitario = array();
	$vigdatainicio = array();
	$pedvalor = array();
	$especificacao = array();
	$beneficiario = array();
	$vigdatafim = array();
	$arTotal = array();

	$boAlteracaoValor = false;
	$boSuplementacaoRecurso = false;
	$boProrrogacaoVigencia = false;
	$boAlteracaoConvenio = false;
	$boReformulacao = false;

	$boV_Proponente = false;
	$boV_concedente = false;
	$boV_Quantidade = false;
	$boV_Unitario = false;
	$bo_MesAno = false;
	$bo_Beneficiario = false;
	$bo_Vigdatafim = false;

	foreach ($arDados as $key => $v) {
		array_push( $ptrid, $v['ptrid'] );
		array_push( $arTotal, $v['totalpta'] );
		array_push( $ptrvalorproponente, $v['ptrvalorproponente'] );
		array_push( $ptequantidade, $v['ptequantidade'] );
		array_push( $ptevalorproponente, $v['ptevalorproponente'] );
		array_push( $ptevalorunitario, $v['ptevalorunitario'] );
		array_push( $vigdatainicio, $v['vigdatainicio'] );
		array_push( $pedvalor, $v['pedvalor'] );
		array_push( $especificacao, $v['especificacao'] );
		array_push( $beneficiario, $v['beneficiario'] );
		array_push( $vigdatafim, $v['vigdatafim'] );
	}

	if( $ptrvalorproponente[0] != $ptrvalorproponente[1] ){
		$boV_Proponente = true;
	}
	if( $pedvalor[0] != $pedvalor[1] ){
		$boV_concedente = true;
	}
	/*if( ( ($especificacao[0] != $especificacao[1]) || ($ptequantidade[0] != $ptequantidade[1]) ||
	 ($ptevalorunitario[0] != $ptevalorunitario[1]) || ($ptevalorproponente[0] != $ptevalorproponente[1]) ) &&
	 !$boV_concedente && !$boV_Proponente ){
		$boReformulacao = true;
		}*/
	//		ver($vigdatainicio);
	if( !empty($vigdatafim[1]) && ( ($vigdatainicio[0] != $vigdatainicio[1]) || ($vigdatafim[0] != $vigdatafim[1]) ) ){
		$boProrrogacaoVigencia = true;
	}
	
	if( ( ($especificacao[0] != $especificacao[1]) || ($ptequantidade[0] != $ptequantidade[1]) ||
			($ptevalorunitario[0] != $ptevalorunitario[1]) || ($ptevalorproponente[0] != $ptevalorproponente[1]) ) &&
	!$boV_concedente && !$boV_Proponente && ($arTotal[0] == $arTotal[1]) ){
		$boReformulacao = true;
	}
	if( ($beneficiario[0] != $beneficiario[1]) ){
		$bo_Beneficiario = true;
	}

	if( $boV_Proponente && $boV_concedente ){
		$boAlteracaoValor = true;
	} else if( $boV_Proponente ){
		$boAlteracaoConvenio = true;
	} else if( $boV_concedente ){
		$boSuplementacaoRecurso = true;
	}
	
	$arrTipos = verificaTiposReformulacao($ptridAtual);
	$arrTipos = $arrTipos ? $arrTipos : array();
	 
	foreach ($arrTipos as $v) {
		array_push( $arTipos, array("codigo" => $v['codigo'], "descricao" => $v['descricao'] ));
	}

	if( $boAlteracaoConvenio ) array_push( $arTipos, array("codigo" => 1, "descricao" => 'Alteração de Cláusula de Convênio' ));
	if( $boAlteracaoValor ) array_push( $arTipos, array("codigo" => 2, "descricao" => 'Alteração de Valor' ) );
	if( $boProrrogacaoVigencia && $refprorrogacaooficio == 'N' ) array_push( $arTipos, array("codigo" => 3, "descricao" => 'Prorrogação de Vigência' ) );
	if( $boReformulacao ) array_push( $arTipos, array("codigo" => 4, "descricao" => 'Reformulação' ) );
	if( $boSuplementacaoRecurso ) array_push( $arTipos, array("codigo" => 5, "descricao" => 'Suplementação de Recursos' ) );
	if( $refprorrogacaooficio == 'S' ) array_push( $arTipos, array("codigo" => 8, "descricao" => 'Prorrogração de Ofício' ) );

	return $arTipos;
}



function insereFilhosPTA( $ptridPai, $boPai = true ){
	global $db;
	
	include_once APPRAIZ.'emenda/classes/planoTrabalhoDAO.class.inc';
	$obPTA = planoTrabalhoDAO::getInstance();
	
	#Tabela planotrabalho
	$ptridFilho = $obPTA->inserePlanoTrabalhoFilhoDAO( $ptridPai );
		
	$arrTiposRef = verificaTiposReformulacao( $ptridPai, 'codigo' );
	$arrTiposRef = $arrTiposRef ? $arrTiposRef : array();
	
	if( $ptridFilho ){
		#Tabela ptemendadetalheentidade
		//$obPTA->inserePtemendadetalheentidadeFilhoDAO( $ptridFilho, $ptridPai);

		$sql = "SELECT ptiid FROM emenda.ptiniciativa WHERE ptrid = $ptridPai";
		$arPtiniciativa = $obPTA->carregar( $sql );
		$arPtiniciativa = ( $arPtiniciativa ? $arPtiniciativa : array() );

		$sql = "SELECT prdid FROM emenda.ptparceladesembolso WHERE ptrid = $ptridPai";
		$arPtParcela = $obPTA->carregar( $sql );
		$arPtParcela = ( $arPtParcela ? $arPtParcela : array() );

		$sql = "SELECT pedid, ptrid, edeid, pedvalor FROM emenda.ptemendadetalheentidade WHERE ptrid = $ptridFilho";
		$arDadosEntidade = $obPTA->carregar( $sql );
		
		#Tabela ptemendadetalheentidade
		$sql = "SELECT pe.edeid, pe.pedvalor, pr.perid, pr.pedid, pr.pervalor, pr.pteid FROM emenda.ptemendadetalheentidade pe
				inner join emenda.ptiniesprecurso pr on pr.pedid = pe.pedid WHERE ptrid = $ptridPai order by pr.pedid";
		$arDados = $obPTA->carregar( $sql );

		$arInespRecurso = array();
		foreach ($arPtiniciativa as $vIni) {

			$ptiidPai = $vIni['ptiid'];
			#Tabela ptiniciativa
			$ptiid = $obPTA->inserePtiniciativaFilhoDAO( $ptridFilho, $ptiidPai, $ptridPai );
				
			#Tabela ptiniciativaEspecificacao
			$arPtespecificacao = $obPTA->carregar( "SELECT pteid FROM emenda.ptiniciativaespecificacao WHERE ptiid = $ptiidPai and ptestatus = 'A'");
			$arPtespecificacao = ( $arPtespecificacao ? $arPtespecificacao : array() );
			
			foreach ($arPtespecificacao as $vEsp) {
				$pteidPai = $vEsp['pteid'];

				$pteidNovo = $obPTA->inserePtiniciativaEspecificacaoFilhoDAO( $ptiid, $pteidPai );

				#Tabela ptitemkit
				$obPTA->inserePtitemkitFilhoDAO( $pteidNovo, $pteidPai );

				#Tabela ptiniesprecurso
				$perid = $obPTA->inserePtiniesprecursoFilhoDAO( $pteidNovo, $pteidPai );
				if( !empty($perid) ){
					$sql = "SELECT perid, pteid, pedid, pervalor FROM emenda.ptiniesprecurso WHERE pteid = $pteidNovo order by pedid";
					$arInesp = $obPTA->carregar( $sql );
					foreach ($arInesp as $v) {
						array_push( $arInespRecurso, array("perid" => $v['perid'], "pteid" => $v['pteid'], "pedid" => $v['pedid'], "pervalor" => $v['pervalor'], "pteidPai" => $pteidPai) );
					}
				}
			}
			
			$arPtespecRendi = $obPTA->carregar( "SELECT perid FROM emenda.ptaespecificacaorendimento WHERE ptiid = $ptiidPai and perstatus = 'A'" );
			$arPtespecRendi = ( $arPtespecRendi ? $arPtespecRendi : array() );
			
			foreach ($arPtespecRendi as $vEsp) {
				$peridRPai = $vEsp['perid'];
				
				$sql = "INSERT INTO emenda.ptaespecificacaorendimento(ptiid, iceid, perquantidade, pervalorunitario, pervalorrendimento, perdatainicio, perdatafim, perstatus) 
						(SELECT $ptiid, iceid, perquantidade, pervalorunitario, pervalorrendimento, max(perdatainicio), max(perdatafim), perstatus 
							FROM emenda.ptaespecificacaorendimento WHERE perid = $peridRPai and perstatus = 'A'
						GROUP BY iceid, perquantidade, pervalorunitario, pervalorrendimento, perstatus) RETURNING perid";
				
				$perid = $db->pegaUm( $sql );
				
				$sql = "INSERT INTO emenda.ptitemkit(perid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario)
					(SELECT $perid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario
					 FROM emenda.ptitemkit
					 WHERE perid = $peridRPai )";
				$db->executar( $sql );
			}
				
			#Tabela ptiniciativabeneficiario
			$obPTA->inserePtiniciativabeneficiarioFilhoDAO( $ptiid, $ptiidPai );
				
			$sql = "SELECT pd.prdid FROM emenda.ptparceladesembolso  pd
						inner join emenda.ptparcelainiciativa pi
					    on pi.prdid = pd.prdid 
					WHERE pd.ptrid = $ptridPai and pi.ptiid = $ptiidPai";
				
			$arPtParcela = $obPTA->carregar( $sql );
			$arPtParcela = ( $arPtParcela ? $arPtParcela : array() );
				
			foreach ($arPtParcela as $key => $v) {
				$prdidPai = $v['prdid'];
				unset($arPtParcela[$key]);
				#Tabela ptparceladesembolso
				$prdid = $obPTA->inserePtparceladesembolsoFilhoDAO( $ptridFilho, $ptridPai, $prdidPai );

				#Tabela ptparcelainiciativa
				$obPTA->inserePtparcelainiciativaFilhoDAO( $prdid, $ptiid, $prdidPai, $ptiidPai );
			}
		}
		
		$arPedid = array();
		foreach ($arDados as $v) {
			if( !in_array( $v['pedid'], $arPedid ) ){
				$sql = "INSERT INTO emenda.ptemendadetalheentidade( ptrid, edeid, pedvalor)
						VALUES( {$ptridFilho}, {$v['edeid']}, {$v['pedvalor']}) RETURNING pedid";

				$pedid = $obPTA->pegaUm( $sql );
				array_push( $arPedid, $v['pedid'] );
				
				//if( !in_array(APOSTILAMENTO, $arrTiposRef) ){
					$exfid = $obPTA->insereExecucaoFinanceiraFilhoDAO( $ptridFilho, $ptridPai, $pedid, $v['pedid'] );
					if( !empty($exfid) ){
						$obPTA->insereptExecfinanceiraHistoricoFilhoDAO( $exfid, $ptridPai, $v['pedid'] );
						$sql = "SELECT exfid FROM emenda.execucaofinanceira WHERE ptrid = $ptridPai and pedid = ".$v['pedid'];
						$arExfid = $obPTA->carregarColuna($sql);
						foreach ($arExfid as $exfidpai) {
							$orbid = $obPTA->insereOrdembancariaFilhoDAO( $exfid, $exfidpai );
							//if( !empty($orbid) ) insereOrdembanchistoricoFilhoDAO( $orbid, $orbidpai );
						}
					}
				//}
			}

			foreach ($arInespRecurso as $key => $ar) {
				if( $ar['pteidPai'] == $v['pteid'] && $ar['pedid'] == $v['pedid'] ){
					$sql = "UPDATE emenda.ptiniesprecurso SET pedid = {$pedid} WHERE pteid = ".$ar['pteid']." and perid = ".$ar['perid']." and pedid = ".$ar['pedid'];
					$obPTA->executar( $sql );
				}
			}
		}
		
		#Tabela ptescolasbeneficiadas
		$obPTA->inserePtescolasbeneficiadasFilhoDAO( $ptridFilho, $ptridPai );

		$pmcid = $obPTA->inserePtminutaConvenioFilhoDAO( $ptridFilho, $ptridPai );
		$pmcid = $pmcid ? $pmcid : 'null';
		$obPTA->insereIntervenienteConvenioFilhoDAO( $pmcid, $ptridPai );

		$refid = $obPTA->insereptMinutaReformulacaoFilhoDAO( $ptridFilho, $ptridPai, $pmcid );

		//if( $pmcid )
		//$obPTA->inserePtvigenciaFilhoDAO( $ptridFilho, $ptridPai, $pmcid, $refid );
		$obPTA->inserePtvigenciaFilhoDAO( $ptridFilho, $ptridPai, $pmcid, $arrTiposRef );

		/*$refid = $refid ? $refid : 'null';
		 $obPTA->insereptReformulatiposFilhoDAO( $refid, $ptridPai, $ptridFilho );*/
		
		//$obPTA->insereptPtpublicacaoFilhoDAO( $refid, $pmcid, $ptridPai );
		$anaid = $obPTA->insereptAnaliseFilhoDAO( $ptridFilho, $ptridPai, $arrTiposRef );
			
		$cocid = $obPTA->insereContaCorrenteFilhoDAO( $ptridFilho, $ptridPai );
		if( !empty($cocid) ) $obPTA->insereptContaCorrenteHistoricoFilhoDAO( $cocid, $ptridPai );

		$mailid = $obPTA->inserePtmailFilhoDAO( $ptridFilho, $ptridPai );
		$mailid = $mailid ? $mailid : 'null';
		$anaid = $anaid ? $anaid : 'null';
		
		$obPTA->insereAnexoFilhoDAO( $ptridFilho, $anaid, $mailid, $ptridPai, $refid );
		
		$db->executar("update emenda.ptminreformulacao set refstatus = 'A' where ptrid = {$ptridPai} and refstatus = 'I'");
		
		if(!$db->commit()){
			$db->rollback();
			return false;
		} else {
			return $ptridFilho;
		}

	} else {
		return false;
	}
}

function insereTiposReformulacao( $ptrid, $refid, $bosubmit = false ){
	global $db;
	$arTipos = pegaTipoReformulacao( $ptrid );
	
	foreach ($arTipos as $v) {
		$total = $db->pegaUm("SELECT count(rftid) FROM emenda.reformulatipos WHERE trefid = {$v['codigo']} and refid = $refid");
		if( (int)$total == 0 ){
			$sql = "INSERT INTO emenda.reformulatipos( trefid, refid)
					VALUES ( {$v['codigo']}, $refid)";
			$db->executar( $sql );
		}
	}
	if( $bosubmit )
	return $db->commit();
}

/*function montaTiposReformulacao( $ptrid, $status = '' ){
 global $db;

 $status = ( !empty($status) ? " and ptmr.refstatus = 'A'" : '' );

 $sql = "SELECT
 ptmr.refdsc,
 CASE WHEN ptmr.refstatus = 'A' THEN 'Ativo' ELSE 'Inativo' END as status,
 CASE WHEN ptmr.refsituacaoreformulacao = 'C' THEN 'Em andamento'
 WHEN ptmr.refsituacaoreformulacao = 'F' THEN 'Finalizado' END as situacao,
 ptmr.refsituacao, ptmr.refstatus,
 ptmr.refid, to_char(ptmr.refdatainclusao,'dd/mm/YYYY HH24:MI') AS refdatainclusao
 FROM emenda.ptminreformulacao ptmr WHERE ptmr.ptrid = $ptrid";

 $arDados = $db->carregar($sql);

 $arRegistro = array();
 if ($arDados) {
 $boHabilita = false;
 foreach ($arDados as $key => $dados) {

 $estado = pegarEstadoAtual( $ptrid );

 if( $dados['refsituacao'] == 'A' && $dados['refstatus'] == 'A' && $estado != EM_ANALISE_PROCESSO_REF && $estado != EM_ANALISE_TECNICA_REFORMULACAO && $estado != EM_EMPENHO_REFORMULACAO  ){
 $boHabilita = true;
 if( $estado == EM_SOLICITACAO_REFORMULACAO ){
 $acao = "<center><img src='../imagens/alterar.gif' style='cursor:pointer;' title=\"Editar\" onclick=\"window.open('emenda.php?modulo=principal/reformulacao&acao=A&refid={$dados['refid']}','_top')\">
 <img src='../imagens/excluir.gif' style='cursor:pointer;' title=\"Excluir\" onclick=\"if (confirm('Tem certeza que deseja exluir esta reformulação?')) window.open('emenda.php?modulo=principal/reformulacao&acao=A&action=E&refid={$dados['refid']}&refsituacao={$dados['refsituacao']}','_top')\"></center>";
 } elseif( $estado == EM_REFORMULACAO ){
 $acao = "<center><img src='../imagens/editar_nome.gif' style='cursor:pointer;' onclick=\"window.open('emenda.php?modulo=principal/reformulacaoPTA&acao=A&ptrid={$ptrid}','','width=900,height=600,scrollbars=1')\">
 <img src='../imagens/alterar_01.gif' style='cursor:pointer;' title=\"Editar\">
 <img src='../imagens/excluir.gif' style='cursor:pointer;' title=\"Excluir\" onclick=\"if (confirm('Tem certeza que deseja exluir esta reformulação?')) window.open('emenda.php?modulo=principal/reformulacao&acao=A&action=E&refid={$dados['refid']}&refsituacao={$dados['refsituacao']}','_top')\"></center>";
 } else {
 $acao = "<center><img src='../imagens/alterar_01.gif' style='cursor:pointer;' title=\"Editar\">
 <img src='../imagens/excluir.gif' style='cursor:pointer;' title=\"Excluir\" onclick=\"if (confirm('Tem certeza que deseja exluir esta reformulação?')) window.open('emenda.php?modulo=principal/reformulacao&acao=A&action=E&refid={$dados['refid']}&refsituacao={$dados['refsituacao']}','_top')\"></center>";
 }
 } else {
 $acao = "<center><img src='../imagens/alterar_01.gif' style='cursor:pointer;' title=\"Editar\">
 <img src='../imagens/excluir_01.gif' title=\"Excluir\" style='cursor:pointer;'></center>";
 }

 $arRegistro[$key] = array(
 'acao' => $acao,
 'refdsc' => $dados['refdsc'],
 'datainclusao' => $dados['refdatainclusao'],
 'status' => $dados['status'],
 'situacao' => $dados['situacao'],
 );
 }
 }
 return $arRegistro;
 }*/

function montaTiposReformulacao( $ptrid, $status = '' ){
	global $db;

	$status = ( !empty($status) ? " and ptmr.refstatus = 'A'" : '' );
	$sql = "select 
			    array_to_string(array(SELECT tr.trefnome
			                          FROM emenda.reformulatipos rt 
			                              inner join emenda.tiporeformulacao tr on tr.trefid = rt.trefid and tr.trefstatus = 'A'
			                          WHERE 
			                              rt.refid = ptmr.refid), ', <br>') as descricao,
			    to_char(ptmr.refdatainclusao,'dd/mm/YYYY HH24:MI') AS refdatainclusao,
				case when ptmr.refstatus = 'A' then 'Ativo' else 'Inativo' end as status,
			    case when ptmr.refsituacaoreformulacao = 'C' then 'Em andamento'
			    when ptmr.refsituacaoreformulacao = 'F' then 'Finalizado' else 'Excluído' end as situacao,
			    ptmr.refsituacao,
			    ptmr.refstatus,
			    ptmr.refprorrogacaooficio,			    
			    ptmr.refid, 
			    ptmr.ptrid, 
			    ptmr.refsituacaoreformulacao 
			from 
				emenda.ptminreformulacao ptmr 
			where ptmr.ptrid in (select ptrid from emenda.planotrabalho where ptrcod = (select ptrcod from emenda.planotrabalho where ptrid = $ptrid) /*and ptridpai is not null*/)  $status
			order by ptmr.refsituacaoreformulacao = 'C' desc, ptmr.refid desc";
	#and ptmr.refsituacaoreformulacao = 'F'

	$arDados = $db->carregar($sql);
	
	$arRegistro = array();
	if ($arDados) {
		$trefalterpta = 2;
		$boHabilita = false;
		foreach ($arDados as $key => $dados) {			
			/*$sql = "select * from emenda.reformulatipos a, emenda.tiporeformulacao b where (a.refid = '{$dados[refid]}') and (a.trefid = b.trefid)";
			$arTipos = $db->carregar($sql);*/
			
			unset($htmlTipos);
			if ($dados['descricao']) {
				/*foreach ($arTipos as $tipo) {
					if( empty($htmlTipos) ){ 
						$htmlTipos = "<b>{$tipo['trefnome']}</b>";
					} else { 
						$htmlTipos .= ",<br><b>{$tipo['trefnome']}</b>";
					}
	
					if ($arTipos['trefalterpta'] == 'S') $trefalterpta = 1;	
				}*/
				$htmlTipos = "<b>{$dados['descricao']}</b>";
			} else {
				if( $dados['refprorrogacaooficio'] == 'S' ) $htmlTipos .= "<b>Prorrogração de Ofício</b>";
			}

			$estado = pegarEstadoAtual( $ptrid );

			if( $dados['refsituacao'] == 'A' && $dados['refstatus'] == 'A' && $dados['refsituacaoreformulacao'] != 'E' && $estado != EM_ANALISE_PROCESSO_REF && $estado != EM_ANALISE_TECNICA_REFORMULACAO && $estado != EM_EMPENHO_REFORMULACAO ){
				$boHabilita = true;
				if( $estado == EM_SOLICITACAO_REFORMULACAO ){
					$acao = "<center>
	    						<img src='../imagens/alterar.gif' style='cursor:pointer;' title=\"Editar\" onclick=\"window.open('emenda.php?modulo=principal/reformulacao&acao=A&refid={$dados['refid']}','_top')\">
							 	<img src='../imagens/excluir.gif' style='cursor:pointer;' title=\"Excluir\" onclick=\"if (confirm('Tem certeza que deseja exluir esta reformulação?')) window.open('emenda.php?modulo=principal/reformulacao&acao=A&action=E&refid={$dados['refid']}&refsituacao={$dados['refsituacao']}','_top')\">
							 </center>";
				} elseif( $estado == EM_REFORMULACAO ){
					//<img src='../imagens/excluir.gif' style='cursor:pointer;' title=\"Excluir\" onclick=\"if (confirm('Tem certeza que deseja exluir esta reformulação?')) window.open('emenda.php?modulo=principal/reformulacao&acao=A&action=E&refid={$dados['refid']}&refsituacao={$dados['refsituacao']}','_top')\">
					$acao = "<center>
		    				 	<img src='../imagens/alterar_01.gif' style='cursor:pointer;' title=\"Editar\">
							 	<img src='../imagens/excluir_01.gif' title=\"Excluir\">
							 </center>";
				} else {
					//<img src='../imagens/excluir.gif' style='cursor:pointer;' title=\"Excluir\" onclick=\"if (confirm('Tem certeza que deseja exluir esta reformulação?')) window.open('emenda.php?modulo=principal/reformulacao&acao=A&action=E&refid={$dados['refid']}&refsituacao={$dados['refsituacao']}','_top')\">
					$acao = "<center>
		    					<img src='../imagens/alterar_01.gif' style='cursor:pointer;' title=\"Editar\">
							 	<img src='../imagens/excluir_01.gif' title=\"Excluir\">
							</center>";
				}
			} else {
				$acao = "<center><img src='../imagens/alterar_01.gif' title=\"Editar\">
						 <img src='../imagens/excluir_01.gif' title=\"Excluir\"></center>";
			}
			if( $_SESSION['usucpf'] == '' ){
				$arRegistro[$key] = array(
    								'acao' => $acao,
									'pta' => $dados['ptrid'],
									'refid' => $dados['refid'],
    								'tiporef' => $htmlTipos,
    								'datainclusao' => $dados['refdatainclusao'],
    								'status' => $dados['status'],
    								'situacao' => $dados['situacao']
									);
			} else {
				$arRegistro[$key] = array(
    								'acao' => $acao,
    								'tiporef' => $htmlTipos,
    								'datainclusao' => $dados['refdatainclusao'],
    								'status' => $dados['status'],
    								'situacao' => $dados['situacao']
									);
			}

			//echo "<tr><td width='40' style='cursor:pointer;'><img src='../imagens/alterar.gif' onclick=\"window.open('emenda.php?modulo=principal/reformulacao&acao=A&refid=$dados[refid]','_top')\"> <img src='http://simec-local/imagens/excluir.gif' onclick=\"if (confirm('Tem certeza?')) window.open('emenda.php?modulo=principal/reformulacao&acao=A&action=E&refid=$dados[refid]&refsituacao=$dados[refsituacao]','_top')\"></td><td>$htmlTipos</td><td>- $dados[refdatainclusao]</td></tr>";
		}
	}
	return $arRegistro;
}





/**
 * Função que verifica se o plano de trabalho esta em reformulação
 *
 * @param integer $ptrid
 */
#não excluir
function pegaPaiPTA( $ptrid ){
	global $db;
	if( $ptrid ){
		$ptridpai =  $db->pegaUm( "SELECT ptridpai FROM emenda.planotrabalho WHERE ptrid = $ptrid" );
		return $ptridpai;
	} else {
		return false;
	}
}

function montaVisualizarPTA( $ptrid, $boHistoricoPTA = false, $boReformulacao = false  ){

	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

	if( !$boReformulacao )
	$boReformulacao = pegaPaiPTA( $ptrid );

	$label = 'Visualizar PTA';
	if( !empty($boReformulacao) ){
		$label = 'Visualizar PTA Reformulado';
	}
	$html = '';
	if($boHistoricoPTA && (in_array( ANALISTA_FNDE, $arPerfil) || in_array( SUPER_USUARIO, $arPerfil)) ){
		$html = '<td align="right">
					<img src="/imagens/consultar.gif" style="cursor:pointer;" onclick="visualizarHistoricoPTA( '. $_SESSION["emenda"]["ptridAnalise"] .' );">
					<a style="cursor:pointer;" onclick="visualizarHistoricoPTA( '. $_SESSION["emenda"]["ptridAnalise"] .' );"> Visualizar Histórico PTA </a>
				</td>';
	}

	echo '<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr bgcolor="#D0D0D0">
			<td>
				<img src="/imagens/consultar.gif" style="cursor:pointer;" onclick="visualizarPTA( '. $ptrid .' );">
				<a style="cursor:pointer;" onclick="visualizarPTA( '. $ptrid .' );"> '.$label.' </a>
			</td>
			'.$html.'
		</tr>
	  </table>';
}

function encaminharRepublicacaoDOU( $url, $ptrid ){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/minutaConvenioDOU&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function encaminharLiberarRecurso( $url, $ptrid ){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/solicitarPagamentoFNDE&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}



function validaEnvioAssinaturaConcedente( $url, $ptrid ){
	$boPai = pegaPaiPTA( $ptrid );

	if( !empty($boPai) || !strstr( $url, 'emenda.php?modulo=principal/assinaturasPTA&acao=A' ) ){
		return false;
	} else {
		return true;
	}
}

function alteraSituacaoReformulacao( $url, $ptrid ){
	global $db;

	$boPai = pegaPaiPTA( $ptrid );

	if( !empty( $boPai ) ){
		$sql = "UPDATE
				  emenda.ptminreformulacao
				SET 
				  refsituacaoreformulacao = 'F'
				WHERE 
				  ptrid = {$ptrid} AND refsituacaoreformulacao = 'C'";

		$db->executar( $sql );
		return $db->commit();
	} else {
		return true;
	}
}


function montaAbasPTA( $abacod_tela, $url, $parametros = '', $estadoAtual = '', $sisid = '57' ){
	global $db;
	$arMnuid = array();
	$arEstadoTrava = array(EM_SOLICITACAO_REFORMULACAO, EM_ACEITACAO_REFORMULACAO, RECURSO_LIBERADO, EM_LIBERACAO_RECURSO, EM_ANALISE_TECNICA, EM_PROCESSO_REFORMULADO);
	/*if( $_SESSION['emenda']['federal'] == 'S' ){
		array_push($arMnuid, '5145');
		array_push($arMnuid, '5555');
		}*/
	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

	$arVigencia = Array(EM_ANALISE_PROFE,
	EM_SOLICITACAO_REFORMULACAO,
	EM_REFORMULACAO_PROCESSO,
	EM_ANALISE_TECNICA_REFORMULACAO,
	EM_EMPENHO_REFORMULACAO,
	EM_GERACAO_TERMO_ADITIVO,
	EM_ANALISE_PROCESSO_REF,
	EM_IDENTIFICACAO_PROCESSO_REFORMULACAO,
	EM_TERMO_ADITIVO_PUBLICADO,
	EM_PUBLICACAO_REFORMULACAO,
	EM_LIBERACAO_RECURSO_REFORMULACAO,
	EM_ASSINATURA_REFORMULACAO,
	EM_ACEITACAO_REFORMULACAO,
	EM_PROCESSO_REFORMULADO,
	EM_ANALISE_JURIDICA_REFORMULACAO,
	EM_CORREÇÃO_DA_REFORMULAÇÃO_DO_PROCESSO);
	
	$arTipoRef = array();
	if( $_SESSION["emenda"]["ptrid"] ) $arTipoRef = verificaTiposReformulacao( $_SESSION["emenda"]["ptrid"], 'codigo' );
	
	if( $arTipoRef && $estadoAtual != EM_SOLICITACAO_REFORMULACAO) {
		$arMnuid = $db->carregarColuna("select mnuid from emenda.abasreformulacao where abacod = $abacod_tela and trefid in (".implode(',', $arTipoRef).")");
	}
	
	if( !in_array($estadoAtual, $arVigencia ) || (in_array(APOSTILAMENTO, $arTipoRef) || in_array(RENDIMENTO_DE_APLICACAO, $arTipoRef) || in_array(ALTERACAO_DE_ITENS_DA_ESPECIFICACAO, $arTipoRef)) ){
		array_push($arMnuid, '6836');//esconde aba vigência da reformulação
	}
	/*&& ( in_array( ADMINISTRADOR_INST, $arPerfil ) || in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil ))*/
	if(  !in_array( $estadoAtual, $arEstadoTrava ) ){
		array_push($arMnuid, '6838');//esconde aba reformulação
	}
	
	$ptrid = ($_SESSION["emenda"]["ptrid"]) ? $_SESSION["emenda"]["ptrid"] : $_REQUEST["ptrid"];
	$ptrid = !$ptrid ? $_SESSION["emenda"]["ptridAnalise"] : $ptrid;
	
	if( !empty($ptrid) ){
		$sql = "SELECT count(anaid) as total FROM emenda.analise WHERE ptrid = $ptrid and anatipo = 'T' and anasituacaoparecer = 'E'";
		$anaid = $db->pegaUm( $sql );
	}
	
	/*if( in_array(RENDIMENTO_DE_APLICACAO, $arTipoRef) ){
		array_push($arMnuid, '5118'); #Dados Adicionais
		array_push($arMnuid, '5196'); #Anexos
		array_push($arMnuid, '5144'); #Cronograma de Execução e Desembolso
		array_push($arMnuid, '5555'); #Habilitação
		array_push($arMnuid, '5791'); #Histórico e Acompanhamento
		array_push($arMnuid, '6209'); #Documentos
		array_push($arMnuid, '6836'); #Vigência Convênio
		array_push($arMnuid, '6838'); #Reformulação
		array_push($arMnuid, '7590'); #Dados do Convênio
		array_push($arMnuid, '8315'); #Diligência
	}*/

	if( $_SESSION["emenda"]["ptrid"] ) $emendaFederal = verificaEmendaFederal($_SESSION["emenda"]["ptrid"]);
		
	if( $sisid == '23' || $emendaFederal || $sisid == 2 ){
		
		if( $emendaFederal ){
			$arMnuid = array('5119', '5118', '5196', '5555', '5791', '6209', '7590', '5437', '6836', '6838', '5144');
		} else {
			$arMnuid = array('5119', '5118', '5196', '5555', '5791', '6209', '7590', '5437', '6836', '6838', '5144', '14297');
		}
		$boImpositivo = verificaOrcamentoImpositivo( $ptrid );
		if( $boImpositivo < 1 ){
			array_push($arMnuid, '14290');//esconde aba Orçamento Impositivo
		}
	} else {
		array_push($arMnuid, '14290');//esconde aba Orçamento Impositivo
		array_push($arMnuid, '14297');//esconde aba Dados Gerais do PTA
	}
	
	if( !$emendaFederal ){
		array_push($arMnuid, '14379'); //esconde aba Dadso Orçamentarios
	}
	
	if( $sisid != 23 ){
		array_push($arMnuid, '14208');//esconde aba Dados Orçamentários PTA
	}
	
	if( $anaid == 0 ) array_push($arMnuid, '8315');//esconde aba Diligencia
	
	return $db->cria_aba( $abacod_tela, $url, $parametros, $arMnuid );
}

function montaAbasAnalise( $abacod_tela, $url, $parametros = '', $estadoAtual = '', $boReformulacao = false ){
	global $db;

	if( empty($estadoAtual) ) $estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

	$arMnuid = array();

	if( !$boReformulacao && $estadoAtual != EM_SOLICITACAO_REFORMULACAO && $estadoAtual != EM_ACEITACAO_REFORMULACAO ){
		$arMnuid = array('6254');
	} else {
		if( $boReformulacao ){
			$arMnuid = array('5501', '5499');
		}
	}

	if( $estadoAtual == EM_ANALISE_TECNICA_REFORMULACAO || $estadoAtual == EM_IDENTIFICACAO_PROCESSO_REFORMULACAO ){
		array_push( $arMnuid, '5797'); #esconde aba assinatura na alaise
	}
	$sql = "SELECT resid FROM emenda.planotrabalho WHERE ptrid = ".$_SESSION["emenda"]["ptridAnalise"];
	$resid = $db->pegaUm( $sql );
	
	$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = ".$resid);
	if( $reenalisemerito == 'N' ){
		array_push($arMnuid, '5501'); #esconde a aba ANALISE DE MERITO 
		//array_push($arMnuid, '5499'); #esconde a aba VINCULAÇÃO DA UNIDADE GESTORA
	}
	
	$arRrefid = verificaTiposReformulacao( $_SESSION["emenda"]["ptridAnalise"], 'codigo' );	
	if( $arRrefid && $estadoAtual != EM_SOLICITACAO_REFORMULACAO) {
		$arMnuid = $db->carregarColuna("select mnuid from emenda.abasreformulacao where abacod = $abacod_tela and trefid in (".implode(',', $arRrefid).")");
	}
	
	if( $_SESSION['exercicio'] < '2013'){
		array_push($arMnuid, '14279'); #esconde a aba SICONV
	}
	$boImpositivo = verificaOrcamentoImpositivo( $_SESSION["emenda"]["ptridAnalise"] );
	if( $boImpositivo < 1 ){
		array_push($arMnuid, '14286'); #esconde aba Orçamento Impositivo
	}
	
	return $db->cria_aba( $abacod_tela, $url, $parametros, $arMnuid );
}


function montaListaArquivosAnexados( $ptrid, $refid, $estadoAtual ){
	global $db;
	$sql = "select anx.anxid,
		   anx.arqid,
		   anx.ptrid,
		   anx.anxdsc,
		   arq.arqnome || '.' || arq.arqextensao as arquivo,
		   anx.anxtermoref
		from emenda.anexo anx
			inner join public.arquivo arq on anx.arqid = arq.arqid
 		where ptrid = {$ptrid} and anx.anxtipo = 'R' and refid = $refid";

	$arDados = $db->carregar($sql);

	if( $arDados ){
		?>
	<tr>
		<td class="SubTituloEsquerda" colspan="3">Documentos Anexados</td>
	</tr>
	<?
	$count = 1;
	foreach($arDados as $dados){?>
	<tr>
		<td align="left"><?php echo $count.' - '; ?><a
			style="cursor: pointer; color: blue;"
			onclick="window.location='?modulo=principal/reformulacao&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>'"><?php echo $dados['arquivo'];?></a>
		</td>
		<td align="left"><?php echo $dados['anxdsc']; ?></td>
		<td align="center"><? if(possuiPermissao( $estadoAtual ) ) {?> <img
			src="../imagens/alterar.gif"
			onClick="window.location.href='emenda.php?modulo=principal/reformulacao&acao=A&arqid=<?php echo $dados['arqid'];?>'"
			style="border: 0; cursor: pointer;" title="Alterar Descrição Anexo">
		<img src="../imagens/excluir.gif"
			onClick="excluirAnexo('<?php echo $dados['arqid']; ?>');"
			style="border: 0; cursor: pointer;" title="Excluir Documento Anexo">
			<? } else { ?> <img src="../imagens/alterar_01.gif"
			style="border: 0; cursor: pointer;" title="Alterar Descrição Anexo">
		<img src="../imagens/excluir_01.gif"
			style="border: 0; cursor: pointer;" title="Excluir Documento Anexo">
			<? } ?></td>
	</tr>
	<?php
	$count++;
}
}
}



#não exlcuir essa função
function buscaEmendaDescentraliza( $ptrid, $enbid = '', $emecod = '', $resid = '' ){
	global $db;		
	$sql = "SELECT
			  	e.emedescentraliza
			FROM
			  	emenda.emenda e
			  	INNER JOIN emenda.emendadetalhe ed ON (e.emeid = ed.emeid) 
			  	INNER JOIN emenda.emendadetalheentidade ede ON (ed.emdid = ede.emdid)
			  	LEFT JOIN (SELECT ped.pedid, ped.edeid, ped.ptrid
			               FROM emenda.ptemendadetalheentidade ped
			    				inner join emenda.planotrabalho ptr
			                    	on (ptr.ptrid = ped.ptrid)
			               WHERE ptr.ptrstatus = 'A') ptede
			  		ON (ede.edeid = ptede.edeid)  
			WHERE
				(".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
			  	AND ede.edestatus = 'A'
			  	".( $resid ? 'AND e.resid = '.$resid : '')."
			  	AND ede.enbid = ".(($ptrid) ? '(SELECT enbid FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : $enbid)."
			  	AND ede.ededisponivelpta = 'S'
			  	AND e.emeano >= ".$_SESSION['exercicio'] ;

	$federal = $db->pegaUm( $sql );
	$_SESSION['emenda']['federal'] = $federal ;
	
	return $federal;
}

function verificaParecerAnalise( $ptrid, $anatipo ){
	global $db;
	$sql = "SELECT 
				anasituacaoparecer 
			FROM 
				emenda.analise 
			WHERE 
				ptrid = $ptrid
			    and anatipo = '$anatipo'  
			    and anastatus = 'A'  
				and analote = (SELECT max(analote) 
			    				FROM emenda.analise 
			                    WHERE ptrid = $ptrid and anastatus = 'A' and anatipo = '$anatipo')";
		
	$arSituacao = $db->carregarColuna($sql);
	$boSituacao = false;	
	if( in_array('F', $arSituacao) ){
		$boSituacao = true;
	} else {
		$arErro = array(
					"tipo" => "Pagamento",
					"msg"  => "Todos os pareceres técnicos deverão está concluídos e favoráveis"
					);
		$_SESSION['emenda']['msgErro'][]=  $arErro;
	}
}#fim das funções que vem antes da solicitação de reformulação

/*function montaSQLWorkFlow(){
	global $db;
	
	$sql = "";
	$esdiddestino = array(56, 69,70,120,137,200,206,207);
    
    foreach ($esdiddestino as $esdid) {
	    
	    switch ($esdid) {
	    	case 56:
	    		$aedcondicao = "validaEnvioAnaliseTecnicaSolicitacao(ptrid);";
	    	break;
	    	case 69:
	    		$aedcondicao = "validaEnvioPreConvenioSolicitacao(ptrid);";
	    	break;
	    	case 70:
	    		$aedcondicao = "validaEnvioLiberacaoRecursoSolicitacao(ptrid);";
	    	break;
	    	case 120:
	    		$aedcondicao = "validaEnvioRecursoLiberadoSolicitacao(ptrid);";
	    	break;
	    	case 137:
	    		$aedcondicao = "validaEnvioGeraTermoAdtivoSolicitacao(ptrid);";
	    	break;
	    	case 200:
	    		$aedcondicao = "validaEnvioTermoAdtivoPublicadoSolicitacao(ptrid);";
	    	break;
	    	case 206:
	    		$aedcondicao = "validaEnvioAprovacaoReformulacaoSolicitacao(ptrid);";
	    	break;
	    	case 207:
	    		$aedcondicao = "validaEnvioProcessoReformuladoSolicitacao(ptrid);";
	    	break;
	    }
	    
	    $sql = "SELECT count(aedid) FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdidorigem = 207 and esdiddestino = $esdid";
	    $boAcao = $db->pegaUm($sql);	    
	    //if( $boAcao == 0 ) {
	    	$sql = "SELECT aeddscrealizar, aeddscrealizada FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdiddestino = $esdid limit 1";
	    	$arAcoes = $db->pegaLinha( $sql );
	    	$aeddscrealizar  = str_replace( array('Encaminhar', 'encaminhar'), 'Retornar', $arAcoes['aeddscrealizar'] );
	    	$aeddscrealizada = str_replace( array('Encaminhado', 'encaminhado'), 'Retornado', $arAcoes['aeddscrealizada'] );
	    
		    $sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada, 
		  					aedcondicao, aedobs, aedposacao, aedvisivel, aedicone, aedcodicaonegativa) 
					VALUES (207, $esdid, '{$aeddscrealizar}', '{$aeddscrealizada}',
		  					'$aedcondicao', '', '', true, '', false) RETURNING aedid";
			echo( $sql."<br>" );    
		    //$aedid = $db->pegaUm( $sql );
	   // }
	    $sql = "SELECT a.aedid, a.aeddscrealizar, ed.esdid, ed.esddsc,
					a.aedobs, a.aedicone, a.aedcodicaonegativa
				FROM workflow.acaoestadodoc a
					inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
				WHERE
					esdidorigem = 121 and
					aedstatus = 'A' and ed.esdid = $esdid";
	    $arEstados = $db->pegaLinha( $sql );
	    
	    $sql_excluirPerfil = "DELETE FROM workflow.estadodocumentoperfil WHERE aedid=".$arEstados['aedid'];
		//$db->executar($sql_excluirPerfil);
		
	    $sql_perfil = '';
	    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".SUPER_USUARIO." , {$arEstados['aedid']} ); ";
	    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".ADMINISTRADOR_INST." , {$arEstados['aedid']}); ";
	    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".ADMINISTRADOR_REFORMULACAO." , {$arEstados['aedid']}); ";
	    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".ANALISTA_TECNICO." , {$arEstados['aedid']}); ";
	    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".ANALISTA_FNDE." , {$arEstados['aedid']}); ";
		echo( $sql_perfil."<br>" );
	    //$db->executar($sql_perfil);
    }
    die();
	return $db->commit();
}*/

function mostraEstadoAnterior($ptrid){
	global $db;
	
	$docid =  $db->pegaUm("SELECT docid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
	
	$sql = "SELECT ed.esdid, ed.esddsc, ac.aeddscrealizada, * 
		    FROM workflow.historicodocumento hd
		    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
		    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
		    WHERE hd.docid = {$docid} 
		    order by hd.htddata asc";
    
    $historicoDocumento = $db->carregar($sql);
    
    $retorno = '';
    foreach($historicoDocumento as $chave => $documento){
    	$documentos[] = $documento['esdid'];
        if($documento['esdid'] == EM_SOLICITACAO_REFORMULACAO){
        	array_pop($documentos);
            $retorno = end($documentos);
            break; 
        }
    }
    return $retorno;
}

function validaContraPartidaPTA( $enbid ){
	global $db;
	
	$sql = "SELECT enbcertificado, 
				enbcertificadovalido, 
				to_char(enbvaldataini, 'YYYY-MM-DD') as enbvaldataini, 
				to_char(enbvaldatafim, 'YYYY-MM-DD') as enbvaldatafim, 
				to_char(enbdatacertificado, 'YYYY-MM-DD') as enbdatacertificado, 
				to_char(now(), 'YYYY-MM-DD') as dataatual 
			FROM emenda.entidadebeneficiada WHERE enbid = $enbid";
	$arRegistro = $db->pegaLinha( $sql );
	$arRegistro = $arRegistro ? $arRegistro : array();
	extract($arRegistro);
	$bocontrapartida = 'true';
	if( $enbcertificado == 'S' && $enbcertificadovalido == 'S' ){
		if( $enbvaldatafim < $dataatual ) $bocontrapartida = 'true'; 
		else $bocontrapartida = 'false';
	} else if( $enbcertificado == 'S' && $enbcertificadovalido == 'N' ){
		if( $enbvaldataini <= $enbdatacertificado && $enbvaldatafim >= $enbdatacertificado ) $bocontrapartida = 'false';
		else $bocontrapartida = 'true';
	}
	
	return $bocontrapartida;
}

function carregaAbasGuia($pagina) {
	global $db;
	switch($pagina) {

		case 'cadastrarGuiaIniciativa':
			if(!empty($_SESSION["emenda"]["ginid"])){
				$menu = array(
				//0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/guiaListaIniciativa&acao=A"),
				0 => array("id" => 1, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/cadastrarGuiaIniciativa&acao=A"),
				1 => array("id" => 2, "descricao" => "Especificações", "link" => "/emenda/emenda.php?modulo=principal/cadastrarGuiaEspecificacao&acao=A")
				);
			}else {
				$menu = array(
				//0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/guiaListaIniciativa&acao=A"),
				0 => array("id" => 1, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/cadastrarGuiaIniciativa&acao=A")
				);
			}
			break;
				
		case 'cadastrarGuiaEspecificacao':
			if(!empty($_SESSION["emenda"]["ginid"])){
					$menu = array(
					//0 => array("id" => 1, "descricao" => "Lista de Iniciativas", "link" => "/emenda/emenda.php?modulo=principal/guiaListaIniciativa&acao=A"),
					0 => array("id" => 1, "descricao" => "Iniciativa", "link" => "/emenda/emenda.php?modulo=principal/cadastrarGuiaIniciativa&acao=A"),
					1 => array("id" => 2, "descricao" => "Especificações", "link" => "/emenda/emenda.php?modulo=principal/cadastrarGuiaEspecificacao&acao=A")
					);
			}
			break;
	}

	$menu = $menu ? $menu : array();
	 
	return $menu;
}

function cabecalhoGuia($passo){
	global $db;
	
	$arrGuia = $db->pegaLinha("SELECT guinome, ini.ininome, tpe.tpedsc FROM emenda.guia g
									left join emenda.guia_guiainiciativa ggi on ggi.guiid = g.guiid and ggi.ginid = ".($_SESSION['emenda']['ginid'] ? $_SESSION['emenda']['ginid'] : '0')."
									left join emenda.guiainiciativa gi on gi.ginid = ggi.ginid
								    left join emenda.iniciativa ini on ini.iniid = gi.iniid
								    left join emenda.tipoensino tpe on tpe.tpeid = gi.tpeid
								WHERE g.guiid = ".$_SESSION['emenda']['guiid']);
	/*ver("SELECT guinome, ini.ininome, tpe.tpedsc FROM emenda.guia g
									left join emenda.guia_guiainiciativa ggi on ggi.guiid = g.guiid
									left join emenda.guiainiciativa gi on gi.ginid = ggi.ginid and gi.ginid = ".($_SESSION['emenda']['ginid'] ? $_SESSION['emenda']['ginid'] : '0')."
								    left join emenda.iniciativa ini on ini.iniid = gi.iniid
								    left join emenda.tipoensino tpe on tpe.tpeid = gi.tpeid
								WHERE g.guiid = ".$_SESSION['emenda']['guiid']);*/
	$html = "<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
				 <tbody>
				 	<tr>
				 		<td colspan=\"4\" class=\"subtitulocentro\">Dados da Guia</td>
				 	</tr>
					<tr>
						<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">Passo:</td>
						<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$passo}</td>
					</tr><tr>
						<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">Guia:</td>
						<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$arrGuia['guinome']}</td>
					</tr>
					<tr>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Iniciativa:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$arrGuia['ininome']}</td>
					</tr><tr>
						<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Tipo de Ensino:</td>
						<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$arrGuia['tpedsc']}</td>
					</tr>
				 </tbody>
			</table>";
	echo $html;
}

function montaGuiaPlanoTrabalho($resid, $enbid){
	global $db;
	
	$sql = "SELECT DISTINCT
				/*ini.iniid,
   				g.guinome,*/
   				ini.ininome
			FROM
				emenda.iniciativadetalheentidade ide
			    inner join emenda.emendadetalheentidade ede on ede.edeid = ide.edeid
			    inner join emenda.iniciativa ini on ini.iniid = ide.iniid
			    inner join emenda.iniciativaresponsavel inr on inr.iniid = ini.iniid
			    inner join emenda.guiainiciativa gi on gi.iniid = ini.iniid
			    inner join emenda.guia_guiainiciativa ggi on ggi.ginid = gi.ginid
			    inner join emenda.guia g on g.guiid = ggi.guiid
			WHERE
				ede.enbid = $enbid
			    and inr.resid = $resid
			    and gi.ginistatus = 'A'
			    and ide.idestatus = 'A'
    			and g.guistatus = 'A'";
	
	$etoid = $db->pegaUm("select e.etoid from emenda.v_emendadetalheentidade v 
								inner join emenda.emenda e on e.emeid = v.emeid
							where v.entid = $enbid");
    
	$etoid = ( !empty($etoid) ? $etoid : '3');
    
    if( $resid && $enbid && ($etoid == 3) )
	$arrGuia = $db->carregarColuna( $sql, "ininome" );

	if( $arrGuia ){
		
		$cabecalho = "<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">	
					 <tbody>
					 	<tr>
					 		<td colspan=\"2\" style=\"text-align: left;\"><b>Proposta de Preenchimento do Plano de Trabalho</b></td>
					 	</tr>
					 	<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Iniciativa:</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".implode('<br>', $arrGuia)."</td>
						</tr>
						<tr style=\"display: none\" id=\"tr_guia\">
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Guia(s):</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\"><div id=\"td_guia\"></div></td>
						</tr>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">&nbsp;</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\"><a href=\"#\" onclick=\"visualizaProposta($resid, $enbid);\">Visualizar Proposta</a></td>
						</tr>";
		$cabecalho .= "</tbody></table>";
	}
		echo $cabecalho;
}

function cadastraGuiaPTA($ptrid, $post){
	global $db;
	//$_SESSION['emenda']['cadGuia']
	$guiids = $post['guia_check'];
	
	$sql = "SELECT DISTINCT
   				g.guinome,
                g.guijustificativa,
   				g.guiid,
                gi.ginidetalhamento,
                gi.tpeid,
                gi.ginid,
                gi.iniid
			FROM
            	emenda.guia g
                inner join emenda.guia_guiainiciativa ggi on ggi.guiid = g.guiid
                inner join emenda.guiainiciativa gi on gi.ginid = ggi.ginid
                left join emenda.guia_guiadocumento gd on gd.ginid = gi.ginid
                left join emenda.guiadocumento gid on gid.gdoid = gd.gdoid
			WHERE
				g.guiid in ($guiids)
			    and gi.ginistatus = 'A'
    			and g.guistatus = 'A'";
	
	$arrGuias = $db->carregar($sql);
	$arrGuias = $arrGuias ? $arrGuias : array();
	
	$strJustificativa = '';
	foreach ($arrGuias as $key => $guias) {
		extract($guias);
		$strJustificativa .= $guijustificativa.' ';
		$sql = "INSERT INTO emenda.guia_planotrabalho(ptrid, guiid) 
				VALUES ($ptrid, $guiid)";
		$db->executar($sql);
		
		$sql = "INSERT INTO emenda.ptiniciativa(ptrid, ptidescricao, iniid, tpeid) 
				VALUES ($ptrid, '$ginidetalhamento', $iniid, $tpeid) returning ptiid";
		$ptiid = $db->pegaUm($sql);
		
		$sql = "SELECT DISTINCT
					ge.espid
				FROM
					emenda.guiainiciativa gi
				    inner join emenda.guia_guiainiciativa ggi on ggi.ginid = gi.ginid
					inner join emenda.guiaespecificacao ge on ge.ginid = gi.ginid
				WHERE
					ge.ginid = $ginid
				    and ggi.guiid = $guiid
				    and gi.ginistatus = 'A' order by ge.espid";
		
		$arrEsp = $db->carregarColuna($sql);
		$arrEsp = $arrEsp ? $arrEsp : array();
		
		foreach ($arrEsp as $espid) {
			$arrPedids = $db->carregarColuna("SELECT pedid FROM emenda.ptemendadetalheentidade WHERE ptrid = $ptrid");
			
			$iceid = $db->pegaUm("SELECT iceid FROM emenda.iniciativaespecificacao WHERE iniid = $iniid and espid = $espid and icestatus = 'A'");
			if( $iceid ){
				$sql = "INSERT INTO emenda.ptiniciativaespecificacao(ptiid, iceid, ptequantidade, ptevalorunitario, ptevalorproponente, ptedatainicio, ptedatafim) 
						VALUES ($ptiid, $iceid, 0, 0.00, null, null, null) returning pteid";
				$pteid = $db->pegaUm($sql);
				
				/*$sql = "SELECT DISTINCT
							ge.iteid,
							ie.ipaid
						FROM
			            	emenda.guiaespecificacao ge
			                inner join emenda.itempar_especificacao ie on ie.iteid = ge.iteid
						WHERE
							ge.espid = $espid
			                and ie.espid = $espid
			                and ge.ginid = $ginid";*/
			                
			    $sql = "SELECT DISTINCT
						    ie.iteid,
						    ie.ipaid,
						    d.dicvalor, 
						    umdi.umidescricao,
						    p.picdescricao
						FROM
						    emenda.guiaespecificacao ge
						    inner join emenda.itempar_especificacao ie on ie.iteid = ge.iteid and ie.espid = ge.espid
						    inner join emenda.itempar ip on ip.ipaid = ie.ipaid    
						    inner join par.propostaitemcomposicao p ON ip.picid = p.picid
						    inner join par.detalheitemcomposicao d ON d.picid = p.picid
							inner join par.ufdetalheitemcomposicao ufd ON ufd.dicid = d.dicid
							inner join par.unidademedidadetalhamentoitem umdi on umdi.umiid = p.umiid and umdi.umistatus = 'A'
						WHERE
						    ge.espid = $espid
						    and ie.espid = $espid
						    and ge.ginid = $ginid
						    and d.dicstatus = 'A' 
						    and (cast(now() as date) between d.dicdatainicial and d.dicdatafinal) 
							and ufd.estuf in (SELECT DISTINCT enb.estuf
													FROM 
														emenda.planotrabalho ptr 
													    inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
													WHERE 
														ptr.ptrid = $ptrid)";
			    
			    $arrItem = $db->carregar($sql);
				$arrItem = $arrItem ? $arrItem : array();
				
				$valorUnitario = 0;
				foreach ($arrItem as $itens) {
					$sql = "INSERT INTO emenda.ptitemkit(pteid, iteid, ptkunidademedida, ptkquantidade, ptkvalorunitario) 
							VALUES ($pteid, {$itens['iteid']}, '{$itens['umidescricao']}', 0, '{$itens['dicvalor']}')";
					$db->executar($sql);
					$valorUnitario = (float)$valorUnitario + (float)$itens['dicvalor'];
				}
				$sql = "UPDATE emenda.ptiniciativaespecificacao SET ptevalorunitario = $valorUnitario WHERE pteid = $pteid";
				$db->executar( $sql );
				
				foreach ($arrPedids as $pedid) {
					$sql = "INSERT INTO emenda.ptiniesprecurso( pteid, pedid, pervalor) 
							VALUES ( $pteid, $pedid, 0.00)";
					$db->executar($sql);	
				}
			}
		}
	}
	$sql = "UPDATE emenda.planotrabalho SET ptrjustificativa = '$strJustificativa', ptrguia = true WHERE ptrid = $ptrid";
	$db->executar($sql);
	
	echo $db->commit();
}

function verificaFluxo( $ptrid ){
	global $db;
	
	$estadoAtual = pegarEstadoAtual( $ptrid );
	$arFluxo = $db->carregarColuna("select aedid from emenda.fluxoreformulacao where esdid = $estadoAtual");
	
	return $arFluxo;
}

function verificaOrcamentoImpositivo( $ptrid ){
	global $db;
	
	$sql = "select
				count(v.emdid)
			from 
				emenda.ptemendadetalheentidade pt
			    inner join emenda.v_emendadetalheentidade v on v.edeid = pt.edeid
			where
				pt.ptrid = $ptrid
			    and v.emdimpositiva = '6'";
	$total = $db->pegaUm($sql);
	return $total;
}

function verificaDataImpositivo( $dataFimLibera ){
	global $db;
	
	require_once APPRAIZ . 'includes/classes/dateTime.inc';
	
	$data = new Data();
	$dataAtual = date('Y-m-d');
	$arData = explode('-', $dataAtual);
	if( (int)$arData[2] == 1 ){
		$dataAtual = $arData[0].'-'.((int)$arData[1] - 1).'-30';
	} else {
		$dataAtual = ($arData[0]).'-'.$arData[1].'-'.((int)$arData[2] - 1);
	}
	
	$boLiberarEmenda = $data->diferencaEntreDatas(  $dataFimLibera, $dataAtual, 'maiorDataBolean', '', 'YYYY-MM-DD');
	
	return $boLiberarEmenda;
}

function verificaEmendaFederal( $ptrid ){
	global $db;
	
	$sql = " select 
			 	count(ed.emdid) 
			 from 
			 	emenda.ptemendadetalheentidade pt
			    inner join emenda.emendadetalheentidade ede on ede.edeid = pt.edeid
			    inner join emenda.emendadetalhe ed on ed.emdid = ede.emdid
			    inner join emenda.emenda e on e.emeid = ed.emeid
			 where
			 	pt.ptrid = $ptrid
			 	and e.etoid = 4
			    and ede.edestatus = 'A'";
	$mod = $db->pegaUm($sql);
	
	if( $mod > 0 && $_SESSION['exercicio'] >= '2013' ){
		return true;
	} else {
		return false;
	}
}

function verificaPrazoPreenchimentoEmenda( $enbid, $ptrid = '' ){
	global $db;
	
	$arrIndicacao = $db->carregar("select count(v.edeid) as total, v.emecod from emenda.v_emendadetalheentidade v 
									where v.entid = $enbid and to_char(v.ededataindicacao, 'YYYY-MM-DD') > '".DATA_INDICACAO_EMENDA."' 
										and emeano = '{$_SESSION['exercicio']}' 
										and emdimpositiva = '6'
									group by v.emecod");
	$arrIndicacao = $arrIndicacao ? $arrIndicacao : array();
	
	$arEmenda = array();
	$arrMsg = array();
	$strIndicacao = '';
	if( $arrIndicacao[0] ){
		foreach ($arrIndicacao as $v) {
			if( $v['total'] > 0 ){
				array_push( $arEmenda, $v['emecod'] );
			}
		}
		
		if( $arEmenda ){
			array_push($arrMsg, "A(s) emenda(s) ".implode(', ', $arEmenda)." foi(ram) indicada(s) com prazo expirado.");
		}
	}
	
	if( $ptrid ){
		$boPta = $db->pegaUm("select count(ptrid) from emenda.planotrabalho p where p.ptrid = $ptrid and to_char(p.ptrdatainclusao, 'YYYY-MM-DD') > '".DATA_ELABORACAO_PTA."'");
		if( $boPta > 0 ){
			array_push($arrMsg, "O PTA está sendo ou foi Elaborado com prazo de vigência expirado.");
		}
		
		$sql = "select count(ptrid) from(
				    select max(a.anadatainclusao) as data, p.ptrid  
					from emenda.planotrabalho p
						inner join emenda.analise a on a.ptrid = p.ptrid and a.anastatus = 'A'
				    where
				        p.ptrid = $ptrid
				        and a.anatipo in ('D', 'M', 'T')
				    group by
				        p.ptrid
				    ) as foo
				where
					to_char(data, 'YYYY-MM-DD') > '".DATA_ANALISE_PTA."'";
		$boAnalise = $db->pegaUm($sql);
		
		if( $boAnalise > 0 ){
			array_push($arrMsg, "O PTA está sendo ou foi Analisado com prazo de vigência expirado.");
		}
		
		$boDiligencia = $db->pegaUm("select count(ptrid) from emenda.planotrabalho where ptrid = $ptrid and to_char(ptrdatadiligencia, 'YYYY-MM-DD') > '".DATA_DILIGENCIA_PTA."'");
		if( $boDiligencia > 0 ){
			array_push($arrMsg, "O PTA está sendo ou foi Diligênciado com prazo de vigência expirado.");
		}
	}
	
	if( $arrMsg[0] ){
		$strIndicacao = "<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Prazo de Indicação:</td>
							<td width=\"80%\" colspan=3 style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; color: red; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".implode('<br>', $arrMsg)."</td>
						</tr>";
	}
	
	return $strIndicacao;
}

function carregaAbasEmendaObras($stPaginaAtual = null, $param = Array() ){
	global $db;
	
	$ptrid = $param['ptrid'];
	$pteid = $param['pteid'];
	$preid = $param['preid'];
	$iceid = $param['iceid'];
	
	$lnkDados = "emenda.php?modulo=principal/emendaObras&acao=A&pteid=$pteid&iceid=$iceid&ptrid=$ptrid".($preid ? "&preid=$preid" : "");
	$abas = array(0 => array("descricao" => "Dados do terreno", "link" => $lnkDados));
	
	return montarAbasArray($abas, $stPaginaAtual, $win);
}

function salvarEmendaObras( $post ){
	global $db;

	$preid = $post['preid'];
	$predescricao = $post['predescricao'];
	$presistema = $_SESSION['sisid'];
	$preidsistema = $_SESSION['sisid'];
	$ptoid =  $post['ptoid'];//$post['ptoid_disable'];
	$estuf = $post['estuf_disable'] ? $post['estuf_disable'] : $post['estuf'];
	$muncod = $post['muncod_'];
	$estufpar = $_SESSION['par']['estuf'];
	/*if( $_SESSION['par']['itrid'] == 2 ){
		$muncodpar = $_SESSION['par']['muncod'];
		$preesfera = 'M';
	} else {
		$preesfera = 'E';
	}*/
	
	$preesfera = 'null';
	
	$prelogradouro = $post['endlog'];
	$precomplemento = $post['endcom'];
	$precep = str_replace(array("-","."),"",$post['endcep1']);
	$prenumero = $post['endnum'];
	$prebairro = $post['endbai'];
	$prelatitude = $post['latitude'] ? implode(".",$post['latitude']) : "null";
	$prelongitude = $post['longitude'] ? implode(".",$post['longitude']) : "null";
	$predtinclusao = "'now()";
	$preano = $_SESSION['exercicio'];
	$tooid = 4;
	
	if($preid){
		$sql = "UPDATE obras.preobra SET 
					presistema = '$presistema',
					preidsistema = '$preidsistema',
					ptoid = '$ptoid',
					prelogradouro = '$prelogradouro',
					precomplemento = '$precomplemento',
					estuf = '$estuf',
					muncod = '$muncod',
					precep = '$precep',
					prelatitude = '$prelatitude',
					prelongitude = '$prelongitude',
					prebairro = '$prebairro',
					preano = '$preano',
					predescricao = '$predescricao',
					prenumero = '$prenumero',
					tooid = '$tooid',
					preesfera = $preesfera
				WHERE
					preid = $preid";
		$db->executar($sql);
	}else{
		$sql = "INSERT INTO obras.preobra(presistema, preidsistema, ptoid, prelogradouro, precomplemento, estuf, muncod, precep, prelatitude, prelongitude, predtinclusao,
  					prebairro, preano, predescricao, prenumero, prestatus, tooid, preesfera) 
				VALUES ('$presistema', '$preidsistema', '$ptoid', '$prelogradouro', '$precomplemento', '$estuf', '$muncod', '$precep', '$prelatitude', '$prelongitude', now(),
  					'$prebairro', '$preano', '$predescricao', '$prenumero', 'A', '$tooid', $preesfera) returning preid";
		/*if( $post['frmid_libera'] == 15 ){
			$sql = "SELECT docid FROM par.subacao WHERE sbaid = ".$_REQUEST['sbaid'];
			$documentoSubacao = $db->pegaUm($sql);
			$estadoAtualSubacao = wf_pegarEstadoAtual( $documentoSubacao );
			if( $estadoAtualSubacao == WF_SUBACAO_DILIGENCIA_CONDICIONAL ){ // Aceitou condicional
				$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
						VALUES (".WF_FLUXO_OBRAS_PAR.", ".WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL.", 'Em cadastramento Condicional', now()) returning docid ";
			} else { // Aceitou normal
				$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
						VALUES (".WF_FLUXO_OBRAS_PAR.", ".WF_PAR_EM_CADASTRAMENTO.", 'Em Cadastramento', now()) returning docid ";
			}
			$docid = $db->pegaUm($sql);
			$sql = "UPDATE obras.preobra SET docid = {$docid} WHERE preid = {$preid}";
			$db->executar( $sql );
			$db->commit();
		} else {
			preCriarDocumento($preid, WF_FLUXO_OBRAS_PAR);
		}*/
		$preid = $db->pegaUm($sql);
	}
	$db->commit();
	
	return $preid;
}

function logWsSIGEF( $arrParam = array() ){
	global $db;
	
	$ptrid = $arrParam['ptrid'];
	$xmlEnvio = $arrParam['xmlEnvio'];
	$xmlRetorno = $arrParam['xmlRetorno'];
	$logTipo = $arrParam['logTipo'];
	$exfid = ($arrParam['exfid'] ? $arrParam['exfid'] : 'null');
	$cocid = ($arrParam['cocid'] ? $arrParam['cocid'] : 'null');
	$lwsid = ($arrParam['lwsid'] ? $arrParam['lwsid'] : 'null');
	
	$xmlEnvio = str_replace( "'", '"', $xmlEnvio);
	$xmlRetorno = str_replace( "'", '"', $xmlRetorno);
	//$xmlEnvio = utf8_encode($xmlEnvio);
	//$xmlRetorno = utf8_decode($xmlRetorno);
	
	$sql = "INSERT INTO emenda.logerrows(ptrid, exfid, cocid, lwsid, logtipo, usucpf, logdatainclusao, logenvio, logresposta) 
			VALUES (
			  $ptrid,
			  $exfid,
			  $cocid,
			  $lwsid,
			  '{$logTipo}',
			  '{$_SESSION['usucpf']}',
			  '".date('Y-m-d H:i:s')."',
			  '".($xmlEnvio)."',
			  '".($xmlRetorno)."'
			)";
	
	$logid = $db->pegaUm($sql);
	$db->commit();
	
	return $logid;
}

function logWsRequisicao( $arrParam = array(), $chave = '', $tabela = '', $action = 'insert' ) {
	global $db;

	$codigo = '';
	if( $action == 'insert' ){
		$arCampos = array();
		$arValor = array();
		foreach ($arrParam as $campo => $valor){
			if( $valor !== null ){
				$arCampos[]  = $campo;
				$valor = str_replace($troca, "", $valor);
				$arValor[] = trim( pg_escape_string( $valor ) );
			}
		}
		$sql = "insert into ".$tabela." (".implode(', ', $arCampos) ." ) values( '". implode("', '", $arValor )."') returning ".$chave."";
		$codigo = $db->pegaUm($sql);
	} else {
		$campos = "";
		foreach ($arrParam as $campo => $valor){
			if( !empty($valor) || is_bool($valor) ){

				if( $campo == $chave ){
					$codigo = $valor;
				}elseif( is_bool($valor) ){
					$campos .= $campo." = ".($valor ? 'true' : 'false').", ";
				}else {
					$valor = pg_escape_string( $valor );
					$campos .= $campo." = '".$valor."', ";
				}
			}
		}
		$campos = substr( $campos, 0, -2 );

		$sql = " UPDATE ".$tabela." SET $campos WHERE ".$chave." = $codigo";
		$db->executar($sql);
	}
	$db->commit();
	return $codigo;
}

function cargaEmendaBaseSiop( $arrDados = array() ){
	global $db;
	
	$db->executar("DELETE FROM emenda.beneficiarioemenda WHERE ano = '".date('Y')."'");
	$db->executar("DELETE FROM emenda.objetosbeneficiarioemenda WHERE ano = '".date('Y')."'");
	$db->executar("DELETE FROM emenda.sioptipoimpedimento WHERE ano = '".date('Y')."'");
	
	foreach ($arrDados as $v) {

		$codigo = $db->pegaUm("select cesid from emenda.cargaemendasiop
				where numeroemenda = '{$v->numeroEmenda}'
				and emendaano = '".date('Y')."'
									and naturezadespesa = '".$v->naturezaDespesa."'
									and fonte = '".$v->fonte."'
									and codigoparlamentar = '{$v->codigoParlamentar}'");
		
		if( strlen($v->numeroEmenda) < 8 ){
			if( strlen($v->numeroEmenda) == 2 ){
				$codigoemenda = str_pad($v->codigoParlamentar, 6, 0, STR_PAD_RIGHT).(string)$v->numeroEmenda;
			} else {
				$codigoemenda = str_pad($v->codigoParlamentar, 7, 0, STR_PAD_RIGHT).(string)$v->numeroEmenda;
			}
		} else {
			$codigoemenda = $v->numeroEmenda;
		}

		if( !empty($codigo) ){
			$sql = "UPDATE emenda.cargaemendasiop
					   SET identificadorunicolocalizador = '".$v->identificadorUnicoLocalizador."', esfera = '".$v->esfera."', codigouo = '".$v->codigoUO."',
					       codigoprograma = '".$v->codigoPrograma."', codigofuncao = '".$v->codigoFuncao."', codigosubfuncao = '".$v->codigoSubFuncao."', codigoacao = '".$v->codigoAcao."',
					       codigolocalizador = '".$v->codigoLocalizador."', naturezadespesa = '".$v->naturezaDespesa."', resultadoprimario = '".$v->resultadoPrimario."',
					       fonte = '".$v->fonte."', iduso = '".$v->idUso."', planoorcamentario = '".$v->planoOrcamentario."', codigoparlamentar = '".$v->codigoParlamentar."', nomeparlamentar = '".$v->nomeParlamentar."', numeroemenda = '".$v->numeroEmenda."',
					       codigoemenda = '".$codigoemenda."', codigopartido = '".$v->codigoPartido."', siglapartido = '".$v->siglaPartido."',
					       ufparlamentar = '".$v->ufParlamentar."', valoratual = ".($v->valorAtual ? $v->valorAtual : 'null')."
					       WHERE cesid = $codigo";
			$db->executar($sql);
			$cesid = $codigo;
		} else {
			$sql = "INSERT INTO emenda.cargaemendasiop(identificadorunicolocalizador, esfera, codigouo, codigoprograma, codigofuncao, codigosubfuncao, codigoacao, codigolocalizador,
  						naturezadespesa, resultadoprimario, fonte, iduso, planoorcamentario, codigoparlamentar, nomeparlamentar, codigoemenda, numeroemenda, emendaano, codigopartido, siglapartido, ufparlamentar, valoratual)
					VALUES ('".$v->identificadorUnicoLocalizador."',
						'".$v->esfera."',
						'".$v->codigoUO."',
						'".$v->codigoPrograma."',
						'".$v->codigoFuncao."',
						'".$v->codigoSubFuncao."',
						'".$v->codigoAcao."',
						'".$v->codigoLocalizador."',
  						'".$v->naturezaDespesa."',
						'".$v->resultadoPrimario."',
						'".$v->fonte."',
						'".$v->idUso."',
						'".$v->planoOrcamentario."',
						'".$v->codigoParlamentar."',
						'".$v->nomeParlamentar."',
						'".$codigoemenda."',
						'".$v->numeroEmenda."',
						'".date('Y')."',
						'".$v->codigoPartido."',
						'".$v->siglaPartido."',
						'".$v->ufParlamentar."',
						".($v->valorAtual ? $v->valorAtual : 'null').") returning cesid";
			$cesid = $db->pegaUm($sql);
		}

		$gnd = substr($v->naturezaDespesa, 0, 1);

		if( !empty($v->beneficiariosEmenda->beneficiarioEmenda) && $cesid ){
				
			$v->beneficiariosEmenda->beneficiarioEmenda = is_array($v->beneficiariosEmenda->beneficiarioEmenda) ? $v->beneficiariosEmenda->beneficiarioEmenda : array($v->beneficiariosEmenda->beneficiarioEmenda);
			$boEnviado = false;
				
			foreach ($v->beneficiariosEmenda->beneficiarioEmenda as $benef) {

				$sql = "INSERT INTO emenda.beneficiarioemenda(cesid, cnpjbeneficiario, nomebeneficiario, valorapuradorcl, valorrevisadobeneficiario, codigomomento, snatual, ano)
						VALUES ($cesid, '".$benef->CNPJBeneficiario."',
			    						'".$benef->nomeBeneficiario."',
			    						".($benef->valorApuradoRCL ? $benef->valorApuradoRCL : 'null').",
			    						".($benef->valorRevisadoBeneficiario ? $benef->valorRevisadoBeneficiario : 'null').",
			    						'".$benef->codigoMomento."',
			    						'".$benef->snAtual."',
			    						'".date('Y')."');";
				$db->executar($sql);

				if( !empty($benef->objetosBeneficiarioEmenda->objetoBeneficiarioEmenda) ){
					foreach ($benef->objetosBeneficiarioEmenda->objetoBeneficiarioEmenda as $obj) {						
						$sql = "INSERT INTO emenda.objetosbeneficiarioemenda(cesid, cnpjbeneficiario, descricaoobjeto, valorobjeto, justificativa, ano)
								VALUES ($cesid, '".$benef->CNPJBeneficiario."', '".$obj->descricaoObjeto."', ".($obj->valorObjeto ? $obj->valorObjeto : 'null').", '".$obj->justificativa."', '".date('Y')."') returning obeid";
						$obeid = $db->pegaUm($sql);
						
						if( !empty($obj->codigosTipoImpedimento->codigoTipoImpedimento) ){
						
							if( is_array($obj->codigosTipoImpedimento->codigoTipoImpedimento) ){
								foreach ($obj->codigosTipoImpedimento->codigoTipoImpedimento as $imp) {
									$sql = "INSERT INTO emenda.sioptipoimpedimento(obeid, cesid, codigotipoimpedimento, ano) VALUES ($obeid, $cesid, $imp, '".date('Y')."')";
									$db->executar($sql);
								}
							} else {
								$sql = "INSERT INTO emenda.sioptipoimpedimento(obeid, cesid, codigotipoimpedimento, ano) VALUES ($obeid, $cesid, {$obj->codigosTipoImpedimento->codigoTipoImpedimento}, '".date('Y')."')";
								$db->executar($sql);
							}
						}
					}
				}
			}
			$db->commit();
		}
	}
	$db->commit();
}
?>