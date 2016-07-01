<?php
/*
if($_REQUEST['carregarSubAcoes']){
	header('content-type: text/html; charset=ISO-8859-1');
	carregarSubAcoes_Ajax($_POST['acaid']);
	exit;
}
if($_REQUEST['gravarCredito']){
	header('content-type: text/html; charset=ISO-8859-1');
	gravarCreditoDisponivel($_POST);
	exit;
}
*/
if($_REQUEST['carregavalorcdo']){
	header('content-type: text/html; charset=ISO-8859-1');
	carregaValorCDO($_POST['gnd'], $_POST['acaid'] );
	exit;
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );

$sqlCombo = "
	SELECT
		ac.acaid as codigo,
		ac.prgcod || '.' || ac.acacod || '.' || ac.unicod || '.' || ac.loccod ||' - '|| ac.acadsc as descricao
	FROM monitora.pi_subacao sa
		inner join monitora.pi_subacaodotacao sad ON sad.sbaid = sa.sbaid
		inner join monitora.ptres p ON p.ptrid = sad.ptrid
		inner join monitora.acao ac on ac.acaid = p.acaid
		inner join monitora.pi_subacaounidade sau ON sau.sbaid = sa.sbaid
	WHERE ungcod is not null and ac.acastatus = 'A' and ac.unicod = '26101'
	GROUP BY ac.acaid, ac.acadsc, ac.acacod, ac.prgcod, ac.unicod, ac.loccod
	ORDER BY ac.acadsc";
//dbg($sqlCombo,1);
$acaid = $_REQUEST['acaid'];
?>
<form method='post' name='formulario' id="formulario">

<input type="hidden" name="requisicao" id="requisicao" value="">

<input type="hidden" name="addLinha" id="addLinha" value="<?=$_REQUEST['addLinha']?>">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Selecione a Ação:</td>
		<td><?=$db->monta_combo('acaid', $sqlCombo, 'S','-- Selecione --','', '', '',500,'N','acaid', true, '', 'Ações'); ?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td></td>
		<td>
			<input type="button" value="Pesquisar" name="btnPesquisa" id="btnPesquisa" style="cursor: pointer;" onclick="pesquisarAcao();">
		</td>
	</tr>
</table>

<?
if( $_REQUEST['acaid'] ){
	$sql = "
		SELECT
            (select prgcod || '.' || acacod || '.' || unicod || '.' || loccod from monitora.acao where acacod = ac.acacod limit 1) || ' - ' || ac.acadsc as acao,
            lc.ungcod,
            lc.foncod,
            lc.gndcod,
            lc.ldivalorug,
            lc.ldivalorcontigenciado,
            ac.acaid
		FROM  monitora.acao ac 
		inner join monitora.ptres pt ON pt.acaid = ac.acaid
		inner join elabrev.liberacaocreditodisp lc on lc.acaid = pt.acaid
		WHERE 
	            ac.acastatus = 'A' 
	            and ac.acaid = '{$_REQUEST['acaid']}' 
	            and acasnrap = false 
	            and ac.prgano = '{$_SESSION['exercicio']}' 
		GROUP BY ac.acaid, ac.acadsc, ac.acacod, lc.ungcod, lc.foncod, lc.gndcod, lc.ldivalorug, lc.ldivalorcontigenciado
		ORDER BY ac.acadsc";
	//dbg($sql,1);
	$dados = $db->carregar($sql);
	 
	$totalLinha = $_REQUEST['addLinha'] ? $_REQUEST['addLinha']+1 : ($dados==0 ? 1 : count($dados));  
	
	monta_titulo('Gerenciar Crédito Disponivel', '' );
	echo '<table width="95%" align="center"><tr><td>
	 		<a href="javascript:addLinha();"><img src="../imagens/gif_inclui.gif"> Adicionar</a>
	 	</td></tr></table>';
	//$cabecalho = array("Ação","UG","Fonte","GND","Dotação da UG (A)","Valor Contingenciado (B)","Crédito Disponível (C) = (A) - (B)","CDO (D) = Será carregado com o total informado nas CDOs","A Utilizar (E) = (C) - (D)");
	//$db->monta_lista($sql, $cabecalho, 150, 5, 'N', 'center', '', 'formulario');
	
	
	//if($dados){
		$d = $dados;
		?>
		<table id="tabela_gerencia" width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
			<tr>
				<!-- <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>  -->  
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>UG</strong></td>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte</strong></td>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>GND</strong></td>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Dotação da UG (A)</strong></td>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Contingenciado (B)</strong></td>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Crédito Disponível (C) = (A) - (B)</strong></td>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>CDO (D) = Será carregado com o total informado nas CDOs</strong></td>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>A Utilizar (E) = (C) - (D)</strong></td>
			</tr>	
			</thead>
			<thbody>		
			<?
			
			
			//foreach($dados as $d){
			for($i=0; $i<$totalLinha; $i++){
				
				
				//recupera valores
				$ungcod = $_POST['ungcod'][$i] ? $_POST['ungcod'][$i] : $d[$i]['ungcod'];
				$foncod = $_POST['foncod'][$i] ? $_POST['foncod'][$i] : $d[$i]['foncod'];
				$gndcod = $_POST['gndcod'][$i] ? $_POST['gndcod'][$i] : $d[$i]['gndcod'];
				
				$ldivalorug = $_POST['ldivalorug'][$i] ? $_POST['ldivalorug'][$i] : $d[$i]['ldivalorug'];
				if(!$ldivalorug) $ldivalorug = 0;
				
				$ldivalorcontigenciado = $_POST['ldivalorcontigenciado'][$i] ? $_POST['ldivalorcontigenciado'][$i] : $d[$i]['ldivalorcontigenciado'];
				if(!$ldivalorcontigenciado) $ldivalorcontigenciado = 0;
				
				
				
				$vlcreditodisponivel = $ldivalorug - $ldivalorcontigenciado;
				
				//buscar valor de uma query
				$valorcdo = "0.00";
				
				$vlautilizar = $vlcreditodisponivel - $valorcdo;
				
				//formata valores
				if($vlcreditodisponivel) $vlcreditodisponivel = number_format($vlcreditodisponivel,2,",",".");
				if($valorcdo) $valorcdo = number_format($valorcdo,2,",",".");
				if($vlautilizar) $vlautilizar = number_format($vlautilizar,2,",",".");
				
				/*
				 * Dotação da UG (A)	
				 * Valor Contingenciado (B)	
				 * Crédito Disponível (C) = (A) - (B)	
				 * CDO (D) = Será carregado com o total informado nas CDOs	
				 * A Utilizar (E) = (C) - (D)
				 */
				
				?>
				<tr>
					<!-- 
					<td>
						<?//=$d[0]['acao'];?>
					</td>
					 -->
					<td align="center">
						<?
							$sql = "SELECT 
											ungcod as codigo, 
											ungdsc as descricao
										 FROM public.unidadegestora
										 order by 2";
							echo $db->monta_combo('ungcod[]', $sql, 'S','-- Selecione --','', '', '',150,'N','ungcod', true, $ungcod, 'UG');
						?>
					</td>
					<td align="center">
						<?
							$sql = "SELECT 
											foncod as codigo, 
											foncod||' - '||fondsc as descricao
										 FROM public.fonterecurso
										 order by 2";
							echo $db->monta_combo('foncod[]', $sql, 'S','-- Selecione --','', '', '',150,'N','foncod', true, $foncod, 'Fonte');
						?>
					</td>
					<td align="center">
						<?
							$sql = "SELECT 
											gndcod as codigo, 
										    gndcod||' - '||gnddsc as descricao 
										FROM public.gnd
										WHERE gndstatus = 'A'";
							$arrGND = $db->carregar($sql);
							//echo $db->monta_combo('gndcod['.$d[0]['acacod'].']', $sql, 'S','-- Selecione --','carregaValorCDO', '', '',150,'N','gndcod'.$i, true, $d['gndcod'], 'GND');
							
						?>
						<select  name="gndcod[]"  class='CampoEstilo' title='GND' style='width:200px;' onchange="carregaValorCDO(this.value, '<?=$i?>')" id="gndcod_<?=$i?>">
							<option value=''>-- Selecione --</option>
								<?foreach ($arrGND as $v) {?>
									<option value="<?=$v['codigo']?>" <?if( $v['codigo'] == $gndcod ) echo "selected";?> ><?=$v['descricao']?></option>
								<?}?>
						</select>
					</td>
					<td align="center">
						<?
						/*
						if($d[0]['ldivalorug']){
							$ldivalorugx = number_format($d[0]['ldivalorug'],2,",",".");
						}
						else
							$ldivalorugx = '0,00';
						*/
						
						?>
						<?= campo_texto( 'ldivalorug[]', 'N', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="ldivalorug_'.$i.'"', '', $ldivalorug, "calculaV('".$i."')"); ?>
					</td>
					<td align="center">
						<?= campo_texto( 'ldivalorcontigenciado[]', 'N', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="ldivalorcontigenciado_'.$i.'"', '', $ldivalorcontigenciado, "calculaV('".$i."')"); ?>
					</td>
					<td align="center">
						<?= campo_texto( 'vlcreditodisponivel[]', 'N', 'N', '', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="vlcreditodisponivel_'.$i.'"', '', $vlcreditodisponivel); ?>
					</td>
					<td align="center">
						<?= campo_texto( 'valorcdo[]', 'N', 'N', '', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="valorcdo_'.$i.'"', '', $valorcdo); ?>
					</td>
					<td align="center">
						<?= campo_texto( 'vlautilizar[]', 'N', 'N', '', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="vlautilizar_'.$i.'"', '', $vlautilizar); ?>
					</td>

				</tr>
				
				<?
			}
		//}
		?>
			</thbody>
		</table>
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr bgcolor="#D0D0D0">
				<td align="center">
					<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="gravarCredito();" style="cursor: pointer;">
				</td>
			</tr>
		</table>
		<?
		
		echo "<script></script>";
}


?>
<div id="debug"></div>
</form>

<script language="JavaScript" src="../includes/prototype.js"></script>
<script>
<?//if( $_REQUEST['acacod'] ){ ?>
	//carregaDadosSubacao('imgAcao<?=$_REQUEST['acacod']?>', '<?=$_REQUEST['acacod'] ?>');
<?//} ?>

/*
function carregaDadosSubacao(idImg, acacod){
	
	var img = document.getElementById( idImg );
	
	var tr_nome = 'vtr_'+acacod;
	var td_nome = 'vtd_'+acacod;
	
	if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML == "" ){
		$(td_nome).innerHTML = 'Carregando...';
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
		carregarSubAcoes( acacod, td_nome );
	}else if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != ""){
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
		carregarSubAcoes( acacod, td_nome );
	} else {
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}

function carregarSubAcoes( acacod, td_nome ){
	var myajax = new Ajax.Request('elabrev.php?modulo=principal/gerenciarCredito&acao=A', {
		        method:     'post',
		        parameters: '&carregarSubAcoes=true&acacod='+acacod,
		        asynchronous: false,
		        onComplete: function (res){
					$(td_nome).innerHTML = res.responseText;
		        }
		  });
}

function calculaContigente(sbaid){
	var disponivel 		= $('lbcvalor_'+sbaid).value;
	var valorcdo 		= $('lbcvalorcdo_'+sbaid).value;
	var valorcontigente = '';
	
	if( disponivel && valorcdo ){
		disponivel = disponivel.replace(/\./gi,"");
		disponivel = disponivel.replace(/\,/gi,".");
		
		valorcdo = valorcdo.replace(/\./gi,"");
		valorcdo = valorcdo.replace(/\,/gi,".");
		
		var valorcontigente = parseFloat(disponivel) - parseFloat(valorcdo);
		$('valorcontigente_'+sbaid).value = mascaraglobal('###.###.###.###,##', valorcontigente.toFixed(2));
	} else {
		if( valorcdo == '' ) valorcdo = '0,00';
		$('valorcontigente_'+sbaid).value = mascaraglobal('###.###.###.###,##', valorcdo);
	}
}
*/

function gravarCredito(){

	document.formulario.requisicao.value = 'gravarCreditoDisponivel';
	document.formulario.submit();

	return false;
	
	
		
	var form = $('formulario');
	
	var boVazio = false;
	
	for(i=0; i<form.length; i++){
		if(form.elements[i].type == "text" || form.elements[i].type == "select-one"){
			
			if( form.elements[i].value == '' ){
				boVazio = true;
			}
		}
	}
	
	if( !boVazio ){
		var myajax = new Ajax.Request('elabrev.php?modulo=principal/gerenciarCredito&acao=A', {
			        method:     'post',
			        parameters: '&gravarCredito=true&'+$('formulario').serialize(),
			        asynchronous: false,
			        onComplete: function (res){
			        
			        	if( res.responseText == '1' ){
			        		alert('Operação realizada com sucesso!');
			        		//carregaDadosSubacao('imgAcao<?=$_REQUEST['acacod']?>', '<?=$_REQUEST['acacod'] ?>');
			        	} else {
			        		alert('Falha na operação!');
			        	}
						//$('debug').innerHTML = res.responseText;
			        }
			  });
	} else {
		alert('Existem campos em branco!');
		return false;
	}
	
}
/*
function carregaValorCDO( gnd , sbaid ){
	var myajax = new Ajax.Request('elabrev.php?modulo=principal/gerenciarCredito&acao=A', {
			        method:     'post',
			        parameters: '&carregavalorcdo=true&gnd='+gnd+'&acacod='+$('acacod').value,
			        asynchronous: false,
			        onComplete: function (res){
						$('lbcvalorcdo_'+sbaid).value = res.responseText;
						calculaContigente(sbaid);
			        }
			  });
}
*/

function carregaValorCDO( gnd, pos ){
	var myajax = new Ajax.Request('elabrev.php?modulo=principal/gerenciarCredito&acao=A', {
			        method:     'post',
			        parameters: '&carregavalorcdo=true&gnd='+gnd+'&acaid='+$('acaid').value,
			        asynchronous: false,
			        onComplete: function (res){
						$('valorcdo_'+pos).value = res.responseText;
						//calculaContigente(sbaid);
						calculaV(pos);
			        }
			  });
}

function calculaV( pos ){

	var ldivalorug 				= $('ldivalorug_'+pos).value;
	var ldivalorcontigenciado		= $('ldivalorcontigenciado_'+pos).value;
	var vlcreditodisponivel		= $('vlcreditodisponivel_'+pos).value;
	var valorcdo 				= $('valorcdo_'+pos).value;
	var vlautilizar 			= 0;//$('vlautilizar_'+pos).value;
	
	if( ldivalorug && ldivalorcontigenciado ){
		ldivalorug = ldivalorug.replace(/\./gi,"");
		ldivalorug = ldivalorug.replace(/\,/gi,".");
		
		ldivalorcontigenciado = ldivalorcontigenciado.replace(/\./gi,"");
		ldivalorcontigenciado = ldivalorcontigenciado.replace(/\,/gi,".");
		if(parseFloat(ldivalorcontigenciado) > parseFloat(ldivalorug)){
			alert('Valor Contingenciado (B) não pode ser maior que o valor Dotação da UG (A).');
			$('ldivalorcontigenciado_'+pos).value = mascaraglobal('###.###.###.###,##', '0,00');
			$('vlcreditodisponivel_'+pos).value = mascaraglobal('###.###.###.###,##', '0,00');
		}
		else{
			var vlcreditodisponivel = parseFloat(ldivalorug) - parseFloat(ldivalorcontigenciado);
			$('vlcreditodisponivel_'+pos).value = mascaraglobal('###.###.###.###,##', vlcreditodisponivel.toFixed(2));
		}
	} else {
		if( ldivalorcontigenciado == '' ) ldivalorcontigenciado = '0,00';

		ldivalorug = ldivalorug.replace(/\./gi,"");
		ldivalorug = ldivalorug.replace(/\,/gi,".");
		
		$('ldivalorcontigenciado_'+pos).value = mascaraglobal('###.###.###.###,##', ldivalorcontigenciado);
		$('vlcreditodisponivel_'+pos).value = mascaraglobal('###.###.###.###,##', ldivalorug);
	}	

	//calcula cdo e a utilizar
	if(valorcdo){
		valorcdo = valorcdo.replace(/\./gi,"");
		valorcdo = valorcdo.replace(/\,/gi,".");

		var vlcreditodisponivel2	= $('vlcreditodisponivel_'+pos).value;
		if(vlcreditodisponivel2){
			vlcreditodisponivel2 = vlcreditodisponivel2.replace(/\./gi,"");
			vlcreditodisponivel2 = vlcreditodisponivel2.replace(/\,/gi,".");
		}
		
		var vlautilizar = parseFloat(vlcreditodisponivel2) - parseFloat(valorcdo);
		$('vlautilizar_'+pos).value = mascaraglobal('###.###.###.###,##', vlautilizar.toFixed(2));
	}
}


function addLinha(){
	document.formulario.addLinha.value="<?=$totalLinha?>";
	document.formulario.requisicao.value = '';
	document.formulario.submit();
}

function pesquisarAcao(){
	document.formulario.addLinha.value="";
	document.formulario.requisicao.value = '';
	document.formulario.submit();
}

</script>