<?php

if ( $_REQUEST["subacao"] == "listasubfuncao" ){
	
	if ( $_REQUEST["funcod"] ){
		
		$sfucod = $_REQUEST["sfucod"];
		
		$sql = "SELECT DISTINCT 
					sfu.sfucod as codigo, 
					sfu.sfucod || ' - ' || sfu.sfudsc as descricao
				FROM 
					emenda.emenda e
				INNER JOIN 
					emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
				INNER JOIN 
					public.ppasubfuncao sfu ON sfu.funcod = sfu.funcod
				WHERE 
					f.prgano = '{$_SESSION["exercicio"]}' AND 
					sfu.funcod = '{$_REQUEST["funcod"]}'
				ORDER BY
					descricao";
		
		$db->monta_combo("sfucod", $sql, "S", "Todas", '', '', '', '250', 'N','sfucod');
	}else{
		print 'Selecione uma função...';
	}
	die;
	
}

if ( $_REQUEST["pesquisa"] == "pesquisa" ){
	$filtros = geraFiltroResponsavel( $_REQUEST );
}

if ( $_REQUEST["insereEmendas"] == "insereEmendas" ){
	vinculaEmendasResponsavel( $_REQUEST );
}

monta_titulo( 'Vincular Emendas ao Responsável', '');

?>

<script>
function selecionaTodos() {

	var selecionaTodos 	= document.getElementById("seleciona_todos");
	var emeid 			= document.getElementsByName("emenda[]");
	
	for(var i=0; i<emeid.length; i++) {
		if(selecionaTodos.checked) {
			if(!emeid[i].checked) emeid[i].checked = true;
		} else {
			if(emeid[i].checked) emeid[i].checked = false;
		}
	}
}
</script>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script src="js/emenda.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <style>
			#div_rolagem table {
				width: 100%;                
			}
       </style>
	</head>
	<body>
		<form id="formulario" name="formulario" method="post" action="">
			<input type="hidden" value="pesquisa" name="pesquisa"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td colspan="2" class="subtitulocentro">Filtros de Pesquisa</td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="150px">Exercício:</td>
					<td>
						<?php print $_SESSION["exercicio"]; ?>
						<input type="hidden" name="exercicio" id="exercicio" value="<?php print $_SESSION["exercicio"]; ?>"/>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Unidade Orçamentária</td>
					<td>
						<?php
							
							$sql = "SELECT unidsc 
									FROM public.unidade u 
									INNER JOIN emenda.responsavel r ON r.unicod = u.unicod
									WHERE resid = {$_SESSION["emenda"]["resid"]}";
							$unicod = $db->pegaUm( $sql );
							
							print $unicod; 
						
						?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Autor da Emenda</td>
					<td>
						<?php
							
							$sql = "SELECT DISTINCT 
										a.autid as codigo,
										a.autnome as descricao 
									FROM 
										emenda.autor a
									INNER JOIN 
										emenda.emenda e ON e.autid = a.autid
									INNER JOIN 
										emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
									WHERE
										e.resid is NULL AND 
										f.prgano = '{$_SESSION["exercicio"]}'
									ORDER BY
										descricao";
							
							$db->monta_combo("autid", $sql, "S", "Todos", '', '', '', '250', 'N','autid');
						?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Ação Orçamentária</td>
					<td>
						<?php
							
							$acacod = $_REQUEST["acacod"];
						
							$sql = "SELECT DISTINCT 
										f.acacod as codigo, 
										f.acacod || ' - ' || f.acadsc as descricao
									FROM
										emenda.emenda e
									INNER JOIN 
										emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
									WHERE 
										e.resid is NULL AND 
										f.prgano = '{$_SESSION["exercicio"]}'
									ORDER BY
										descricao";
							
							$db->monta_combo("acacod", $sql, "S", "Todas", '', '', '', '250', 'N','acacod');
							
						?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Função</td>
					<td>
						<?php
							
							$funcod = $_REQUEST["funcod"];
						
							$sql = "SELECT DISTINCT 
										fu.funcod as codigo, 
										fu.funcod || ' - ' || fu.fundsc as descricao
									FROM 
										emenda.emenda e
									INNER JOIN 
										emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
									INNER JOIN 
										public.ppafuncao fu ON fu.funcod = f.funcod
									WHERE 
										e.resid is NULL AND 
										f.prgano = '{$_SESSION["exercicio"]}'
									ORDER BY
										descricao";
						
							$db->monta_combo("funcod", $sql, "S", "Todas", "listaSubfuncao(this.value, 'popup', '');", '', '', '250', 'N','funcod');
							
						?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Subfunção</td>
					<td id="subfuncao" style="color: #C0C0C0;"> 
						Selecione uma função...
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td>
						<input type='checkbox' id='seleciona_todos' onclick='selecionaTodos();' /> <b>Selecionar Todos</b>
					</td>
					<td>
						<input type="button" style="cursor: pointer;" onclick="document.getElementById('formulario').submit();" value="Pesquisar"/>
					</td>
				</tr>
			</table>
		</form>

		<form id="formEmendas" name="formEmendas" method="post" action="">
			<input type="hidden" name="insereEmendas" id="insereEmendas" value="insereEmendas"/>
			<center>
				<div id="div_rolagem" style="overflow-x: auto; overflow-y: auto; width:95%; height:300px;">
					<?php listaEmendasResponsavel( $filtros, 'popup' ); ?>
				</div>
			</center>
	
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#C0C0C0">
					<td>
						<input type="button" style="cursor: pointer;" onclick="document.getElementById('formEmendas').submit();" value="Salvar"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
<?php if ( $funcod && $_REQUEST["sfucod"] ): ?>
	<script>
		listaSubfuncao(<?php echo $funcod; ?>, 'popup', <?php echo $_REQUEST["sfucod"]; ?>);
	</script>
<?php endif; ?>