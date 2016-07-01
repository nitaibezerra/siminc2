<?php
/**
 * Página inicial do módulo Monitoramento de Obras
 * @author Fernando Bagno <fernandosilva@mec.gov.br>
 * @since 27/04/2010
 * @version 2.0
 * 
 */
if( $_POST['agrupamento'] ){
	$_SESSION["obras"]["filtros"]["agrupamento"] = $_POST['agrupamento'];
	$valida = '1';
}


ini_set("memory_limit", "2048M");
set_time_limit(0);

// Objetos da página
$obrInicio = new inicio();
$obras	   = new Obras();

// cria a sessão com o tipo de ensino selecionado
if(!$_REQUEST['orgid']) $_REQUEST['orgid'] = 3;
$_SESSION["obras"]["orgid"] = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obras"]["orgid"]; 
$_SESSION["obra"]["orgid"]  = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obra"]["orgid"]; 

/*
if ( !possuiPerfil( PERFIL_ADMINISTRADOR ) ) {
	$dados = obrPegaOrgidPermitido( $_SESSION["usucpf"] );
	$i=0;
	if ( is_array($dados) && $dados[0] ){
		foreach( $dados as $orgao ){
			$orgid[$i] = $orgao['id'];
			$i++;
		}
		if ( !in_array($_SESSION["obras"]["orgid"],$orgid) ){
			$_SESSION["obras"]["orgid"] = $orgid[0];
			header( "location:obras.php?modulo=inicio&acao=A&orgid=".$orgid[0] );
			die;
		}
	}
}
*/

// Cabeçalho padrão do SIMEC
include  APPRAIZ."includes/cabecalho.inc";



if( (obrPegaOrgidPermitido($_SESSION["usucpf"]) /*&& !possuiPerfil(PERFIL_SAA)*/) || $db->testa_superuser()){
	
	// Monta o título da dela no padrão SIMEC
	print "<br/><br>";
	//print obrMontaAbasTipoEnsino( $_SESSION["usucpf"], $_REQUEST["orgid"] );
	monta_titulo( "Lista Simplificada de Obras", "" );
	// cria a sessão com o tipo de ensino selecionado
	
	$_SESSION["obras"]["orgid"] = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obras"]["orgid"]; 
	$_SESSION["obra"]["orgid"]  = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obra"]["orgid"]; 
		
?>
	<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
	<script type="text/javascript" src="/includes/remedial.js"></script>
	<script type="text/javascript" src="/includes/superTitle.js"></script>
	<script type="text/javascript">
		var u='/obras/obras.php?modulo=inicio&acao=A&titleFor=';
	</script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	
	<form name="formulario" id="obrFormPesquisa" method="post" action="">
		<input type="hidden" name="requisicao" id="requisicao" value=""/>
		<input type="hidden" name="numero" value="" />
		<input type="hidden" name="arEntid"       id="arEntid" 	     value="<?=$_SESSION["obrasarvore"]["arEntid"] ?>" />
		<input type="hidden" name="arEntidCampus" id="arEntidCampus" value="<?=$_SESSION["obrasarvore"]["arEntidCampus"] ?>"/>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
		
			<tr>
				<td colspan="2" class="subTituloCentro">Argumentos de Pesquisa</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 210px;">Município / UF / ID da Obra / Nome da Obra:</td>
				<td>
					<?php $obrtextobusca = $_REQUEST["obrbuscasimplificada"]; ?>
					<?php print campo_texto( 'obrbuscasimplificada', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 210px;">Tipo de Ensino:</td>
				<td>
					<?
					$sql = "select orgid,orgdesc from obras.orgao where orgstatus = 'A'";
					$orgao = $db->carregar($sql);
					
					foreach($orgao as $o){
						
						$checado = "";
						if($o['orgid'] == $_REQUEST['orgid']) $checado = "checked";
						
						echo '<input type="radio" name="orgid" id="orgid" value="'.$o['orgid'].'" '.$checado.'> ' . $o['orgdesc'] . '&nbsp;&nbsp;&nbsp;';
					}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Situação da Obra:</td>
				<td>
					<?php 
		
						$stoid = $_REQUEST["stoid"];
						
						if($_SESSION["obras"]["orgid"] == ORGAO_SESU){
							$stWhere .= " and stoedsuperior = true";	
						}
						elseif($_SESSION["obras"]["orgid"] == ORGAO_SETEC){
							$stWhere .= " and stoedprofissional = true";	
						}
						elseif($_SESSION["obras"]["orgid"] == ORGAO_FNDE){
							$stWhere .= " and stoedbasica = true";	
						}
						elseif($_SESSION["obras"]["orgid"] == ORGAO_ADM){
							$stWhere .= " and stoedadm = true";	
						}
						elseif($_SESSION["obras"]["orgid"] == ORGAO_REHUF){
							$stWhere .= " and stoedhospitais = true";	
						}
						elseif($_SESSION["obras"]["orgid"] == ORGAO_MILITAR){
							$stWhere .= " and stoedmilitares = true";	
						}
						
						$sql = "SELECT 
									stoid as codigo, 
									stodesc as descricao 
								FROM 
									obras.situacaoobra
								WHERE
									stostatus = 'A'
								{$stWhere} 
								ORDER BY 
									stodesc";
					
						$db->monta_combo( "stoid", $sql, "S", "Todas", "", "", "", "", "N", "stoid" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Programa:</td>
				<td>
					<?php 
					
						$prfid = $_REQUEST["prfid"];
						
						$sql = "SELECT 
									prfid as codigo,
									prfdesc as descricao
							  	FROM 
							  		obras.programafonte
							  	WHERE
							  		orgid = {$_SESSION["obras"]["orgid"]}
							  	ORDER BY
							  		prfdesc";
					
						$db->monta_combo( "prfid", $sql, "S", "Todos", "", "", "", "", "N", "prfid" );
						
					?>
				</td>
			</tr>
			<?php if($_SESSION["obra"]["orgid"] != 1 && $_SESSION["obra"]["orgid"] != 2){ ?>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Fonte:</td>
					<td>
						<?php 
						
							$tooid = $_REQUEST["tooid"];
							
							$sql = "SELECT 
										tooid AS codigo, 
										toodescricao AS descricao
	  								FROM 
	  									obras.tipoorigemobra
	  								WHERE
	  									toostatus = 'A'
									UNION ALL
									SELECT
										9999 as codigo,
										'Não Informado' as descricao
									ORDER BY
										descricao";
						
							$db->monta_combo( "tooid", $sql, "S", "Todos", "", "", "", "", "N", "tooid" );
							
						?>
					</td>
				</tr>
			<?}?>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">% Executado da Obra:</td>
				<td>
					<table>
						<tr>
							<th>Mínimo</th>
							<th>Máximo</th>
						</tr>
						<tr>
							<?php
								
								for ($i = 0; $i <= 100; $i++ ){
									$arPercentual[] = array( 'codigo' =>  "$i" , 'descricao' => "$i%" );
								}
								
								$percentualinicial = $_REQUEST['percentualinicial'] ? $_REQUEST['percentualinicial'] : 0;
								$percentualfinal   = $_REQUEST['percentualfinal'] ? $_REQUEST['percentualfinal'] : 100;
								
								print '<td>';
								$db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial', false, $percentualinicial);
								print '</td><td>';
								$db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percentualfinal);
								print '</td>';
								
							?>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td style="width: 190px;"></td>
				<td>
					<input type="button" name="obrBtPesquisaInicio" value="Pesquisar" onclick="obrFiltraLista();" style="cursor: pointer;"/>
				</td>
			</tr>
		</table>
	</form>
		<?php 
		//fazendo o include da classe
		require_once APPRAIZ . "includes/classes/Atualizacao.class.inc";
//		$mensagem = '<p align="center" style="font-size: 15px;"><font size="4" color="red"><b>Atenção!</b></font><br><br>Existem novas atualizações.<br><br>Para visualizar as atualizações recentes clique no ícone <br><br><table><tbody><tr><td align="right"><img border="0" align="right" src="../includes/layout/montanhas/img/bt_help.png" title="Atualizações" alt="Atualizações"></td><td align="left"><font color="#dd0000"><strong>Atualizações</strong></font></td></tr></tbody></table></p>';
		$Atualizacao = new Atualizacao("float: right; position: absolute; top: 153px; right: 75px;");
	?>
	
	<?php 
	//$arPerfilIncluiObra = $_SESSION["obras"]["orgid"] == ORGAO_FNDE ? array(PERFIL_ADMINISTRADOR) : array( PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_SUPERVISORMEC, PERFIL_GESTORUNIDADE );
	$arPerfilIncluiObra = $_SESSION["obras"]["orgid"] == ORGAO_FNDE ? array(PERFIL_SUPERUSUARIO) : array( PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_SUPERVISORMEC, PERFIL_GESTORUNIDADE );
	$permissaoIncluiObra = possuiPerfil( $arPerfilIncluiObra );
	
	//if( $permissaoIncluiObra && $_SESSION['obras']['orgid'] <> 3 ): 
	//if( $permissaoIncluiObra ):
	?>
	<!-- 
	<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
		<tr>
			<td style="font-weight: bold;">
				<a style="cursor: pointer;" onclick="obrIrParaCaminho( '', 'novaobra', <?php print $_SESSION["obras"]["orgid"]; ?> );" title="Clique para incluir uma nova obra no sistema">
					<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> Incluir nova obra
				</a>
			</td>
		</tr>
	</table>
	 -->
	<?php //endif; ?>
	
	<?php
		// Valida a listas cadastradas no sistema
		//Se Ensino Básico com orgid = 3 e valida != 1, listar Obras.  
		if( $valida <> '1' && !($_SESSION["obras"]["filtros"]["agrupamento"]) ){
			$_SESSION["obras"]["filtros"]["agrupamento"] = 'O';
		//Senão se Ensino Superior com orgid = 1 ou Ensino Profissional com orgid = 2 e valida != 1, listar Unidades.  
		}
//		elseif($_SESSION['obras']['orgid'] <> 3 && $valida <> '1'){
//			$_SESSION["obras"]["filtros"]["agrupamento"] = 'U';
//		}
//		$_SESSION["obras"]["filtros"]["agrupamento"] = 'O';
		// Monta a lista de obras cadastradas no sistema 
//		ver($_SESSION["obras"]["filtros"]["filtros"]);
		//$obrInicio->listaDeObras( $_SESSION["obras"]["filtros"]["filtros"] );
		
		if( !empty( $_REQUEST["obrbuscasimplificada"] ) ){
			
			$filtro .= " AND ( upper(oi.obrdesc) ilike upper('%{$_REQUEST["obrbuscasimplificada"]}%') OR ";
			$filtro .= " oi.obrid =".(int)$_REQUEST["obrbuscasimplificada"]." OR "; // busca pelo campo ID
			$filtro .= " ed.estuf = '{$_REQUEST["obrbuscasimplificada"]}' OR ";
			$filtro .= " tm.mundescricao ILIKE '%{$_REQUEST["obrbuscasimplificada"]}%' OR removeacento(tm.mundescricao) ilike (removeacento('%{$_REQUEST["obrbuscasimplificada"]}%')) ) ";
			
		}
		
		if( !empty( $_REQUEST["stoid"] ) ){
			
			$filtro .= " AND oi.stoid = {$_REQUEST["stoid"]} ";
			
		}

		if( !empty( $_REQUEST["prfid"] ) ){
			
			$filtro .= " AND oi.prfid = {$_REQUEST["prfid"]} ";
			
		}

		if( !empty( $_REQUEST["tooid"] ) ){
			
			if($_REQUEST["tooid"]==9999){
				$filtro .= " AND torgobr.tooid IS NULL";
			}else{
				$filtro .= " AND torgobr.tooid = {$_REQUEST["tooid"]} ";
			}
			
		}

		if ( (int)$_REQUEST["percentualinicial"] > 0 ) {
			
			$perc = (int)$_REQUEST["percentualfinal"] == 100 ? 110 : $_REQUEST["percentualfinal"];
			$filtro .= " AND ( (oi.obrpercexec BETWEEN {$_REQUEST["percentualinicial"]} AND {$perc}))";
			
		}elseif ( $_REQUEST["percentualinicial"] == '0' ) {
			if ( (int)$_REQUEST["percentualfinal"] > 0 ) {
				if( !((int)$_REQUEST["percentualfinal"] == 100) ){
	
					$perc = (int)$_REQUEST["percentualfinal"] == 100 ? 110 : $_REQUEST["percentualfinal"];
					$filtro .= " AND ( (oi.obrpercexec IS NULL OR oi.obrpercexec BETWEEN {$_REQUEST["percentualinicial"]} AND {$perc}))";
				}
			}elseif ( $_REQUEST["percentualfinal"] == '0' ) {
				
				$filtro .= " AND ( (oi.obrpercexec = 0 OR oi.obrpercexec IS NULL))";
				
			}
		}		
		
		$obrInicio->listaObras( "", $filtro, '', '', '', 'simplificada' );
	?>
	
	<?php //if( $permissaoIncluiObra && $_SESSION['obras']['orgid'] <> 3 ): ?>
	<!-- 
	<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
		<tr>
			<td style="font-weight: bold;">
				<a style="cursor: pointer;" onclick="obrIrParaCaminho( '', 'novaobra', <?php print $_SESSION["obras"]["orgid"]; ?> );" title="Clique para incluir uma nova obra no sistema">
					<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> Incluir nova obra
				</a>
			</td>
		</tr>
	</table>
	 -->
	<?php //endif; ?>
<?php }else{ ?>

	<?php print "<br/>"; ?>
	<?php monta_titulo( "Monitoramento de Obras", "" ); ?>
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr style="text-align: center; color: #ff0000;">
			<td>
				Usuário sem permissões para visualizar obras
			</td>
		</tr>
	</table>
<?php } ?>