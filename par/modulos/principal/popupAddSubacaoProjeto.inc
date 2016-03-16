<?php

global $db;

$inuid = $_SESSION['par']['inuid'];

if( empty($inuid) ) {
	echo "<script>
			alert('A sessão foi perdida!');
			window.close();
		  </script>";
	die;
}

if ($_POST['requisicao'] == 'excluirNotaEmpenho') {
    $empid = $_POST['empid'];
    $preid = $_POST['preid'];
    $strLabel = ($_POST['tipo'] == '2') ? 'spar' : ''; #Concatena o nome da tabela de acordo com o tipo da obra PAR: empenhoobraspar PAC: empenhoobra
    
    $sql = "select coalesce(v.saldo,0) from par.v_dadosempenhos v where empid = $empid";
    $saldoEmpenho = $db->pegaUm($sql);
    
    if( (int)$saldoEmpenho == (int)0 ){
    	$sql = "update par.empenho set empstatus = 'I' where (empid = {$empid} OR empidpai = {$empid})";
       	$db->executar($sql);
       	
       	$empidFilho = $db->pegaUm("select empid from par.empenho where empidpai = $empid");
       	$empidFilho = ($empidFilho ? $empidFilho : '0');
       	
    	$sql = "update par.empenhoobra{$strLabel} set eobstatus = 'I' where empid in ($empidFilho, $empid)";
       	$db->executar($sql);
       	$db->commit();
		
		$sql = "SELECT empnumero FROM par.empenho WHERE empid = {$empenho['empid']}";
		$empnumero = $db->pegaUm($sql);
			
		insereHistoricoStatusObra( $empenho['empid'], Array(), 'I', "Obra inativada pelo canelamento do empenho $empnumero" );
							
		inativaObras2SemSaldoEmpenho( $empenho['empid'], Array() );
		
    } else {    
    	$sql = "SELECT empid FROM par.empenho WHERE (empid = {$empid} OR empidpai = {$empid}) AND empcodigoespecie IN ('03', '01', '13')";
    	$arrEmpenho = $db->carregar($sql);
    	
	    if ($arrEmpenho) {
	        foreach ($arrEmpenho as $empenho) {
	            $sql = "update par.empenhoobra{$strLabel} set eobstatus = 'I' where preid = {$preid} and empid = {$empenho['empid']};";
	            $db->executar($sql);
	        }
		
			$sql = "SELECT empnumero FROM par.empenho WHERE empid = {$empenho['empid']}";
			$empnumero = $db->pegaUm($sql);
				
			insereHistoricoStatusObra( $empenho['empid'], Array(), 'I', "Obra inativada pelo canelamento do empenho $empnumero" );
								
			inativaObras2SemSaldoEmpenho( $empenho['empid'], Array() );
			
	        echo ($db->commit()) ? '1':'0';
		die();
	    }
    }
    exit();
}

if ($_POST['requisicaoAjax'] == 'desvinculaObra')
{
	ob_clean();

	$preid 	= $_POST['preid'];
	$tipo	= $_POST['tipo'];
	$proid  = $_POST['proid'];


	if($tipo == 'PAC'){
		$sql2 = "UPDATE  par.processoobraspaccomposicao SET pocstatus = 'I' WHERE preid = {$preid} and proid =  {$proid}";
		if($db->executar( $sql2))
			echo ($db->commit()) ? '1':'0';
	} elseif( $tipo == 'OBRAS' ){
		$sql2 = "UPDATE  par.processoobrasparcomposicao SET pocstatus = 'I' WHERE preid = {$preid} and proid =  {$proid}";
		if($db->executar( $sql2))
			echo ($db->commit()) ? '1':'0';
	}
	exit();
}

if( $_POST['requisicaoReforco'] == 'adicionaReforco' && $_POST['sbdreforco'] ){ //Adiciona reforço
	$sql = "UPDATE par.subacaodetalhe SET sbdreforco = TRUE WHERE sbdid = ".$_POST['sbdreforco'];
	$db->executar($sql);
	$db->commit();
	echo "<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.reload();
				window.close();
	 		</script>";
	die();
}


// Salvar a redistribuição do empenho
if( $_POST['requisicao'] == 'salvarRedistribuirEmpenho' )
{
	salvarRedistribuirEmpenho($_POST);
	exit();
}
if( $_POST['requisicao'] == 'CarregaModalredistribuirEmpenhoObra' )
{
	CarregaModalredistribuirEmpenhoObra($_POST);
	exit();	
}

if( $_POST['requisicao'] == 'CarregaModalredistribuirEmpenho' )
{
	CarregaModalredistribuirEmpenho($_POST);
}


if( $_POST['requisicao'] == 'salvar' ){
	
	switch ($_POST['tipo']) {
		case 1: #Subações Genérico do PAR
			$sqlDelete = "DELETE FROM par.processoparcomposicao WHERE sbdid not in (select pc.sbdid from
																					par.processoparcomposicao pc 
																				    inner join par.subacaodetalhe sd on sd.sbdid = pc.sbdid
																					inner join par.empenhosubacao es on es.sbaid = sd.sbaid and es.eobano = sd.sbdano and eobstatus = 'A'
																				    inner join par.empenho e on e.empid = es.empid and empstatus = 'A' 
																				where pc.ppcstatus = 'A' and pc.prpid = {$_REQUEST['cod']}) AND prpid = {$_REQUEST['cod']} ";
			$db->executar($sqlDelete);
			if( is_array($_POST['codigo']) ){
				$sqlInsert = "";
				foreach( $_POST['codigo'] as $sbdid ){
					$sqlInsert .= "INSERT INTO par.processoparcomposicao ( prpid, sbdid ) VALUES ( ".$_REQUEST['cod'].", ".$sbdid." ); ";
				}
				$db->executar($sqlInsert);
			}
		break;
		case 2: #Obras do PAR
			$sqlDelete = "DELETE FROM par.processoobrasparcomposicao WHERE preid not in (select pc.preid from
																								par.processoobrasparcomposicao pc 
																								inner join par.empenhoobrapar eop on eop.preid = pc.preid and eobstatus = 'A'
																								inner join par.empenho e on e.empid = eop.empid and empstatus = 'A' 
																							where pc.proid = {$_REQUEST['cod']}) AND proid = {$_REQUEST['cod']}";
			$db->executar($sqlDelete);
			if( is_array($_POST['codigo']) ){
				$sqlInsert = "";
				foreach( $_POST['codigo'] as $preid ){
					$sqlInsert .= "INSERT INTO par.processoobrasparcomposicao ( proid, preid ) VALUES ( ".$_REQUEST['cod'].", ".$preid." ); ";
				}
				$db->executar($sqlInsert);
			}
		break;
		case 3: #Obras do PAC
			$sqlDelete = "DELETE FROM par.processoobraspaccomposicao WHERE preid not in (select pc.preid from
																								par.processoobraspaccomposicao pc 
																								inner join par.empenhoobra eop on eop.preid = pc.preid and eobstatus = 'A'
																								inner join par.empenho e on e.empid = eop.empid and empstatus = 'A' 
																							where pc.pocstatus = 'A' and pc.proid = {$_REQUEST['cod']}) AND proid = {$_REQUEST['cod']}";
			$db->executar($sqlDelete);
			if( is_array($_POST['codigo']) ){
				$sqlInsert = "";
				foreach( $_POST['codigo'] as $preid ){
					$sqlInsert .= "INSERT INTO par.processoobraspaccomposicao ( proid, preid ) VALUES ( ".$_REQUEST['cod'].", ".$preid." ); ";
				}
				$db->executar($sqlInsert);
			}
		break;
	}
		
	$db->commit();
	echo "<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.reload();
				window.close();
	 		</script>";
	die();
}

switch ($_REQUEST['tipo']) {
	case 1: #Subações Genérico do PAR
		$sql = sqlSubacoesPAR($_REQUEST['cod']);
		$sqlEmpenho = sqlSubacaoEmpenhoPAR($_REQUEST['cod']);
		$titulo1 = 'Adicionar/Remover Subação';
		$titulo2 = 'Selecione a(s) Subação(ões) e clique no botão Salvar para adicioná-las ou removê-las do Processo';
		$tituloEmpenho1 = 'Subações Empenhadas';
		$tituloEmpenho2 = 'Subações Empenhadas nesse Processo';
		$arProcesso = $db->pegaLinha("select 
											substring(p.prpnumeroprocesso, 12, 4) as ano, 
										    p.prpnumeroprocesso as processo, 
										    case when p.sisid = 57 then 'Subações de Emendas no PAR'
										                        	else 'Subações Genérico do PAR' end as tipo
										from par.processopar p where prpid = {$_REQUEST['cod']}");
	break;
	case 2: #Obras do PAR
		$sql = sqlObrasPAR($_REQUEST['cod']);
		$sqlEmpenho = sqlObrasEmpenhoPAR($_REQUEST['cod']);
		$titulo1 = 'Adicionar/Remover Obras';
		$titulo2 = 'Selecione a(s) Obra(s) e clique no botão Salvar para adicioná-las ou removê-las do Processo';
		$tituloEmpenho1 = 'Obras Empenhadas';
		$tituloEmpenho2 = 'Obras Empenhadas nesse Processo';
		$arProcesso = $db->pegaLinha("select 
											substring(p.pronumeroprocesso, 12, 4) as ano, 
										    p.pronumeroprocesso as processo, 
										    case when p.sisid = 57 then 'Obras de Emendas' else 'Obras PAR' end as tipo
										from par.processoobraspar p where p.prostatus = 'A' and proid = {$_REQUEST['cod']}");
	break;
	case 3: #Obras do PAC
		$sql = sqlObrasPAC($_REQUEST['cod']);
		$sqlEmpenho = sqlObrasEmpenhoPAC($_REQUEST['cod']);
		$titulo1 = 'Adicionar/Remover Obras';
		$titulo2 = 'Selecione a(s) Obra(s) e clique no botão Salvar para adicioná-las ou removê-las do Processo';
		$tituloEmpenho1 = 'Obras Empenhadas';
		$tituloEmpenho2 = 'Obras Empenhadas nesse Processo';
		$arProcesso = $db->pegaLinha("select 
											substring(p.pronumeroprocesso, 12, 4) as ano, 
										    p.pronumeroprocesso as processo, 
										    'Obras do PAC' as tipo
										from par.processoobra p where proid = {$_REQUEST['cod']}");
	break;
}
//ver(simec_htmlentities($sql), simec_htmlentities($sqlEmpenho), d);
if( $sql && $sqlEmpenho ){
	$arrDados = $db->carregar($sql);
	$arrEmpenho = $db->carregar($sqlEmpenho);
}
$arrDados = $arrDados ? $arrDados : array();
$arrEmpenho = $arrEmpenho ? $arrEmpenho : array();

echo cabecalhoProcesso($arProcesso);
monta_titulo( $titulo1, $titulo2);

?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
				<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
		<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
		<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
		<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript" src="../estrutura/js/funcoes.js"></script>
		
	</head>
	<body>
		<form name="formulario" id="formulario" method="post" action="" >
			<input type="hidden" name="requisicao" id="requisicao" value="">
			<input type="hidden" name="cod" id="cod" value="<?=$_REQUEST['cod'] ?>">
			<input type="hidden" name="tipo" id="tipo" value="<?=$_REQUEST['tipo'] ?>">
			<input type="hidden" name="sbdreforco" id ="sbdreforco" value="">
			<input type="hidden" name="requisicaoReforco" id ="requisicaoReforco" value="">
			<table width="95%" align="center"  border="0" cellspacing="0" cellpadding="0">
			<tr><td valign="top">
			<div style="overflow-x: auto; overflow-y: auto; width:100%; height:200px;">
			<?if( $arrDados ){ ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
			<tr>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Ação</strong></td>
					<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Preid</strong></td>
				<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Obrid</strong></td>
				<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Descrição</strong></td>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Ano</strong></td>
			</tr> 
			</thead>
			<tbody>
				<? 
				foreach ($arrDados as $key => $v) {
					$key % 2 ? $cor = "#dedfde" : $cor = "";
				?>
					<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td align="left" title="Ação"><?=$v['acao']; ?></td>
						<td align="left" title="Ação"><?=$v['preid']; ?></td>
						<td align="left" title="Ação"><?=$v['obrid']; ?></td>
						<td align="left" title="Descrição"><?=$v['descricao']; ?></td>
						<td align="left" title="Ano"><?=$v['ano']; ?></td>
					</tr>
				<?
				}
				?>
			</tbody>
			</table>
		<?} else { ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tbody>
				<tr>
					<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
				</tr>
			</tbody>
			</table>
		<?} ?>
		</div>
		<table align="center" border="0" width="100%" cellpadding="3" cellspacing="2">
			<tr bgcolor="#D0D0D0">
				<td>
					<center><input type="button" onclick="javascript: salvar();" value="Salvar"></center>
				</td>
			</tr>
		</table>
		</td></tr>
		</table>
	</form>
	<?php
		monta_titulo( $tituloEmpenho1, $tituloEmpenho2 );
	?>
	<table cellspacing="0"  border="0"  align="center" style="border-top: none; border-bottom: none;" class="tabela">
		<tr>
			<td width="2%" bgcolor="#eeeeee">
				
			</td>
			<td bgcolor="#eeeeee">
				<img align="absmiddle" src="../imagens/money_cancel.gif" style="cursor:pointer;" title="Redistribuir Empenho" >
				<b>= Redistribuir Empenho. </b>
			</td>
		</tr>
	</table>
	
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="0">
	<tr><td valign="top">
	<div style="overflow-x: auto; text-align:center; overflow-y: auto; width:97.5%; height:200px;">				
		<?if( $arrEmpenho ){ ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
			<tr>
				<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Ação</strong></td>
				<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Preid</strong></td>
				<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Obrid</strong></td>
				<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Descrição</strong></td>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Ano</strong></td>
			</tr> 
			</thead>
			<tbody>
				<? 
				foreach ($arrEmpenho as $key => $v) {
					$key % 2 ? $cor = "#dedfde" : $cor = "";
				?>
					<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td align="left" title="Ação"><?=$v['acao']; ?></td>
						<td align="left" title="Ação"><?=$v['preid']; ?></td>
						<td align="left" title="Ação"><?=$v['obrid']; ?></td>
						<td align="left" title="Descrição"><?=$v['descricao']; ?></td>
						<td align="left" title="Ano"><?=$v['ano']; ?></td>
					</tr>
				<?
				}
				?>
			</tbody>
			</table>
		<?} else { ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tbody>
				<tr>
					<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
				</tr>
			</tbody>
			</table>
		<?} ?>
	</div>
	</td></tr>
	</table>
	
		<div id="div_redistribuir" title="Tela de Redistribuição de Empenho" style="display: none; text-align: center;">
			
			<div id="div_obra_distribuicao" >
				
			
			</div> 
			<?php monta_titulo( "Remanejar Empenho",'' );?>
			<div id="div_distribuicao" >
				
			
			</div>
		
			
		</div>
		<div id="debug"></div>
	</body>
	<script type="text/javascript">

	jQuery.noConflict();

	jQuery(function(){

		jQuery('.campoSoma').live('keyup', function(){
			var arCampo = jQuery(this).attr('name').split('_');
			var preid = arCampo[1];
			atualizaValoresTotais( preid );
		});
		
		jQuery('.campoSoma').live('blur', function(){
			var arCampo = jQuery(this).attr('name').split('_');
			var preid 	= arCampo[1];  
	        var empid  	= jQuery('[name="radioempenhoatual"]:checked').val();

			var valorobra 			= jQuery('[name="valorobra_'+preid+'"]').val();
			var valorempenhoobra 	= jQuery('[name="valorempenhoobra_'+preid+'"]').val();
			var vrldistibuido		= replaceAll(replaceAll(jQuery(this).val(), '.', ''), ',', '.');
			var vrlRestante			= jQuery('#vrlempenhoatual_'+empid).val();
			
			if( parseFloat(vrldistibuido) > parseFloat(vrlRestante) ){
				if( parseFloat(vrlRestante) < 0 ){
					alert('O valor empenhado está maior que o valor disponível para a obra');
					jQuery('[name="input_'+preid+'"]').val('');
					jQuery('[name="input_'+preid+'"]').attr('disabled',true);
					jQuery('[name="check_'+preid+'"]').attr('checked',false);
				} else {
					alert('O valor informado está maior que o valor disponível para empenho na obra');
					jQuery(this).val(number_format(vrlRestante, 2, ',', '.'));
				}
				atualizaValoresTotais( preid );
			}	
		});	
	});

	function atualizaValoresTotais( preid ){
		atualizaTotal('campoSoma', 'totaldistribuido', 'campo');
		
		var empidDistribuir  = jQuery('[name="radioempenhoatual"]:checked').val();
		var vrlEmpenhoAtual  = jQuery('#vrlempenhoatual_'+empidDistribuir).val();
		var totaldistribuido = replaceAll(replaceAll( jQuery('[name="totaldistribuido"]').val(), '.', ''), ',', '.');
		
		if( parseFloat(totaldistribuido) > parseFloat(vrlEmpenhoAtual) ){
		
			alert('O valor total informado está maior que o valor disponível para empenho na obra');
			jQuery('[name="input_'+preid+'"]').val(number_format('0', 2, ',', '.'));
			atualizaTotal('campoSoma', 'totaldistribuido', 'campo');
		}
		
		var total = parseFloat(vrlEmpenhoAtual) - parseFloat(replaceAll(replaceAll( jQuery('[name="totaldistribuido"]').val(), '.', ''), ',', '.'));
		jQuery('[name="totalrestante"]').val( number_format(total, 2, ',', '.') );
	}


	function habilitaDistribuicao( preid ){
		
		if( jQuery('[name="radioempenhoatual"]:checked').length == 0 ){
			alert('Selecione o número do empenho a distribuir');
			jQuery('[name="check_'+preid+'"]').attr('checked', false);
			return false;
		}
		if(jQuery('[name="check_'+preid+'"]:checked').length > 0){
			jQuery('[name="input_'+preid+'"]').attr('disabled',false);
			jQuery('[name="input_'+preid+'"]').focus();
		} else	{
			jQuery('[name="input_'+preid+'"]').val('');
			jQuery('[name="input_'+preid+'"]').attr('disabled',true);
			atualizaValoresTotais( preid );
		}
	}
		function salvarDistribuicao( preid )
		{
			
			var inputs = '';
			var checks = '';
			var boolcheck = false;
			jQuery.each(jQuery( ":input" ),function( i, val ) {
					
				  var idinput = this.id;
					
				  if( idinput.indexOf("input_") == 0 )
				  {
					  inputs = inputs + this.id +'='+ this.value + '|';
				  }
				  if( idinput.indexOf("check_") == 0 )
				  {
					  
					  if(jQuery('#'+idinput+':checked').length ) 
					  {
						   valCheck = true;
						   boolcheck = true;
					  }
					  else
					  {
						  valCheck = false;
					  }
						  
					  checks = checks + this.id +'='+ valCheck + '|';
				  }
			});
			var totalDistribuir = jQuery('#total_distribuir').val();
			var tipo = jQuery('#tipo_distribuir').val();
			var preidDistribuir = jQuery('#preid_distribuir').val();
			var empidDistribuir = jQuery('#radioempenhoatual').val();
			
			if( jQuery('[name="radioempenhoatual"]:checked').length == 0 )
			{
				alert('Selecione o número do empenho a distribuir');
				return false;
			}
			if(!boolcheck)
			{
				alert('Selecione pelo menos uma obra para distribuir o empenho');
				return false;
			}

			var totaldistribuido = replaceAll(replaceAll( jQuery('[name="totaldistribuido"]').val(), '.', ''), ',', '.');
			
			
			jQuery.ajax({
	    		type: "POST",
	    		url: window.location.href,
	    		data: "requisicao=salvarRedistribuirEmpenho&arraycheck="+checks+ "&arrayinputs="+inputs+"&total="+totalDistribuir+"&empidDistribuir="+empidDistribuir+"&tipo="+tipo+"&preidDistribuir="+preidDistribuir+'&totaldistribuido='+totaldistribuido+'&'+jQuery('#formdistribuir').serialize(),
	    		async: false,
	    		success: function(msg){
	    				//jQuery('#debug').html(msg);
	    				var arrResposta = msg.split("|");
	    				var status = arrResposta[0];
	    				var mensagem = arrResposta[1]; 
						if( status == 'ERRO')
						{
						  	alert(mensagem);
						}
						else
						{
							alert('Operação realizada com sucesso!');
							jQuery( '#div_redistribuir' ).dialog( 'close' );
		        			window.location.reload();
						}
	    				
		    			
		    		}
		    	});
			
		}
	
		function habilitaDistribuicao( preid ){
			
			if( jQuery('[name="radioempenhoatual"]:checked').length == 0 ){
				alert('Selecione o número do empenho a distribuir');
				jQuery('[name="check_'+preid+'"]').attr('checked', false);
				return false;
			}
			if(jQuery('[name="check_'+preid+'"]:checked').length > 0){
				jQuery('[name="input_'+preid+'"]').attr('disabled',false);
				jQuery('[name="input_'+preid+'"]').focus();
			} else	{
				jQuery('[name="input_'+preid+'"]').val('');
				jQuery('[name="input_'+preid+'"]').attr('disabled',true);
				atualizaValoresTotais( preid );
			}
		}
                
                
                function excluirNotaEmpenho(empid, preid){
                    var tipo = <?php echo $_REQUEST['tipo'] ?>;
                    if(confirm('Tem certeza que deseja excluir essa nota de empenho da obra?')){
                        jQuery.ajax({
                        type: "POST",
                        url: window.location.href,
                        data: "requisicao=excluirNotaEmpenho&empid="+empid+"&preid="+preid+"&tipo="+tipo,
                        async: false,
                        success: function(msg){
                        	//jQuery('#debug').html(msg);
                            if(msg == '1'){
                                alert('Operação realizada com sucesso!');
                                window.location.reload();
                            } else {
                                alert('Falha na Operação!');
                            }
                        }});

                    }
                }

		
                function redistribuirEmpenho( preid , tipo, proid) {

                	jQuery.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: "requisicao=CarregaModalredistribuirEmpenhoObra&preid="+preid+ "&tipo=" + tipo + "&proid=" + proid,
                    async: false,
                    success: function(msg){
                			if(msg != 'erro')
                			{
                					jQuery( "#div_obra_distribuicao" ).html(msg);
                					
                					jQuery.ajax({
                		    		type: "POST",
                		    		url: window.location.href,
                		    		data: "requisicao=CarregaModalredistribuirEmpenho&preid="+preid+ "&tipo=" + tipo + "&proid=" + proid,
                		    		async: false,
                		    		success: function(msg){
                							if(msg != 'erro'){
                							  	jQuery( "#div_distribuicao" ).html(msg);
                							  	jQuery( "#div_redistribuir" ).show();
                							  	jQuery( '#div_redistribuir' ).dialog({
                				    				resizable: true,
                				    				minWidth: 1000,
                				    				modal: true,
                				    				show: { effect: 'drop', direction: "up" },
                				    				buttons: {
                				    					'Salvar': function() {
                				    						salvarDistribuicao();
                				    					},
                				    					'Fechar': function() {
                				    						jQuery( this ).dialog( 'close' );
                				    					}
                				    				}
                				    			});
                								
                							}
                							else
                							{
                								alert('erro');
                							}
                		    				
                			    			
                			    		}
                			    	}); 
                			}
                			else
                			{
                				alert('erro');
                			}
                    		
                	  		}
                	  	});
                	jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
                   }
	
		function salvar(){
			//var check = jQuery('[name="codigo[]"]:checked').length;
			
			//if(check > 0 ){
				if( confirm( "Tem certeza que deseja adicionar/remover a(s) subação(ões)?" ) ){
					jQuery('#requisicao').val('salvar');
					jQuery('#formulario').submit();
				}
			/*} else {
				alert('Selecione pelo menos uma ação!');
				return false;
			}*/
		}
		
		function reforcoSub(sbdid){
			if( confirm( "Tem certeza que deseja reforçar essa subação?" ) ){
				jQuery('#sbdreforco').val(sbdid);
				jQuery('#requisicaoReforco').val('adicionaReforco');
				jQuery('#formulario').submit();
			}
		}
		function desvinculaObra(preid, proid, tipo){
			
		    if(confirm('Tem certeza desvincular a obra do processo?'))
		    {
			    jQuery.ajax({
			    	type: "POST",
			        url: window.location.href,
			        data: "requisicaoAjax=desvinculaObra&preid="+preid+"&tipo="+tipo+"&proid="+proid,
			        async: false,
			        success: function(msg){
			        	if(msg == '1'){
			            	alert('Operação realizada com sucesso!');
			                window.location.reload();
			            } else {
			            	alert('Falha na Operação!');
			            }
				}});

		    }
		}
	</script>
</html>
<?

function sqlSubacoesPAR($codigo){
	$sql = "-- Subações não vinculadas
		(SELECT
			'<input type=\"checkbox\" name=\"codigo[]\" value=\" ' || sd.sbdid || ' \">' as acao,
			d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem || ' - ' || s.sbadsc as descricao,
			sd.sbdano as ano
		FROM
			par.subacao s
		INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid AND sd.ssuid IN (2,12)
		INNER JOIN workflow.documento 	doc ON doc.docid = s.docid
		INNER JOIN par.acao 			a ON a.aciid = s.aciid AND a.acistatus = 'A'
		INNER JOIN par.pontuacao 		p ON p.ptoid = a.ptoid AND p.inuid = {$_SESSION['par']['inuid']}
		inner join par.criterio 		c on c.crtid = p.crtid
		inner join par.indicador 		i on i.indid = c.indid
		inner join par.area 			are on are.areid = i.areid
		inner join par.dimensao 		d on d.dimid = are.dimid
		WHERE
			sd.sbdid NOT IN ( SELECT sbdid FROM par.processoparcomposicao where ppcstatus = 'A' ) AND
			doc.esdid IN (".WF_SUBACAO_ANALISE.",".WF_SUBACAO_ANALISE_COMISSAO.") AND
			s.sbastatus = 'A'
			AND sd.ssuid IN ( 2, 12  )
			AND s.frmid IN ( 13, 6 )
			and sd.sbdptres is not null
			and sd.sbdplanointerno is not null
		ORDER BY
			d.dimcod, 
			are.arecod, 
			i.indcod, 
			sbaordem,
			sd.sbdano)
		
		UNION ALL
		
		-- Subações que já estão no processoparcomposicao
		(SELECT
			'<input type=\"checkbox\" name=\"codigo[]\" checked value=\" ' || sd.sbdid || ' \">' as acao,
			d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem || ' - ' || s.sbadsc as descricao,
			sd.sbdano as ano
		FROM
			par.subacao s
		INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid
		INNER JOIN workflow.documento 	doc ON doc.docid = s.docid
		INNER JOIN par.acao 			a ON a.aciid = s.aciid AND a.acistatus = 'A'
		INNER JOIN par.pontuacao 		p ON p.ptoid = a.ptoid AND p.inuid = {$_SESSION['par']['inuid']}
		inner join par.criterio 		c on c.crtid = p.crtid
		inner join par.indicador 		i on i.indid = c.indid
		inner join par.area 			are on are.areid = i.areid
		inner join par.dimensao 		d on d.dimid = are.dimid
		WHERE
			sd.sbdid IN ( SELECT sbdid FROM par.processoparcomposicao ppc WHERE ppc.prpid = {$codigo} and ppc.ppcstatus = 'A' ) AND
			sd.sbdid NOT IN ( SELECT sbdid FROM par.subacaodetalhe sd2 INNER JOIN par.empenhosubacao ems2 ON ems2.sbaid = sd2.sbaid AND ems2.eobano = sd2.sbdano and eobstatus = 'A'
									inner join par.empenho e on e.empid = ems2.empid and empstatus = 'A' ) AND
			s.sbastatus = 'A'
		ORDER BY
			d.dimcod, 
			are.arecod, 
			i.indcod, 
			sbaordem,
			sd.sbdano)";
	return $sql;
}

function sqlSubacaoEmpenhoPAR($codigo){
	$sql = "SELECT DISTINCT
				CASE WHEN sbdreforco = TRUE THEN '' ELSE '<input type=\"button\" name=\"reforco\" id=\"reforco\" value=\"Reforço\" onclick=reforcoSub(\'' || sd.sbdid || '\')>' END as acao,
				d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem || ' - ' || s.sbadsc as descricao,
				eobano as ano
			FROM
				par.empenhosubacao ems
			INNER JOIN par.empenho 			emp ON emp.empid = ems.empid  and eobstatus = 'A' and empstatus = 'A'
			INNER JOIN par.processopar		prp	ON prp.prpnumeroprocesso = emp.empnumeroprocesso
			INNER JOIN par.subacao 			s 	ON s.sbaid = ems.sbaid
			INNER JOIN par.subacaodetalhe   sd  ON sd.sbaid = ems.sbaid AND sd.sbdano = ems.eobano
			INNER JOIN par.acao 			a 	ON a.aciid = s.aciid AND a.acistatus = 'A'
			INNER JOIN par.pontuacao 		p 	ON p.ptoid = a.ptoid AND p.inuid = {$_SESSION['par']['inuid']}
			INNER JOIN par.criterio 		c 	ON c.crtid = p.crtid
			INNER JOIN par.indicador 		i 	ON i.indid = c.indid
			INNER JOIN par.area 			are ON are.areid = i.areid
			INNER JOIN par.dimensao 		d 	ON d.dimid = are.dimid
			WHERE
				prp.prpid = {$codigo}
			ORDER BY
				descricao,
				eobano";
	return $sql;
}

function sqlObrasPAR($codigo){
	
	$sql = "select 
				acao,
			    preid || ' - ' || descricao as descricao,
			    ano
			from(
			SELECT DISTINCT
			    '<input type=\"checkbox\" name=\"codigo[]\" value=\" ' || pe.preid || ' \">' as acao,
			    pe.preid,
			    pe.predescricao as descricao,
			    pe.preano||' ' as ano 
			FROM obras.preobra	pe 
				INNER JOIN par.subacaoobra so ON so.preid = pe.preid
				INNER JOIN par.subacao s ON s.sbaid = so.sbaid AND s.sbastatus = 'A'
				INNER JOIN par.acao a ON a.aciid = s.aciid
				INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.inuid = {$_SESSION['par']['inuid']}
				INNER JOIN workflow.documento doc ON doc.docid = pe.docid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pe.ptoid AND pto.ptostatus = 'A'
			WHERE			    
			    s.frmid IN ( SELECT CASE WHEN sisid = 57 THEN 15 ELSE 11 END as frmid FROM par.processoobraspar WHERE prostatus = 'A' and proid = $codigo ) 
			    AND esd.esdid=".WF_PAR_OBRA_APROVADA."
			    AND pto.tooid in (11, 2)
			    and pe.prestatus = 'A'
			    AND pe.preid not in (SELECT preid FROM par.processoobrasparcomposicao )
			    and pe.prevalorobra is not null
			    -- verificando se a obra não foi empenhado - solicitado pelo Daniel Areas -- 
			    and (select count(*) from par.empenhoobrapar where preid=pe.preid and eobstatus = 'A')=0
			union all                  
			SELECT DISTINCT
			    '<input type=\"checkbox\" name=\"codigo[]\" checked value=\" ' || pe.preid || ' \">' as acao,
			    pe.preid,
			    pe.predescricao as descricao,
			    pe.preano||' ' as ano 
			FROM obras.preobra	pe 
				inner join par.subacaoobra so on so.preid = pe.preid
				INNER JOIN workflow.documento doc ON doc.docid = pe.docid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
                 pe.prestatus = 'A'
			    AND pe.preid in (SELECT preid FROM par.processoobrasparcomposicao where proid = $codigo)
                    and pe.preid not in (SELECT sd2.preid FROM par.subacaoobra sd2 INNER JOIN par.empenhoobrapar ems2 ON ems2.preid = sd2.preid and eobstatus = 'A'
										inner join par.empenho e on e.empid = ems2.empid and empstatus = 'A')
			) as foo
			order by
				descricao";
	return $sql;
}




function sqlObrasEmpenhoPAR($codigo){
	$sql = "SELECT DISTINCT
				CASE WHEN pre.preid not in ( 
					select distinct preid from par.termocomposicao  t
					INNER JOIN par.vm_documentopar_ativos d on d.dopid = t.dopid
					where preid is not null
				) and coalesce(v.saldo, 0) = 0 THEN					
					'<img align=\"absmiddle\" onclick=\"desvinculaObra(' || pre.preid || ', {$codigo}, \'OBRAS\' )\" title=\"Desvincular obra do processo\" style=\"cursor:pointer;\" src=\"../imagens/excluir.gif\">' 
				ELSE
					'<img align=\"absmiddle\" id=\'' || pre.preid  || '\' title=\"Esta obra já está vinculada a um termo, não pode ser desvinculada do processo\" style=\"width:15px; height:15px; cursor:pointer;\" src=\"../imagens/exclamacao_checklist_vermelho.png\">&nbsp;'
				END ||'&nbsp;'||
				'<img align=\"absmiddle\" onclick=\"redistribuirEmpenho( ' || pre.preid || ' , \'PAR\', '|| pro.proid ||' );\" title=\"Redistribuir Empenho\" style=\"cursor:pointer;\" src=\"../imagens/money_cancel.gif\">' as  acao,
				pre.preid,
				pre.obrid,
			    pre.preid || ' - ' || pre.predescricao as descricao,
			    pre.preano as ano
			FROM
			    par.empenhoobrapar ems
				INNER JOIN par.empenho 				emp ON emp.empid = ems.empid  and eobstatus = 'A' and empstatus = 'A'
				INNER JOIN par.processoobraspar		pro	ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
				inner join obras.preobra 			pre on pre.preid = ems.preid
				inner join par.vm_saldo_empenho_por_obra v on v.preid = pre.preid
			WHERE
				pro.proid = $codigo";
// 	ver($sql);
	return $sql;
}

function sqlObrasPAC($codigo){
	
	global $db;
	
	$estuf	= $_SESSION['par']['estuf'];
	$muncod	= $_SESSION['par']['muncod'];
	
	if( $muncod ){
		$filtro = "AND muncodpar = '$muncod'
				   AND pto.ptoesfera in ('M','T')";
				   
	} else if( $estuf ){
		$filtro = "AND pe.estufpar = '$estuf' 
					AND (pe.muncodpar IS NULL or pe.muncodpar = '')
					AND pto.ptoesfera in ('E','T')";
	}
	
	if($estuf == 'DF' || $muncod == '5300108' ){
		$filtro = "AND pe.estufpar = '$estuf'
				   AND pto.ptoesfera in ('M','T')";
	}
	
	/*$dados = $db->pegaLinha("SELECT itrid, CASE WHEN itrid = 1 THEN estuf ELSE muncod END as local FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par']['inuid']);

	if($dados['itrid'] == 1){
		$local = " and estuf = '".$dados['local']."' and preesfera = 'E' ";
	} else {
		$local = " and muncod = '".$dados['local']."' and preesfera = 'M' ";
	}
	
	if( $_SESSION['par']['inuid'] == 1 ){ //DF
		$local = " and estuf = 'DF' and preesfera = 'E' ";
	}*/
	//$local = $dados['itrid'] == 1 ? " and estuf = '".$dados['local']."' and preesfera = 'E' " : " and muncod = '".$dados['local']."' and preesfera = 'M' ";  
	
	$sql = "select 
				acao,
			    preid || ' - ' || descricao as descricao,
			    ano
			from(
			SELECT DISTINCT
			    '<input type=\"checkbox\" name=\"codigo[]\" value=\" ' || pe.preid || ' \">' as acao,
			    pe.preid,
			    pe.predescricao as descricao,
			    pe.preano||' ' as ano
			FROM obras.preobra	pe 
				INNER JOIN workflow.documento doc ON doc.docid = pe.docid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
				INNER JOIN obras.pretipoobra pto ON pe.ptoid = pto.ptoid
			WHERE
				esd.esdid in ( ".WF_TIPO_OBRA_APROVADA.",".WF_TIPO_OBRA_APROVACAO_CONDICIONAL." )
				and pe.prestatus = 'A'
				AND pto.tooid in (1)
				AND pto.ptoclassificacaoobra IN ('Q', 'C')
				AND ptostatus = 'A'
				AND pe.preid not in (SELECT preid FROM par.processoobraspaccomposicao where pocstatus = 'A' )
				$filtro
			
			
                /*esd.esdid = ".WF_TIPO_OBRA_APROVADA."
                and pe.prestatus = 'A'
			    and pe.preid not in (SELECT preid FROM par.processoobraspaccomposicao where pocstatus = 'A' )
			    and pe.prevalorobra is not null
                and pe.tooid = 1
                {$local} 
			    -- verificando se a obra não foi empenhado - solicitado pelo Daniel Areas -- 
			    and (select count(*) from par.empenhoobra where preid=pe.preid and eobstatus = 'A')=0*/
			union all                  
			SELECT DISTINCT
				'<input type=\"checkbox\" checked name=\"codigo[]\" value=\" ' || pe.preid || ' \">' as acao,
				pe.preid,
			    pe.predescricao as descricao,
			    pe.preano||' ' as ano 
			FROM obras.preobra	pe 
				INNER JOIN workflow.documento doc ON doc.docid = pe.docid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
            	pe.prestatus = 'A'
            	and pe.tooid = 1
			    and pe.preid in (SELECT preid FROM par.processoobraspaccomposicao where pocstatus = 'A' and proid = $codigo)
                and pe.preid not in (SELECT distinct ems2.preid FROM par.empenhoobra ems2
											inner join par.empenho e on e.empid = ems2.empid  and eobstatus = 'A' and empstatus = 'A'
                                            inner join obras.preobra pre1 on pre1.preid = ems2.preid and pre1.tooid = 1)
			) as foo
			order by
				descricao";
//ver($sql);
	return $sql;
}

function sqlObrasEmpenhoPAC($codigo){
	$sql = "SELECT DISTINCT
				CASE WHEN pre.preid not in (
					select distinct preid from par.termoobra o
					INNER JOIN par.termocompromissopac t on t.terid = o.terid and terstatus = 'A'
				) and coalesce(v.saldo, 0) = 0 THEN					
					'<img align=\"absmiddle\" onclick=\"desvinculaObra(' || pre.preid || ', {$codigo}, \'PAC\' )\" title=\"Desvincular obra do processo\" style=\"cursor:pointer;\" src=\"../imagens/excluir.gif\">' 
				ELSE
					'<img align=\"absmiddle\" id=\'' || pre.preid  || '\' title=\"Esta obra já está vinculada a um termo, não pode ser desvinculada do processo\" style=\"width:15px; height:15px; cursor:pointer;\" src=\"../imagens/exclamacao_checklist_vermelho.png\">&nbsp;'
				END ||'&nbsp;'||
				'<img align=\"absmiddle\" onclick=\"redistribuirEmpenho(' || pre.preid || ' , \'PAC\', '|| pro.proid ||');\" title=\"Redistribuir Empenho\" style=\"cursor:pointer;\" src=\"../imagens/money_cancel.gif\">' as  acao,
			    pre.preid,
				pre.obrid,
			    pre.predescricao as descricao,
			    pre.preano as ano
			FROM
			    par.empenhoobra ems
				INNER JOIN par.empenho 				emp ON emp.empid = ems.empid  and eobstatus = 'A' and empstatus = 'A'
				INNER JOIN par.processoobra			pro	ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
				inner join obras.preobra 			pre on pre.preid = ems.preid and pre.tooid = 1
				inner join par.vm_saldo_empenho_por_obra v on v.preid = pre.preid
			WHERE
				pro.proid = $codigo";
	return $sql;
}
?>