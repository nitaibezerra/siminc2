<?php

if( $_REQUEST["obrid"] ){
	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	unset($_SESSION["obra"]);
	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
	$obrid = $_SESSION["obra"]["obrid"]; 
}

obras_verificasessao();

$obras = new Obras();
$dobras = new DadosObra(null);

if ($_REQUEST["requisicao"]){
	$obras->DeletarRestricao($_REQUEST["rstoid"]);
}

$restricao = new DadosRestricao();

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 

?>

<br/>

<?
montaAbaObras($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Restrições e Providências' );

echo $obras->CabecalhoObras();

if($_SESSION['obra']['obrid']){
		
	$resultado = $restricao->busca($_SESSION['obra']['obrid']);	
	$dados = $restricao->dados($resultado);
	
}

//VERIFICA PERMISSÃO DE ACESSO
if($_SESSION['obras']['orgid'] == ORGAO_FNDE && !possuiPerfil(PERFIL_SUPERUSUARIO)){
	if(possuiPerfil(PERFIL_SUPERVISORUNIDADE)){
		echo "<script>
				alert('Você não possui permissão para ver esta tela!');
				history.back(-1);
			  </script>";
		die;
	}
}

function listaRestricao(){
	
	global $db;
	
	#Verifica se no parecer do ckeck list esta aba foi bloqueiada
	$boBloqueiaAbaCleckList = verificaCheckListBloqueiaAba($titulo_modulo, $_SESSION['obra']['obrid']);
	
	if( possuiPerfil(array(PERFIL_EMPRESA) ) ){
		$boBloqueioAba = false;
		if( $boBloqueiaAbaCleckList[0] == 't') {
			$boBloqueioAba = true;
		}
	}
	
	$sql = "SELECT
				rstoid,
				CASE WHEN fsrid is not null THEN fsrdsc ELSE 'Não Informada' END as fase,
				to_char(rstdtinclusao,'DD/MM/YYYY') as datainclusao,
				rstdesc,
				trtdesc,
				rstdescprovidencia,
				to_char(rstdtprevisaoregularizacao,'DD/MM/YYYY') as rstdtprevisaoregularizacao,
				CASE WHEN rstsituacao = true THEN to_char(rstdtsuperacao,'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
				usu.usucpf AS cpfcriador,
				usu.usunome AS criadopor,
				sup.usunome as ususuperacao
			FROM
				obras.restricaoobra res
			INNER JOIN 
				obras.tiporestricao USING (trtid)
			LEFT JOIN
				obras.faserestricao USING (fsrid)
			INNER JOIN
			 	seguranca.usuario usu USING (usucpf)
			LEFT JOIN
			 	seguranca.usuario sup on res.usucpfsuperacao = sup.usucpf	 
			WHERE
				rststatus = 'A' AND
				obrid = " . $_SESSION["obra"]["obrid"];
	$arrDados = $db->carregar( $sql );
	if(!$arrDados){
		echo '
			<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr>
					<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
				</tr>
		</table>';
		
	}else {
	
		echo "
			<thead>
				<tr style=\"background-color: #e0e0e0\">
					<td style=\"font-weight:bold; text-align:center; width:3%\">Ação</td>
					<td style=\"font-weight:bold; text-align:center; width:5%\">Fase da Restrição</td>
					<td style=\"font-weight:bold; text-align:center; width:5%\">Tipo de Restrição</td>
					<td style=\"font-weight:bold; text-align:center; width:2%\">Data da Inclusão</td>
					<td style=\"font-weight:bold; text-align:center; width:25%\">Restrição</td>
					<td style=\"font-weight:bold; text-align:center; width:25%\">Providência</td>
					<td style=\"font-weight:bold; text-align:center; width:2%\">Previsão da Providência</td>
					<td style=\"font-weight:bold; text-align:center; width:2%\">Superação</td>
					<td style=\"font-weight:bold; text-align:center; width:10%\">Criado por</td>
					<td style=\"font-weight:bold; text-align:center; width:10%\">Superado por</td>
				</tr>
			</thead>";
	
	}
	
	$copiaObra = verificaCopiaObra($_SESSION["obra"]["obrid"]);
	
	
	$count = 1;
	$soma = 0;
	
	if($arrDados){
		foreach($arrDados as $dados){
			$cor = "#f4f4f4";
			$count++;
			
			if ($count % 2){
				$cor = "#e0e0e0";
			}
			
			$rstoid = $dados["rstoid"];
			$fsrdsc = $dados["fase"];
			$dataiInclusao = $dados["datainclusao"];
			$rstdesc = $dados["rstdesc"];
			$trtdesc = $dados["trtdesc"];
			$rstdescprovidencia = $dados["rstdescprovidencia"];
			$rstdtprevisaoregularizacao = $dados["rstdtprevisaoregularizacao"];
			$rstdtsuperacao = $dados["rstdtsuperacao"];
			$criadoPor  = $dados["criadopor"];
			$cpfCriador = $dados["cpfcriador"];
			$usuSuperacao = $dados["ususuperacao"];
			
			$botaoAlteraRestricao = "<a><img src='/imagens/alterar.gif' border=0 title='Editar' style='cursor: pointer' onclick='window.open(\"?modulo=principal/inserir_restricao&acao=A&requisicao=alterar&rstoid=". $rstoid . "\", \"inserirRestricao\", \"menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480\");'></a>";
			
			if( ($db->testa_superuser() || possuiPerfil(PERFIL_ADMINISTRADOR) || $cpfCriador === $_SESSION['usucpf']) )
			{
				if( $boBloqueioAba ){
					$botaoExcluirRestricao = "<a><img src='/imagens/excluir_01.gif' border=0 title='Excluir' style='cursor: pointer;'></a>";
				} else {
					$botaoExcluirRestricao = "<a><img src='/imagens/excluir.gif' border=0 title='Excluir' style='cursor: pointer;' onclick=\"javascript:ExcluirRestricao('" . $rstoid . "');\"></a>";
				}
			}
			//else if($cpfCriador != $_SESSION['usucpf'])
			else
			{
					$botaoExcluirRestricao = "<a><img src='/imagens/excluir_01.gif' border=0 title='Excluir' style='cursor: pointer;'></a>";
			}
			
			if($copiaObra)
			{
					$botaoAlteraRestricao = "<a><img src='/imagens/alterar_01.gif' border=0 title='Editar' style='cursor: pointer' </a>";
					$botaoExcluirRestricao = "<a><img src='/imagens/excluir_01.gif' border=0 title='Excluir' style='cursor: pointer;'></a>";
			}
			
			echo "
				<tr bgcolor=\"" . $cor . "\">
					<td align='center'>
						$botaoAlteraRestricao
						&nbsp$botaoExcluirRestricao
					</td>
					<td align='center'>".$fsrdsc."</td>
					<td align='center'>".$trtdesc."</td>
					<td align='center'>".$dataiInclusao."</td>
					<td>".$rstdesc."</td>
					<td>".$rstdescprovidencia."</td>
					<td align='center'>".$rstdtprevisaoregularizacao."</td>
					<td align='center'>".$rstdtsuperacao."</td>
					<td align='center'>".$criadoPor."</td>
					<td align='center'>".$usuSuperacao."</td>
				</tr>";
		}
	}
}
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td align="center">
			<?php if($habilitado){ ?>
				<input type="button" value="Inserir Nova Restrição e Providência" style="padding: 1px; padding-left: 20px; padding-right: 20px; margin-top:3px; margin-bottom:3px;" 
				onclick="janela = window.open('obras.php?modulo=principal/inserir_restricao&acao=A', 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480' ); janela.focus();"/>
			<?php } ?>
		</td>
	</tr>
</table>

<center>
	<table class='tabela' style="width:95%;" cellpadding="3">
		<?php listaRestricao(); ?>
	</table>
</center>
<?php chkSituacaoObra(); ?>