<?php 

//echo md5('correpula2013');
//394bd0c5670359dc0cf1683e85c25263


$_REQUEST['baselogin'] = "simec_espelho_producao";

// CPF do administrador de sistemas
$_SESSION['usucpf'] = '00000000191';

if(!$_SESSION['usucpf'])

	$_SESSION['usucpforigem'] = '00000000191';

if(md5('correpula2013') == $_GET['chave'])
{
	include 'config.inc';
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";
	
	$db = new cls_banco();
	
	//Filtra municípios
	if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
		$sql = "SELECT
					ter.muncod AS codigo,
					ter.mundescricao AS descricao
				FROM
					territorios.municipio ter
				WHERE
					ter.estuf = '".$_REQUEST['estuf']."'
				ORDER BY ter.mundescricao";
			
		echo $db->monta_combo( "muncod", $sql, 'S', 'Todos', '', '', '', '215', 'N','id="muncod"');
		exit;
	}
	
	?>
	<html>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='/includes/listagem.css'/>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	
	<script type="text/javascript">
		
		function filtraEsfera(id) {
				if(id == '2'){
					document.getElementById("tr_municipio").style.display = '';
				}
				else{
					document.getElementById("tr_municipio").style.display = 'none';
				}
				
		}
		
		function filtraMunicipio(estuf) {
			if(estuf!='' && document.formulario.esfera.value == '2'){
				
				document.getElementById("tr_municipio").style.display = '';
				
				var destino = document.getElementById("td_municipio");
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: "filtraMunicipio=true&" + "estuf=" + estuf,
						asynchronous: false,
						onComplete: function(resp) {
							if(destino) {
								destino.innerHTML = resp.responseText;
							} 
						},
						onLoading: function(){
							destino.innerHTML = 'Carregando...';
						}
					});
			}
		}
		
		
		
		
		function pesquisar() {
			var btPesquisa	= document.getElementById("bt_pesquisar");
			
			if( document.formulario.ano.value == '' ){
				alert("Selecione o Ano.");
				document.formulario.ano.focus();
				return false;
			}
			if( document.formulario.esfera.value == '' ){
				alert("Selecione a Esfera.");
				document.formulario.esfera.focus();
				return false;
			}
			/*
			if( document.formulario.esfera.value == '2' ){
				if( document.formulario.uf.value == '' ){
					alert("Selecione o Estado.");
					document.formulario.esfera.focus();
					return false;
				}
			}
			*/
		
			
			btPesquisa.disabled = true;
			document.formulario.submit();
		}
	
	</script>
	
	<body>
	
	<form id="formulario" name="formulario" method="post" action="">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
			<tr>
				<td class="SubTituloDireita" valign="top"><b>Ano:</b></td>
				<td>
					<?
						$sql = "SELECT
									'2013' AS codigo,
									'2013' AS descricao
								UNION
								SELECT
									'2014' AS codigo,
									'2014' AS descricao";
						$db->monta_combo( "ano", $sql, 'S', 'Selecione...', '', '', '', '115','','','',$_REQUEST['ano'] );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top"><b>Esfera:</b></td>
				<td>
					<?
						$sql = "SELECT
									'1' AS codigo,
									'Estadual' AS descricao
								UNION
								SELECT
									'2' AS codigo,
									'Municipal' AS descricao";
						$db->monta_combo( "esfera", $sql, 'S', 'Selecione...', 'filtraEsfera', '', '', '115','','','',$_REQUEST['esfera'] );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top"><b>Estado:</b></td>
				<td>
					<?
						$estuf = ($uf==''?$_REQUEST['estuf']:$uf); 
						$sql = "SELECT
									estuf AS codigo,
									estdescricao AS descricao
								FROM
									territorios.estado
								ORDER BY
									estdescricao";
						$db->monta_combo( "estuf", $sql, 'S', 'Todos', 'filtraMunicipio', '', '', '215','','','',$estuf );
					?>
				</td>
			</tr>
			<tr id="tr_municipio" style="display: <?if($_REQUEST['esfera'] != '2') echo 'none;';?>">
				<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
				<td id="td_municipio">
				<? 
					$muncod = ($muncod==''?$_REQUEST['muncod']:$muncod); 
					$sql = "SELECT
								ter.muncod AS codigo,
								ter.mundescricao AS descricao
							FROM
								territorios.municipio ter
							WHERE
								ter.estuf = '$estuf' 
							ORDER BY ter.estuf, ter.mundescricao"; 
					$db->monta_combo( "muncod", $sql, 'S', 'Todos', '', '', '', '215', 'N','','','',$muncod);
				?>
				</td>
			</tr>
			<tr>
				<td bgcolor="#c0c0c0"></td>
				<td align="left" bgcolor="#c0c0c0">
					<input type="button" id="bt_pesquisar" value="Pesquisar" onclick="pesquisar()" />
				</td>
			</tr>
		</table>
	</form>
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
		<tr>
			<td	colspan="2">
				<?
				if($_REQUEST['esfera']){
					if($_REQUEST['esfera'] == '1'){
						echo '<br><center><b>LISTA DE ADESÃO ESTADUAL</b></center><br>';
						
						$and = "";
						if($_REQUEST['estuf']) $and .= " AND i.estuf = '".$_REQUEST['estuf']."'";
						if($_REQUEST['ano']=='2013') $and .= " AND a.tapid = 17";
						if($_REQUEST['ano']=='2014') $and .= " AND a.tapid = 30";
						
						$sql = "select i.estuf as estado from par.pfadesaoprograma a
								inner join par.instrumentounidade i on i.inuid = a.inuid
								where a.adpresposta = 'S' 
								$and
								order by 1";

						$cabecalho 		= array( "Estado");
						$tamanho		= array( '100%');
						$alinhamento	= array( 'center');
						$db->monta_lista( $sql, $cabecalho, 100, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
						
					}elseif($_REQUEST['esfera'] == '2'){
						echo '<br><center><b>LISTA DE ADESÃO MUNICIPAL</b></center><br>';
						
						$and = "";
						if($_REQUEST['estuf']) $and .= " AND m.estuf = '".$_REQUEST['estuf']."'";
						if($_REQUEST['muncod']) $and .= " AND m.muncod = '".$_REQUEST['muncod']."'";
						if($_REQUEST['ano']=='2013') $and .= " AND a.tapid = 18";
						if($_REQUEST['ano']=='2014') $and .= " AND a.tapid = 31";
						
						$sql = "select m.estuf as estado, m.mundescricao as municipio from par.pfadesaoprograma a
								inner join par.instrumentounidade i on i.inuid = a.inuid
								inner join territorios.municipio m on m.muncod = i.muncod
								where a.adpresposta = 'S' 
								$and
								order by 1,2";
						
						$cabecalho 		= array( "Estado", "Município");
						$tamanho		= array( '20%', '80%');
						$alinhamento	= array( 'center', 'left');
						$db->monta_lista( $sql, $cabecalho, 2000, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
							
					}else{
						echo '<br><center><b>LISTA DE ADESÃO</b></center><br>';
						echo '<center><b><font color=red>Não existe registros</font></b></center>';
					} 
				}
				?>
			</td>
		</tr>
	</table>
		
	</body>
	</html>
	
	<?
	
}//fecha if(md5('correpula2013') == $_GET['chave'])

?>

