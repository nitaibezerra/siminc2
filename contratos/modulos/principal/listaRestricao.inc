<?php
include_once APPRAIZ."contratos/classes/Restricao.class.inc";


if ($_GET['ctrid']) {
    $_SESSION['ctrid'] = $_GET['ctrid'];
}

if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	exit;
}
switch ( $_REQUEST['requisicao'] ){
	case 'apagar':
		$restricao = new Restricao( $_REQUEST['rstid'] );
		$restricao->rststatus = 'I';
		$restricao->salvar();
		$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location = '?modulo=principal/listaRestricao&acao={$_GET['acao']}';
			 </script>");

}
// Por padrão os campos vem habilitados
// $desabilitado   = false;
// $somenteLeitura = 'S';
// $verifica = true;
$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$somenteLeitura = $acesso['leitura'];

$perfis 			= arrayPerfil();
// $arPerfisBloqEdicao = array(PERFIL_CONSULTA_UNIDADE,
// 		PERFIL_CONSULTA_GERAL);

// $arInt = array_intersect($perfis, $arPerfisBloqEdicao);

// //MÉTODO PARA A VERIFICAR SE O CONTRATO PERTENCE À UNIDADE DO USUARIO E SE O USUARIO PODERÁ EDITAR NESTA TELA
// if($_SESSION['ctrid'])
// 	$verifica = verificaResponsabilidade($_SESSION['ctrid'],$perfis,array(PERFIL_GESTOR_FINANCEIRO_UNIDADE,PERFIL_EQUIPE_TECNICA_UNIDADE));
// //verifica se nao esta no bloqueado e se caso nao esteja bloq ele tem direito a editar
// if( count($arInt) != 0 || (count($arInt) == 0 && $verifica==false)) {
// 	$desabilitado 	= true;
// 	$somenteLeitura = 'N';
// }
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

$db->cria_aba( $abacod_tela, $url, $parametros);
//$titulo_modulo = "Problemas na execução do contrato";
monta_titulo( $titulo_modulo, obrigatorio() . ' Indica campo obrigatório' );

if($_SESSION['ctrid']){
	montaCabecalhoContrato( $_SESSION['ctrid'] );
}

if($_POST){
	extract($_POST);
}
?> 
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td align="center">
			<?php 
			if($somenteLeitura == "S"):
			?>
			<input type="button" 
				   value="Inserir Nova Restrição e Providência" 
				   style="margin-top:3px; margin-bottom:3px;"
				   onclick="janela = window.open('?modulo=principal/cadRestricao&acao=<?php echo $_GET['acao'] ?>', 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=550' ); janela.focus();"/>
			<?php 
			endif;
			?>	   
		</td>
	</tr>
</table>
<?php

$acao = "'<center>
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: alterarRest(\'' || r.rstid || '\');\"
	 					title=\"Alterar\">
	 				<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: excluirRest(\'' || r.rstid || '\');\"
	 					title=\"Excluir\">
	 			  </center>'";




$sql = "SELECT
			{$acao} AS acao,
			tr.tprdsc,
			TO_CHAR(r.rstdtinclusao, 'DD/MM/YYYY') AS rstdtinclusao,
			r.rstdsc,
			r.rstdscprovidencia,
			TO_CHAR(r.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
			usu.usunome AS criadopor,
			CASE WHEN r.rstsituacao = TRUE THEN TO_CHAR(r.rstdtsuperacao, 'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
			sup.usunome as ususuperacao
			FROM
			contratos.restricao r
			JOIN contratos.tiporestricao tr ON tr.tprid = r.tprid AND
			tr.tprstatus = 'A'
			JOIN seguranca.usuario usu ON usu.usucpf = r.usucpf
			LEFT JOIN seguranca.usuario sup ON sup.usucpf = r.usucpfsuperacao
			WHERE
			r.rststatus = 'A' AND ctrid=".$_SESSION['ctrid'] ;

$cabecalho = array("Ação", "Tipo de Restrição", "Data da Inclusão", "Restrição", "Providência", "Previsão da Providência", "Criado Por", "Superação", "Superado Por");
$db->monta_lista($sql,$cabecalho,100,5,'N','center','');

?>
<script type="text/javascript">

function alterarRest( rstid ){
	janela = window.open('?modulo=principal/cadRestricao&acao=<?php echo $_GET['acao'] ?>&rstid=' + rstid, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=550' ); 
	janela.focus();
}

function excluirRest( rstid ){
	if ( confirm('Deseja apagar esta Restrição?') ){
		location.href = '?modulo=principal/listaRestricao&acao=<?php echo $_GET['acao'] ?>&rstid=' + rstid + '&requisicao=apagar';
	}
}

</script>
