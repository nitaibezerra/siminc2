<?php 

//echo md5('secretariacorrepula2013');
//cbadb590831d3e40ce76f788c69e9b5f


$_REQUEST['baselogin'] = "simec_espelho_producao";

// CPF do administrador de sistemas
$_SESSION['usucpf'] = '00000000191';

if(!$_SESSION['usucpf'])

	$_SESSION['usucpforigem'] = '00000000191';

if(md5('secretariacorrepula2013') == $_GET['chave'])
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
					echo '<center><a href="http://pdeinterativo.mec.gov.br/listaComiteCorrePula.php?chave=b74d8cf84f4740c2cbd43bee6a18abf8&esfera='.$_REQUEST['esfera'].'&estuf='.$_REQUEST['estuf'].'&muncod='.$_REQUEST['muncod'].'"><b><font color=blue>Clique aqui para visualizar os membros dos comitês</font></b></a></center>';
					echo '<br>';
					if($_REQUEST['esfera'] == '1'){
						echo '<br><center><b>LISTA DE SECRETARIAS ESTADUAIS</b></center><br>';
						
						$and = "";
						if($_REQUEST['estuf']) $and .= " AND ent.estuf = '".$_REQUEST['estuf']."'";
						
						$sql = "select 	
								ent.estuf UF ,
								ent.entnome Nome,
								ent.entemail Email,
								'('||ent.entnumdddcomercial||') '||ent.entnumcomercial as telefone,
								ent.endlog || ' Nº ' || ent.endnum || ' - ' || ent.endcom as endereco,
								ent.endbai as bairro
								from par.entidade ent
								where ent.entstatus = 'A' AND dutid=9
								$and
								order by 1,2";
						//dbg($sql,1);
						$cabecalho 		= array( "Estado", "Secretaria", "E-mail", "Telefone", "Endereço", "Bairro");
						$tamanho		= array( '10%','20%','20%','10%','10%','20%','10%');
						$alinhamento	= array( 'center','left','left','left','left','left','left');
						$db->monta_lista( $sql, $cabecalho, 1000, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
						
					}elseif($_REQUEST['esfera'] == '2'){
						echo '<br><center><b>LISTA DE SECRETARIAS MUNICIPAIS</b></center><br>';
						
						$and = "";
						if($_REQUEST['estuf']) $and .= " AND ent.estuf = '".$_REQUEST['estuf']."'";
						if($_REQUEST['muncod']) $and .= " AND ent.muncod = '".$_REQUEST['muncod']."'";
						
						$sql = "select mun.estuf UF ,
								--ent.entid,
								-- mun.muncod as IBGE,
								mun.mundescricao Municipio,
								ent.entnome Nome,
								-- ent.entnumcpfcnpj CPF,
								ent.entemail Email,
								'('||ent.entnumdddcomercial||') '||ent.entnumcomercial as telefone,
								ent.endlog || ' Nº ' || ent.endnum || ' - ' || ent.endcom as endereco,
								ent.endbai as bairro
								from par.entidade ent
								inner join territorios.municipio mun         on mun.muncod = ent.muncod
								where ent.entstatus = 'A' AND dutid=8 
								$and
								order by uf,mun.mundescricao";
						//dbg($sql,1);
						$cabecalho 		= array( "Estado", "Município", "Secretaria", "E-mail", "Telefone", "Endereço", "Bairro");
						$tamanho		= array( '10%','20%','20%','10%','10%','20%','10%');
						$alinhamento	= array( 'center','left','left','left','left','left','left');
						$db->monta_lista( $sql, $cabecalho, 1000, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
													
					}else{
						echo '<br><center><b>LISTA DE ADESÃO</b></center><br>';
						echo '<center><b><font color=red>Não existe registros</font></b></center>';
					} 
					
					
					echo '<br>';
					echo '<center><a href="http://pdeinterativo.mec.gov.br/listaComiteCorrePula.php?chave=b74d8cf84f4740c2cbd43bee6a18abf8&esfera='.$_REQUEST['esfera'].'&estuf='.$_REQUEST['estuf'].'&muncod='.$_REQUEST['muncod'].'"><b><font color=blue>Clique aqui para visualizar os membros dos comitês</font></b></a></center>';
					
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

