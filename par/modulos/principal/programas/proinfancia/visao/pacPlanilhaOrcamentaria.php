<?php

//$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

$preid = $_REQUEST['preid'];		
$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$oSubacaoControle = new SubacaoControle();

if($preid){
	$arDados = $oSubacaoControle->recuperarPreObra($preid);
}

if(!is_array($perfil)){
	$perfil = Array();
}

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

$reformulaMI = verificaMi( $preid );

$boAtivo = 'N';
$stAtivo = 'disabled="disabled"';
$travaCorrecao = true;
$tipoB = array();

// Manaus = 1302603
// Belo Horizonte = 3106200
// Catalão = 5205109

if( $_SESSION['par']['muncod'] == '1302603' || $_SESSION['par']['muncod'] == '3106200' || $_SESSION['par']['muncod'] == '5205109' ){
	if( possuiPerfil( ( Array(PAR_PERFIL_ENGENHEIRO_FNDE)) ) ){
		$boAtivo = 'S';
		$stAtivo = '';
	}
}

echo "<script>
		jQuery(document).ready(function(){
			jQuery('.enviar').attr('disabled',true); 
		});
	  </script>";

if( $esdid ){
	if( is_array($respSim) ){
		$travaCorrecao = !in_array(QUESTAO_PLANILHA,$respSim);
	}
	
	$obSubacaoControle = new SubacaoControle();
	$obPreObra = new PreObra();
	
	if($preid){
		$arDados = $obSubacaoControle->recuperarPreObra($preid);
	}	
	#Regra passada pelo Daniel - 9/6/11
	#Alteração do código feito em 23/10/2012 mas, mantenodo a mesma "lógica", regra já definida.
	if(in_array(array(PAR_PERFIL_COORDENADOR_GERAL,PAR_PERFIL_ENGENHEIRO_FNDE), $perfil) && $esdid == WF_TIPO_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
		$boAtivo = 'S';
		$stAtivo = '';
		echo "<script>
				jQuery(document).ready(function(){
					jQuery('.enviar').attr('disabled',false); 
				});
			  </script>";
	} else {
		$arrPerfil = array(PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO,PAR_PERFIL_SUPER_USUARIO);
		$tipoB = Array(2,7);
		$tipoA = Array(1);
		if( ($esdid == WF_TIPO_EM_CADASTRAMENTO || ( $esdid == WF_TIPO_EM_CORRECAO && $travaCorrecao ) ) && possuiPerfil($arrPerfil) && !($arDados['pretipofundacao'] == '' &&  in_array($arDados['ptoid'], $tipoB)) ){
			$boAtivo = 'S';
			$stAtivo = '';
			echo "<script>
					jQuery(document).ready(function(){
						jQuery('.enviar').attr('disabled',false); 
					});
				  </script>";
		}
		
		#Foi inserido na data 22/10/2012, para que uma obra quando aprova, independente de perfil e "ptoprojetofnde = false", ficara desabilitada.
		if( ( 	WF_TIPO_OBRA_APROVADA != $esdid	) && (	$arDados['ptoprojetofnde'] == 'f'	) && in_array(array(PAR_PERFIL_COORDENADOR_GERAL,PAR_PERFIL_ENGENHEIRO_FNDE), $perfil) ){
			$boAtivo = 'S';
			$stAtivo = '';
			echo "<script>
					jQuery(document).ready(function(){
						jQuery('.enviar').attr('disabled',false); 
					});
				  </script>";
		}
	}
}

# Código refeito em 22/10/2012. Regra para liberação da tela para (cadastramento e ateração) dos perfil abaixos listados nas seguintes situações também listadas abaixo. 
# Foi inserido os perfis Estaduais e a situação em Diligência.
# Foi também inserido o os perfis. (não havia perfil, era verificado apenas o estado).
if(	(	WF_TIPO_EM_CORRECAO == $esdid || WF_TIPO_EM_CADASTRAMENTO == $esdid || WF_TIPO_EM_REFORMULACAO == $esdid || WF_TIPO_EM_ANALISE_DILIGENCIA == $esdid	) &&
		(	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
			in_array(PAR_PAR_PERFIL_PREFEITO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) ||
			in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
			in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
			in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)
		)
){
	$boAtivo = 'S';
	$stAtivo = '';
}

// nova situação, se tiver mais de 0% de execução da obra... desabilitar
if((float)$arDados['percexec'] > 0) {
	$boAtivo = 'N';
	$stAtivo = 'disabled="disabled"';
	$travaCorrecao = true;
}

$sql = "SELECT 
			pre1.preid as preid1,
			pre2.preid as preid2,
			pre1.ptoid as tipo1,
			pre2.ptoid as tipo2 
		FROM 
			obras.preobra pre1
		LEFT JOIN obras.preobra pre2 ON pre1.preid = pre2.preidpai
		WHERE 
			pre1.preid = ".$preid;

$rsTipoObras = $db->pegaLinha($sql);

if( $esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI || $reformulaMI ){
	if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){
		$boAtivo = 'S';
		$stAtivo = '';
	}
}

//$arrPreids = array( 128240,9642,101481,130313, 9484, 9676 );
$arrPreids = array( 108313,10606,9092,99517,10609,130313,9337,9204,8351,104496,100637,9484,100055,10613,127058,9552,127956,127471,100376,127541,115354,126605,10317,9177,127366,10611,9876,128100,9340,128240,10605,127551,10612,10604,9676,127052,10584,8879,10608,9120,10326,8352,8868,126929,9642,126941,10607,127054,100978,102377,10610,9657,100049,100252,127941,101540,109610,10614,9211,9274 );

$visualizaBotaoAtualizacao = false;
if( $reformulaMI ){
	if($esdid == WF_TIPO_VALIDACAO_DEFERIMENTO || $esdid == WF_TIPO_EM_ANALISE_DILIGENCIA || $esdid == WF_TIPO_EM_ANALISE_FNDE || $esdid == WF_TIPO_OBRA_APROVADA){
		if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_ADM_OBRAS, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)){
			if( $esdid == WF_TIPO_OBRA_APROVADA ){
				$boItemPerdido = $db->pegaUm("SELECT count(com.preid)  
												FROM par.processoobraspaccomposicao com 
												INNER JOIN par.enviomiitemperdido eip ON eip.preid = com.preid 
												WHERE com.pocstatus = 'A' and com.preid = $preid");
				$visualizaBotaoAtualizacao = false;
				if( in_array($preid, $arrPreids) ){
					$boItemPerdido = 1;
				}
				if( (int)$boItemPerdido > 0 && $arDados['terid'] == '' ) $visualizaBotaoAtualizacao = true;
			} else {
				$visualizaBotaoAtualizacao = true;
			}
		}
	}
}

/*
 * Alteração de regra
 * De: Só libera edição de obra em reformulação com tipo da obra diferente do tipo de seu backup
 * Para: Se estiver em reformulação libera para edição.
 * A pedido de Thiago Tasca e feito por Eduardo Dunice.
 * */
$arrReformulacao = Array(WF_TIPO_EM_REFORMULACAO, WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL, WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL);
if( in_array($esdid, $arrReformulacao) ){
// 	if(!empty($rsTipoObras['preid1']) && !empty($rsTipoObras['preid2'])){		
// 		if($rsTipoObras['tipo1'] != $rsTipoObras['tipo2']){								
			$boAtivo = 'S';
			$stAtivo = '';
// 		}		
// 	}
}
/*
 * FIM - Alteração de regra
* De: Só libera edição de obra em reformulação com tipo da obra diferente do tipo de seu backup
* Para: Se estiver em reformulação libera para edição.
* A pedido de Thiago Tasca e feito por Eduardo Dunice.
* */

$perfil_array = array(PAR_PERFIL_COORDENADOR_GERAL,PAR_PERFIL_ENGENHEIRO_FNDE,PAR_PERFIL_ADMINISTRADOR);
if( ( $esdid == WF_TIPO_OBRA_APROVADA || $esdid == WF_TIPO_EM_ANALISE ) && possuiPerfil($perfil_array)	){
	$boAtivo = 'S';
	$stAtivo = '';
}

/*
 * REGRA TEMPORARIA 02/05/2012
 * SOLICITADO PELO DANIEL AREAS
 * LIBERA EDIÇÃO PARA
 * MUNICIPIO COM OBRA
 * EM REFORMULAÇÃO
 * 
 * MUNICIPIOS: SORRISO/MT
 */
//if(in_array($_SESSION['par']['muncod'], array(5107925)) && in_array($esdid, array(WF_TIPO_EM_REFORMULACAO)) ){
//	$boAtivo = 'S';
//	$stAtivo = '';
//}

echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=planilhaOrcamentaria&preid=".$_REQUEST['preid'], $_REQUEST['preid'], $descricaoItem );
monta_titulo( 'Planilha Orçamentária', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
/*
if( $arDados['ptoid'] == 42 || $arDados['ptoid'] == 43 || $arDados['ptoid'] == 44 || $arDados['ptoid'] == 45){
	include_once(APPRAIZ."par/modulos/principal/manutencao.inc");
}
*/

$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];
$_SESSION['par']['preid'] = $preid;

//Obs -> itcvalorunitario não existe mais
//regra-> pegar o campo obras.preplanilhaorcamentaria.ppovalorunitario (valor do item) e multiplicar por obras.preitenscomposicao.itcquantidade (quantidade)

if($_POST['ppovalorunitario'] && $boAtivo == 'S'){
	
	$count = count($_POST['itcid']);
	$obPrePlanilhaOrcamentaria = new PrePlanilhaOrcamentaria();

	for($x=0;$x<$count;$x++){
		
		if($_POST['ppovalorunitario'][$x] != '' && $_POST['ppovalorunitario'][$x] != '0' && $_POST['ppovalorunitario'][$x] != "0,00" && $_POST['ppovalorunitario'][$x] != $_POST['ppovalorunitario_ant'][$x]){
			$arDados['ppoid'] = $_POST['ppoid'][$x];
			$arDados['preid'] = $preid;
			$arDados['itcid'] = $_POST['itcid'][$x];
			
			$arDados['ppovalorunitario'] = str_replace(" ","", $_POST['ppovalorunitario'][$x]);
			$arDados['ppovalorunitario'] = str_replace(".","", $arDados['ppovalorunitario']);
			$arDados['ppovalorunitario'] = str_replace(",",".", $arDados['ppovalorunitario']);
			
			//$arDados['ppovalorunitario'] = trim(str_replace(" ","",str_replace(array(".",""),array(",","."),$_POST['ppovalorunitario'][$x])));
			
			$obPrePlanilhaOrcamentaria->excluiItensPlanilhaOrcamentaria($arDados['preid'],$arDados['itcid'] );
			
			$obPrePlanilhaOrcamentaria->ppoid = null;
			$obPrePlanilhaOrcamentaria->preid = $arDados['preid'];
			$obPrePlanilhaOrcamentaria->itcid = $arDados['itcid'];
			$obPrePlanilhaOrcamentaria->ppovalorunitario = $arDados['ppovalorunitario'];
			$obPrePlanilhaOrcamentaria->salvar();
		}
	}
	
	$obPrePlanilhaOrcamentaria->commit();
	atualizaValorObra( $preid );

	echo '<script type="text/javascript"> 
			alert("Operação realizada com sucesso.");
			document.location.href = \''.$_SERVER['HTTP_REFERER'].'\';
		  </script>';
	exit;
}

if($_POST['itcquantidade'] && $boAtivo == 'S'){

	global $db;
	
	$count = count($_POST['itcid']);
	
	for($x=0;$x<$count;$x++){
		
		if($_POST['itcquantidade'][$x] != '' //&& $_POST['itcquantidade'][$x] != '0' && $_POST['itcquantidade'][$x] != "0,00" 
			&& $_POST['itcquantidade'][$x] != $_POST['itcquantidade_ant'][$x]){
			$arDados['ppoid'] = $_POST['ppoid'][$x];
			$arDados['preid'] = $preid;
			$arDados['itcid'] = $_POST['itcid'][$x];
			//$arDados['itcquantidade'] = str_replace('.','',str_replace(",",".",$_POST['itcquantidade'][$x]));
			$arDados['itcquantidade'] = retiraPontosBD($_POST['itcquantidade'][$x]);
			
			$sqlUp .= "UPDATE obras.preitenscomposicaomi SET itcquantidade = ".$arDados['itcquantidade']." WHERE itcid = ".$arDados['itcid']."; ";
			
		}
	}
	$db->executar( $sqlUp );
	$db->commit();

	atualizaValorObra( $preid, 'mi' );

	echo '<script type="text/javascript"> 
			alert("Operação realizada com sucesso.");
			document.location.href = \''.$_SERVER['HTTP_REFERER'].'\';
		  </script>';
	exit;
}

$arDados = $oSubacaoControle->recuperarPreObra($preid);

$tipoObra = $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
$boPlanilhaOrcamentaria = $oSubacaoControle->verificaCategoriaObra($preid); 

if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ){
	$boAtivo = 'S';
	$stAtivo = '';
	$travaCorrecao = false;
}

if( $arDados['pretipofundacao'] == '' &&  in_array($arDados['ptoid'],$tipoB) ){
?>
	<div id="lista">
	<form action="" method="post" name="formulario" id="formulario">
		<table id="tabela_planilha" class="tabela" bgcolor="#f5f5f5" cellpadding="3" align="center">
			<tr style="background-color: #e0e0e0">
				<td style="font-weight:bold; text-align:center; width:20%;">Descriçao do item</td>
				<td style="font-weight:bold; text-align:center; width:10%;">Valor Unitario</td>			
				<td style="font-weight:bold; text-align:center;">Unidade de Medida</td>
				<td style="font-weight:bold; text-align:center; width:30%;">Quantidade</td>
				<td style="font-weight:bold; text-align:center; width:10%;">Valor</td>
				<td style="font-weight:bold; text-align:right; width:10%;">%</td>			
			</tr>
			<tr>
				<td colspan="6" align="center">
					<label style="color:red">Existem pendencias no cadastro deste programa. Favor corrigir antes de continuar com o preenchimento da planilha.  </label>
				</td>
			</tr>
		</table>
		<table class="tabela" bgcolor="#f5f5f5" cellpadding="3" align="center">
			<tr style="background-color: #e0e0e0">
				<td id="td_tbl_salvar" style="text-align: center;">
				<?php if( $boAtivo == 'S' ){ ?>
					<input type="submit" value="Salvar" />
				<?php } ?>
				<input class="fechar" type="button" value="Fechar" onclick="atualizarObra();" />
				</td>
			</tr>
		</table>
	</form>
	</div>
<?php 	
	die();
}
?>
<?php echo cabecalho();?>
<?php if($boAtivo == 'S' && count($respSim)): ?>
	<?php
	$txtAjuda = "A Planilha Orçamentária disponibilizada no sistema deve ser preenchida com os valores praticados na região para os materiais e serviços predefinidos, atribuindo-se valores para todos os itens listados. Deve-se utilizar como balizamento de preços os valores de referência do SINAPI da Caixa Econômica Federal, acrescidos de BDI.";
	$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">"; 
	?>
	<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
		<tr>
			<td width="100" style="text-align: right;" class="SubTituloDireita">Ajuda:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?php echo $imgAjuda ?>
			</td>
		</tr>
	</table>
<?php endif;  ?>
	<div id="lista">
	<form action="" method="post" name="formulario" id="formulario">
		<table id="tabela_planilha" class="tabela" bgcolor="#f5f5f5" cellpadding="3" align="center">
			<tr style="background-color: #e0e0e0">
				<td style="font-weight:bold; text-align:center; width:20%;">Descriçao do item</td>
				<td style="font-weight:bold; text-align:center; width:10%;">Valor Unitario</td>			
				<td style="font-weight:bold; text-align:center;">Unidade de Medida</td>
				<td style="font-weight:bold; text-align:center; width:30%;">Quantidade</td>
				<td style="font-weight:bold; text-align:center; width:10%;">Valor</td>
				<td style="font-weight:bold; text-align:right; width:10%;">%</td>			
			</tr>
		</table>
		<table class="tabela" bgcolor="#f5f5f5" cellpadding="3" align="center">
			<tr style="background-color: #e0e0e0">
				<td id="td_tbl_salvar" style="text-align: center;">
				<?php if( $boAtivo == 'S' ){ ?>
					<input type="button" class="validaForm" value="Salvar" <?=$stAtivo ?>/>
				<?php } ?>
				<input class="fechar" type="button" value="Fechar" onclick="atualizarObra();" />
				<?php if( $visualizaBotaoAtualizacao ){ ?>
				<input type="button" value="Atualizar Planilha" onclick="atualizarPlanilhaMI();" />
				<?php } ?>
				</td>
			</tr>
		</table>
	</form>
	</div>
<script type="text/javascript">
<!--

jQuery(document).ready(function(){

	//jQuery("#formulario").submit(function(){	
		//$(".itcvalorunitario").keyup();	
		//var ptoid = <?php //echo $tipoObra; ?>;
		//var total = replaceAll(jQuery('#totalValor').html().replace("<strong>","").replace("</strong>",""), '.','').replace(",",".");		
		//var total = replaceAll(replaceAll(replaceAll(jQuery('#totalValor').html(), '.',''), "<strong>",""), "</strong>","").replace(",",".");		
		//if(ptoid == 3){		
		//	if(total < 520000 || total > 620000){
		//		alert('O valor total da obra deve ser de R$ 520.000,00 até R$ 620.000,00');
		//		return false;
		//	}
		//}

		//if(ptoid == 2){		
		//	if(total < 1100000 || total > 1330000){
		//		alert('O valor total da obra deve ser de R$ 1.100.000,00 até R$ 1.330.000,00');
		//		return false;
		//	}
		//}
	//});
	
	jQuery('.validaForm').click(function(){
//		alert(123);
		jQuery('.validaForm').attr('disabled',true);
		jQuery('.validaForm').val('Aguarde...');
		jQuery("#formulario").submit();
	});
	
	var param = 'requisicao=montaArvoreAberta&db=1&ptoid=<?php echo $tipoObra ?>';

	<?php $tipoFundacao = $oSubacaoControle->verificaTipoFundacao($preid); ?>
	<?php if($tipoFundacao && $tipoObra == OBRA_TIPO_B): ?>
		param += "&tipoFundacao=<?php echo $tipoFundacao; ?>";
	<?php endif; ?>
	<?php if($_REQUEST['atualiza']): ?>
		param += "&atualmi=1";
	<?php endif; ?>
	
	var cor = "#f0f0f0";
	var total_valor_unitario = 0;
	var total_valor = 0;
	var arrItens = new Array();
	var num = 0;

	jQuery('#aguarde_').show();	

	jQuery.ajax({
	   type		: "POST",
	   url		: "ajax.php",
	   data		: param,
	   async    : false,
	   dataType: 'json',
	   success	: function(data){	

		var idNivel = new Array();
		var totalValor = 0;
		var totalValorUni = 0;
		var data;
		
		if( data.erro != '' &&  data.erro != undefined ){
			alert(data.erro[0]);
			window.location = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=planilhaOrcamentaria&preid=<?=$_REQUEST['preid'] ?>';
			return false;
		}
		
		if(!data){
			jQuery('#aguarde_').hide();
			<? if($esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI || $reformulaMI){?>
				jQuery('#td_tbl_salvar').html("Pregões não cadastrados para o Estado!");
			<? }else{?>
				jQuery('#td_tbl_salvar').html("Não existem registros!");
			<? }?>
			return false;
		}
		
		jQuery.each(data, function(i,item){
			var itcid 					= item.itcid;
			var itcidpai 				= item.itcidpai;
       		var boFilho 				= item.boFilho;
       		var img 					= item.img;
       		var itccodigoitem			= item.itccodigoitem;
       		var itcdescricao			= item.itcdescricao;
       		var itccodigoitemcodigo		= item.itccodigoitemcodigo;
       		var itcquantidade			= item.itcquantidade;
       		var umdeesc					= item.umdeesc;
       		var ppovalorunitario		= item.ppovalorunitario;
       		var ppoid					= item.ppoid;
       		var ptopreencher			= item.ptopreencher;
			var tamanho     			= itccodigoitemcodigo.length;
			var nivel 	 				= tamanho / 3;
			
		//	alert(item.itccodigoitemcodigo+" - "+item.itcid+" - "+ppovalorunitario);
			
			if(umdeesc == null){
				umdeesc = '-';
			}
			total_valor_unitario = ( (total_valor_unitario * 1) + (ppovalorunitario * 1) );
			total_valor = ( (total_valor * 1) + (ppovalorunitario * itcquantidade) );

			if(itcid){
				arrItens[num] = new Object();
				arrItens[num].itcid = itcid;
				arrItens[num].valor_unitario = ppovalorunitario;
				arrItens[num].valor = (ppovalorunitario * itcquantidade);
				num++;
			}

			idNivel[nivel] = itcid;
			var id = '';
			// prepara para forma o id das TR
			for (i=1; i <= nivel; i++){
				id += (i == 1 ? idNivel[i] : '_' + idNivel[i]);
			}

			// Identação
       		var espaco     = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
       		var espacoTemp = "";

			if(itcidpai){
	       		for (y = 1; y < nivel; y++) {
	            	espacoTemp = espacoTemp + espaco;
	            }
			}
            
            var seta = "";
            if(espacoTemp){
            	seta = "<img src=\"../imagens/seta_filho.gif\">";
            }

            if(cor == "#fafafa") {
				cor = "#f0f0f0";
			} else {
				cor = "#fafafa";
			}

			var html = "<tr id=\""+id+"\" style=\"background: "+cor+" \" cor=\""+cor+"\">";
				if( boFilho > 0 ) {
					html += "<td>"+espacoTemp+seta
					+"<a href=\"#\" onclick=\"alteraIcone('"+id+"');\"><img id=\"img_"+id+"\" src=\"../imagens/"+img+"\" border=\"0\"></a> "
					+itccodigoitem+" "+itcdescricao+"</td>";
				} else {
					html += "<td>"+espacoTemp+seta+itccodigoitem+" "
					+itcdescricao+"</td>";
				}
				
			 
			// Valor Unitário = ppovalorunitario
			var stAtivo = '<?=$stAtivo ?>';
			
			// Verifico se são obras MI
			<? if($esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI || $reformulaMI){?>
				if( boFilho < 1 ) {
					//html += "<td>"+parseFloat(ppovalorunitario).toFixed(2).replace(".",",");
					html += "<td>"+mascaraglobal('###.###.###.###,##',parseFloat(ppovalorunitario).toFixed(2));
					html += "<input type=\"hidden\" name=\"ppovalorunitario_ant[]\" value=\""+parseFloat(ppovalorunitario).toFixed(2).replace(".",",")+"\">";
					html += "<input type=\"hidden\" name=\"itcid[]\" value=\""+itcid+"\">";
					html += "<input type=\"hidden\" name=\"ppoid[]\" value=\""+ppoid+"\"></td>";								
				} else {
					html += "<td id=\"valor_unitario_pai_"+itcid+"\"></td>";
				}
			<? } else { ?>
// 				if( boFilho < 1 && (itcquantidade > 0) ) {
				if( trim(umdeesc) != '' ) {
					if( ptopreencher != 't' ){
						html += "<td>"+mascaraglobal('###.###.###.###,##',parseFloat(ppovalorunitario).toFixed(2));
						html += "<input type=\"hidden\" class=\"normal itcvalorunitario\" name=\"ppovalorunitario[]\" value=\""+parseFloat(ppovalorunitario).toFixed(2).replace(".",",")+"\">";
						html += "<input type=\"hidden\" name=\"ppovalorunitario_ant[]\" value=\"\">";
					}else{
// 						html += "<td><input type=\"text\" class=\"normal itcvalorunitario\" id=\"item_"+itcid+"\" "+readonly+" name=\"ppovalorunitario[]\" value=\""+parseFloat(ppovalorunitario).toFixed(2).replace(".",",")+"\">";
						html += "<td><input type=\"text\" class=\"normal itcvalorunitario\" id=\"item_"+itcid+"\" name=\"ppovalorunitario[]\" value=\""+parseFloat(ppovalorunitario).toFixed(2).replace(".",",")+"\">";
						html += "<input type=\"hidden\" name=\"ppovalorunitario_ant[]\" value=\""+parseFloat(ppovalorunitario).toFixed(2).replace(".",",")+"\">";
					}
					html += "<input type=\"hidden\" name=\"itcid[]\" value=\""+itcid+"\">";
					html += "<input type=\"hidden\" name=\"ppoid[]\" value=\""+ppoid+"\"></td>";								
				} else {
					html += "<td id=\"valor_unitario_pai_"+itcid+"\"></td>";
				}
			<? } ?>
			
			// Unidade de Medida
			html += "<td align=\"center\" id=\"umdeesc"+umdeesc+" \">"+umdeesc+"</td>";
			
			// Quantidade = itcquantidade
			// Verifico se são obras MI
			<? if($esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI || $reformulaMI){?>
				if( boFilho < 1 ) {
					html += "<td><input type=\"text\" class=\"normal quantidade\" onblur=\"this.value = mascaraglobal('###.###.###.###,##',this.value);\" onkeyup=\"this.value = mascaraglobal('###.###.###.###,##',this.value);\" size=\"17\" maxlength=\"17\" id=\"quantidade_"+itcid+" \" "+stAtivo+" name=\"itcquantidade[]\" value=\""+itcquantidade+"\">";
					//html += "<td class=\"quantidade\" align=\"right\" id=\"quantidade_"+itcid+" \">"+itcquantidade+"</td>";
				} else {
					html += "<td id=\"qtde_pai_"+itcid+"\"></td>";
				}
			<? } else { ?>
				if( boFilho < 1 ) {
					html += "<td class=\"quantidade\" align=\"right\" id=\"quantidade_"+itcid+" \">"+itcquantidade+"</td>";
				} else {
					html += "<td id=\"qtde_pai_"+itcid+"\"></td>";
				}
			<? } ?>
			
			
			
			// Valor e porcentagem
			if( boFilho < 1 ) {
				html += "<td align=\"right\" id=\"valor_"+itcid+"\" class=\"valor\">" + (ppovalorunitario * itcquantidade).toFixed(2).replace(".",",") + "</td>";
				html += "<td align=\"right\" id=\"porcentagem_"+itcid+"\"></td>";
			} else {
				html += "<td id=\"valor_pai_"+itcid+"\" ></td>";
				html += "<td id=\"percent_pai_"+itcid+"\" ></td>";
			}
			html += "</tr>";

			jQuery('#tabela_planilha tr:last').after(html);	
        	
		});

		//alert(totalValor)
		var html2 = "<tr style=\"background: #e0e0e0\" >";
			html2 += "<td><strong>TOTAL:</strong></td>";
			
			//html2 += "<td id=\"totalValorUni\" align=\"right\"><strong>" + parseFloat(total_valor_unitario).toFixed(2).replace(".",",") + "</strong></td>";
			html2 += "<td id=\"totalValorUni\" align=\"right\"></td>";
			html2 += "<td></td>";
			html2 += "<td></td>";
			total_valor = Number(total_valor.toString().match(/^\d+(?:\.\d{0,2})?/));
			html2 += "<td id=\"totalValor\" align=\"right\"><strong>" + total_valor.toString().replace(".",",") + "</strong></td>";
			html2 += "<td align=\"right\" ><strong>100</strong></td>";
			html2 += "</tr>";
		jQuery('#tabela_planilha tr:last').after(html2);

		for (i=0;i<=arrItens.length;i++){
			if(arrItens[i]){
				if(arrItens[i].valor == 0 && total_valor == 0){
					var valor = 0; 
				} else {
					var valor = arrItens[i].valor / total_valor;
				}
				jQuery('#porcentagem_' + arrItens[i].itcid ).html(  parseFloat( valor * 100).toFixed(2).replace(".",",")  );
			}			
		}
		
		jQuery('#aguarde_').hide();
		
		
				  }
	 });
	
//}


	jQuery('#tabela_planilha tr')
		.live('mouseover',function(){
			if(jQuery(this).attr('cor')){
	        	jQuery(this).css('background','#ffffcc');
			}
	    })
	    .live('mouseout',function(){
	    	if(jQuery(this).attr('cor')){
	    		jQuery(this).css('background',jQuery(this).attr('cor'));
	    	}
	    });

    jQuery('.itcvalorunitario').live('keyup', function(event){

    	jQuery(this).val(mascaraglobal('###.###.###.###,##',jQuery(this).val()));
    	if(event.keyCode != 9){
	    	var itcvalorunitario = jQuery(this).val();
	    	
	    	if(itcvalorunitario == ''){
	    		itcvalorunitario = 0;
	        }
	    	
	    	itcvalorunitario = parseFloat(replaceAll(replaceAll(itcvalorunitario,".",""),",","."));
	
	    	var ppovalorunitario = jQuery(this).parent('td').next().next().text();
	    	var valor      = parseFloat(ppovalorunitario)*parseFloat(itcvalorunitario);
	    	
	    	jQuery(this).parent('td').next().next().next().text(mascaraglobal('###.###.###.###,##',valor.toFixed(2).replace(".",",")));
	
	
	    	var totalValor = 0;
	    	jQuery('.valor').each(function(i){
	    		totalValor = totalValor + parseFloat(replaceAll(replaceAll(jQuery(this).text(),".",""),",","."));
	        });
	
	    	jQuery('#totalValor').text(mascaraglobal('###.###.###.###,##',totalValor.toFixed(2).replace(".",","))).css('font-weight','bold');
	
	    	var totalValorUni = 0;
	    	jQuery('input[name=itcvalorunitario[]]').each(function(i){
	        	var valorUni = jQuery(this).val();
	        	if(valorUni == ''){
	        		valorUni = 0;
	            } 
	    		totalValorUni = totalValorUni + parseFloat(replaceAll(replaceAll(valorUni,".",""),",","."));
	        });	        	        
	
	    	jQuery('#totalValorUni').text(mascaraglobal('###.###.###.###,##',totalValorUni.toFixed(2).replace(".",","))).css('font-weight','bold');	
	
	    	var porcentagem = (100*parseFloat(valor))/parseFloat(totalValor);
	    	jQuery(this).parent('td').next().next().next().next().text(porcentagem.toFixed(2).replace(".",","))
        }

    });
    
    jQuery('.quantidade').live('keyup', function(event){
		
		//jQuery(this).val(mascaraglobal('###.###.###.###,##',jQuery(this).val()));
		
		var quantidade = replaceAll(replaceAll(jQuery(this).val(),'.',''),',','.');
		var valorUnitario = jQuery(this).parent('td').prev().prev().text();
    	
		var valorLinha = quantidade*parseFloat(replaceAll(replaceAll(valorUnitario,".",""),",","."));;
		
		jQuery(this).parent('td').next().text(mascaraglobal('###.###.###.###,##',valorLinha.toFixed(2).replace(".",",")));

    });
    
    jQuery('.quantidade').live('blur', function(event){
		
		//jQuery(this).val(mascaraglobal('###.###.###.###,##',jQuery(this).val()));
		
		var quantidade = replaceAll(replaceAll(jQuery(this).val(),'.',''),',','.');
		var valorUnitario = jQuery(this).parent('td').prev().prev().text();
    	
		var valorLinha = quantidade*parseFloat(replaceAll(replaceAll(valorUnitario,".",""),",","."));;
		
		jQuery(this).parent('td').next().text(mascaraglobal('###.###.###.###,##',valorLinha.toFixed(2).replace(".",",")));

    });
    
    jQuery('.quantidade').keyup();
    
    jQuery('.itcvalorunitario').live('blur', function(event){

    	jQuery(this).val(mascaraglobal('###.###.###.###,##',jQuery(this).val()));
    	if(event.keyCode != 9){
	    	var itcvalorunitario = jQuery(this).val();
	    	
	    	if(itcvalorunitario == ''){
	    		itcvalorunitario = 0;
	        }
	    	
	    	itcvalorunitario = parseFloat(replaceAll(replaceAll(itcvalorunitario,".",""),",","."));
	
	    	var ppovalorunitario = jQuery(this).parent('td').next().next().text();
	    	var valor      = parseFloat(ppovalorunitario)*parseFloat(itcvalorunitario);
	    	
	    	jQuery(this).parent('td').next().next().next().text(mascaraglobal('###.###.###.###,##',valor.toFixed(2).replace(".",",")));
	
	
	    	var totalValor = 0;
	    	jQuery('.valor').each(function(i){
	    		totalValor = totalValor + parseFloat(replaceAll(replaceAll(jQuery(this).text(),".",""),",","."));
	        });
	
	    	jQuery('#totalValor').text(mascaraglobal('###.###.###.###,##',totalValor.toFixed(2).replace(".",","))).css('font-weight','bold');
	
	    	var totalValorUni = 0;
	    	jQuery('input[name=itcvalorunitario[]]').each(function(i){
	        	var valorUni = jQuery(this).val();
	        	if(valorUni == ''){
	        		valorUni = 0;
	            } 
	    		totalValorUni = totalValorUni + parseFloat(replaceAll(replaceAll(valorUni,".",""),",","."));
	        });	        	        
	
	    	jQuery('#totalValorUni').text(mascaraglobal('###.###.###.###,##',totalValorUni.toFixed(2).replace(".",","))).css('font-weight','bold');	
	
	    	var porcentagem = (100*parseFloat(valor))/parseFloat(totalValor);
	    	jQuery(this).parent('td').next().next().next().next().text(porcentagem.toFixed(2).replace(".",","))
        }

    });
	 
});

function alteraIcone(trId){
	var img = 'img_'+trId;
	var i = document.getElementById(img);
	var tabela = document.getElementById('tabela_planilha');
	if(i.src.search("menos.gif") > 0){
		i.src = "../imagens/mais.gif";
		for(i=0; i < tabela.rows.length; i++) {
			if(tabela.rows[i].id.search(trId+"_") >= 0) {
				tabela.rows[i].style.display = "none";
			}
		}
	} else if(i.src.search("mais.gif") > 0){
		i.src = "../imagens/menos.gif";
		for(i=0; i < tabela.rows.length; i++) {
			if(tabela.rows[i].id.search(trId+"_") >= 0) {
				tabela.rows[i].style.display = "";
			}
		}
	}
}

function atualizarPlanilhaMI(){
	window.location.href = window.location + '&atualiza=1';
}

//-->
</script>
<div id="divDebug"></div>
<center>
	<div id="aguarde_" style="display: none;position:absolute;color:#000033;top:50%;left:35%; width:300;font-size:12px;z-index:0;">
		<br><img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
	</div>
</center>