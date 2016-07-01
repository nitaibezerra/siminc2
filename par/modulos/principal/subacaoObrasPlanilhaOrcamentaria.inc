<?php
$oSubacaoControle = new SubacaoControle ();
$oPreObraControle = new PreObraControle ();

$_SESSION ['par'] ['preid'] = $_REQUEST ['preid'] ? $_REQUEST ['preid'] : $_SESSION ['par'] ['preid'];

if ($_SESSION ['par'] ['itrid'] == 1) {
	$campo = array (
			"campo" => "estuf",
			"valor" => $_SESSION ['par'] ['estuf'] 
	);
} else {
	$campo = array (
			"campo" => "muncod",
			"valor" => $_SESSION ['par'] ['muncod'] 
	);
}

$stAtivo = 'disabled="disabled"';

$reformulaMI = verificaMi ( $preid );
$verificaEmenda = verificaEmenda ( $preid );

$arEscolasQuadraSelecionadas = $oPreObraControle->verificaEscolasQuadraSelecionadas ( $campo );
$arEscolasQuadraSelecionadas = $arEscolasQuadraSelecionadas ? $arEscolasQuadraSelecionadas : array ();

$boTipo_A = $oPreObraControle->verificaGrupoMunicipioTipoObra_A ( $_SESSION ['par'] ['muncod'] );

$arEscolasQuadra = $oPreObraControle->recuperarEscolasQuadra ( $preid );
$arEscolasQuadra = $arEscolasQuadra ? $arEscolasQuadra : array ();

if ($preid) {
	$obSubacaoControle = new SubacaoControle ();
	$obPreObra = new PreObra ();
	$arDados = $obSubacaoControle->recuperarPreObra ( $preid );
	$docid = prePegarDocid ( $preid );
	$esdid = prePegarEstadoAtual ( $docid );
}

$valorReferencia = false;
	
$arrEsdid =  Array (
		WF_PAR_EM_CADASTRAMENTO,
		WF_PAR_EM_DILIGENCIA,
		WF_PAR_OBRA_EM_REFORMULACAO,
		WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR,
		WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR
	);
if ($verificaEmenda) {
	
	if ($esdid == WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL || $esdid == WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL) {
		if (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) || in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ) || in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil )) {
			// Só o Valor de referência da obra para Emendas Condicionais
			$valorReferencia = true;
			$boAtivo = 'S';
			$stAtivo = '';
		}
	}
	if ( in_array( $esdid, $arrEsdid ) ) {
		if (in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_ADM_OBRAS, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_PREFEITO, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil ) || in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil )) {
			$valorReferencia = false;
			$boAtivo = 'S';
			$stAtivo = '';
		}
	}
}

if( in_array( $esdid, $arrEsdid ) ){
	if (in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_ADM_OBRAS, $perfil )  || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_PREFEITO, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil ) || in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil )) {
		$boAtivo = 'S';
		$stAtivo = '';
	}
} 

/*
 * Implementado dia 27/11/2013 - Analista: Daniel Areas Brito, Desenvolvedor: Eduardo Dunice Neto Em Analise de Engenharia, abre para edição no perfil de Coordenador Técnico para os tipos de Obra de Ampliação e Reforma.
 */
$arrESDID = Array (
		WF_PAR_EM_ANALISE,
		WF_PAR_OBRA_APROVADA 
);
if (in_array ( $esdid, $arrESDID )) {
	
	$arrReformaAmpliacao = Array (
			OBR_TPOID_REFORMA,
			OBR_TPOID_AMPLIACAO,
			OBR_TPOID_REFORMA_UAB,
			OBR_TPOID_AMPLIACAO_UAB,
			OBR_TPOID_AMPLIACAO_EMENDA,
			OBR_TPOID_REFORMA_EMENDA,
			OBR_TPOID_AMPLIACAO_UAB_EMENDA,
			OBR_TPOID_REFORMA_UAB_EMENDA,
			OBR_TPOID_TIPO_A_EMENDA 
	);
	
	if (in_array ( $arDados ['ptoid'], $arrReformaAmpliacao ) && (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ))) {
		$boAtivo = 'S';
		$habil = 'S';
		$stAtivo = '';
	}
}

/*
 * Implementado dia 13/12/2013 - Analista: Daniel Areas Brito, Desenvolvedor: Eduardo Dunice Neto Obras do Tipo Projeto próprio, Reforma e Ampliação sempre serão editadas pelo coordenador geral
 */
$arrReformaAmpliacaoProjetoProprio = Array (
		OBR_TPOID_ESCOLA_PROJETO_PROPRIO,
		OBR_TPOID_ESCOLA_PROJETO_PROPRIO_EMENDA,
		OBR_TPOID_COBERTURA_DE_QUADRA_ESCOLAR_PROJETO_PROPRIO_EMENDA,
		OBR_TPOID_QUADRA_ESCOLAR_COBERTA_PROJETO_PROPRIO_EMENDAOBR_TPOID_REFORMA,
		OBR_TPOID_REFORMA,
		OBR_TPOID_AMPLIACAO,
		OBR_TPOID_REFORMA_UAB,
		OBR_TPOID_AMPLIACAO_UAB,
		OBR_TPOID_REFORMA_EMENDA,
		OBR_TPOID_AMPLIACAO_EMENDA,
		OBR_TPOID_REFORMA_UAB_EMENDA,
		OBR_TPOID_AMPLIACAO_UAB_EMENDA,
		OBR_TPOID_TIPO_A_EMENDA 
);

$arrTipoPerfilCoordenador = Array(
		WF_PAR_OBRA_ARQUIVADA,
		WF_PAR_VALIDACAO_INDEFERIMENTO,
		WF_PAR_OBRA_EM_VALIDACAO_INDEFERIMENTO_REFORMULACAO,
		WF_PAR_OBRA_CANCELADA
);

if (in_array ( $arDados ['ptoid'], $arrReformaAmpliacaoProjetoProprio ) && (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) && !in_array ( $esdid, $arrTipoPerfilCoordenador) ) )) {
	$boAtivo = 'S';
	$habil = 'S';
	$stAtivo = ''; 
}

$visualizaBotaoAtualizacao = false;
if( $reformulaMI ){
	if($esdid == WF_PAR_VALIDACAO_DEFERIMENTO || $esdid == WF_PAR_EM_DILIGENCIA || $esdid == WF_PAR_EM_ANALISE_FNDE || $esdid == WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA){
		if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_ADM_OBRAS, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)){
			$visualizaBotaoAtualizacao = true;
			$boAtivo = 'S';
			$habil = 'S';
			$stAtivo = ''; 
		}
	}
}

if ($_POST ['vocvalorobra']) {
	
	global $db;
	
	$valor = retiraPontosBD ( $_POST ['vocvalorobra'] );
	
	$teste = $db->pegaUm ( "SELECT preid FROM par.valorobracondicional WHERE preid = " . $preid );
	
	$sqlAlteracao = "UPDATE obras.preobra set prevalorobra = " . $valor . " WHERE preid = " . $preid . "; ";
	if ($teste) {
		$sqlAlteracao .= "UPDATE par.valorobracondicional SET vocvalorobra = " . $valor . " WHERE preid = " . $preid . "; ";
	} else {
		$sqlAlteracao .= "INSERT INTO par.valorobracondicional (preid, vocvalorobra) VALUES (" . $preid . ", " . $valor . "); ";
	}
	
	$db->executar ( $sqlAlteracao );
	$db->commit ();
	
	echo '<script type="text/javascript"> 
			alert("Operação realizada com sucesso.");
		  </script>';
}

if ($_POST ['ppovalorunitario'] && ! $_POST ['vocvalorobra']) {
	
	$count = count ( $_POST ['itcid'] );
	$obPrePlanilhaOrcamentaria = new PrePlanilhaOrcamentaria ();
// 	ver($_POST,d);
	for($x = 0; $x < $count; $x ++) {
		
		if ($_POST ['ppovalorunitario'] [$x] != '' && $_POST ['ppovalorunitario'] [$x] != '0' && $_POST ['ppovalorunitario'] [$x] != "0,00" && $_POST ['ppovalorunitario'] [$x] != $_POST ['ppovalorunitario_ant'] [$x]) {
			$arDados ['ppoid'] = $_POST ['ppoid'] [$x];
			$arDados ['preid'] = $preid;
			$arDados ['itcid'] = $_POST ['itcid'] [$x];
			$arDados ['ppovalorunitario'] = str_replace ( array (
					".",
					"," 
			), array (
					"",
					"." 
			), $_POST ['ppovalorunitario'] [$x] );
			$obPrePlanilhaOrcamentaria->excluiItensPlanilhaOrcamentaria ( $arDados ['preid'], $arDados ['itcid'] );
			
			$obPrePlanilhaOrcamentaria->ppoid = null;
			$obPrePlanilhaOrcamentaria->preid = $arDados ['preid'];
			$obPrePlanilhaOrcamentaria->itcid = $arDados ['itcid'];
			$obPrePlanilhaOrcamentaria->ppovalorunitario = str_replace ( ' ', '', $arDados ['ppovalorunitario'] );
			$obPrePlanilhaOrcamentaria->salvar ();
		}
	}
	
	$obPrePlanilhaOrcamentaria->commit ();
	atualizaValorObra ( $preid );
	
	echo '<script type="text/javascript"> 
			alert("Operação realizada com sucesso.");
		  </script>';
}

if ($_POST ['itcquantidade'] && $boAtivo == 'S' && ! $_POST ['vocvalorobra']) {
	
	global $db;
	
	$count = count ( $_POST ['itcid'] );
	
	for($x = 0; $x < $count; $x ++) {
		
		if ($_POST ['itcquantidade'] [$x] != '' && 		// && $_POST['itcquantidade'][$x] != '0' && $_POST['itcquantidade'][$x] != "0,00"
		$_POST ['itcquantidade'] [$x] != $_POST ['itcquantidade_ant'] [$x]) {
			$arDados ['ppoid'] = $_POST ['ppoid'] [$x];
			$arDados ['preid'] = $preid;
			$arDados ['itcid'] = $_POST ['itcid'] [$x];
			// $arDados['itcquantidade'] = str_replace('.','',str_replace(",",".",$_POST['itcquantidade'][$x]));
			
			$arDados ['itcquantidade'] = retiraPontosBD ( $_POST ['itcquantidade'] [$x] );
			
			$sqlUp .= "UPDATE obras.preitenscomposicaomi SET itcquantidade = " . $arDados ['itcquantidade'] . " WHERE itcid = " . $arDados ['itcid'] . "; ";
		}
	}
	$db->executar ( $sqlUp );
	$db->commit ();
	
	atualizaValorObra ( $preid, 'mi' );
	
	echo '<script type="text/javascript"> 
			alert("Operação realizada com sucesso.");
			document.location.href = \'' . $_SERVER ['HTTP_REFERER'] . '\';
		  </script>';
	exit ();
}

$tipoObra = $oSubacaoControle->verificaTipoObra ( $preid, SIS_OBRAS );
$boPlanilhaOrcamentaria = $oSubacaoControle->verificaCategoriaObra ( $preid );

if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ){
	$boAtivo = 'S';
	$stAtivo = '';
}
cabecalho();
?>
<div id="lista">
	<form action="" method="post" name="formulario" id="formulario">
		<table id="tabela_planilha" class="tabela" bgcolor="#f5f5f5"
			cellpadding="3" align="center">
			<tr style="background-color: #e0e0e0">
				<td style="font-weight: bold; text-align: center; width: 20%;">Descriçao
					do item</td>
				<td style="font-weight: bold; text-align: center; width: 10%;">Valor
					Unitario</td>
				<td style="font-weight: bold; text-align: center;">Unidade de Medida</td>
				<td style="font-weight: bold; text-align: center; width: 30%;">Quantidade</td>
				<td style="font-weight: bold; text-align: center; width: 10%;">Valor</td>
				<td style="font-weight: bold; text-align: right; width: 10%;">%</td>
			</tr>
		</table>
		<?php
		
if ($valorReferencia) {
			$vocvalorobra = $db->pegaUm ( "SELECT vocvalorobra FROM par.valorobracondicional WHERE preid = " . $preid );
			?>
			<table class="tabela" style="margin-bottom: 15px" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding="3" align="center">
			<colgroup>
				<col width="25%" />
				<col width="75%" />
			</colgroup>
			<tbody>
				<tr>
					<td style="background-color: #dcdcdc;" colspan="2"><b>Os empenhos
							dos valores referentes a emendas parlamentares indicadas para
							obras são feitos antes da aprovação dos documentos técnicos, com
							embasamento nas Portarias nº507/2011, artigo 37, § 1º e nº274/13,
							artigo 17. A liberação de recursos fica condicionada à aprovação
							total do projeto básico.</b></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Valor da emenda:</td>
					<td><?php echo campo_texto( "vocvalorobra", 'S', $boAtivo, '', 40, '', '[.###],##', '','','','','id="predescricao"','',$vocvalorobra); ?></td>
				</tr>
			</tbody>
		</table>
		<?php } ?>
		<table class="tabela" bgcolor="#f5f5f5" cellpadding="3" align="center">
			<tr style="background-color: #e0e0e0">
				<?php if($boAtivo == 'S' && $habil == 'S'){ ?>
					<?php if($editavel != "readonly='readonly'"){?>
						<td id="td_tbl_salvar" style="text-align: center;"><input
						type="button" class="validaForm" value="Salvar" <?=$stAtivo ?> /></td>
					<?php }} ?>
				<?php if( $visualizaBotaoAtualizacao ){ ?>
					<td style="text-align: center;"><input type="button" value="Atualizar Planilha" onclick="atualizarPlanilhaMI();" /></td>
				<?php } ?>
			</tr>
		</table>
	</form>
</div>
<script type="text/javascript">

jQuery(document).ready(function(){

	jQuery('input').keyup();

	jQuery('.validaForm').click(function(){
//		alert(123);
		jQuery('.validaForm').attr('disabled',true);
		jQuery('.validaForm').val('Aguarde...');
		jQuery("#formulario").submit();
	});
	
	var param = 'requisicao=montaArvoreAberta&db=1&ptoid='+<?php echo $tipoObra ?>;
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
	
	//param = "requisicao=montaArvoreAberta&db=1&ptoid=7";
	
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
		if(!data){
			jQuery('#aguarde_').hide();
			<?php if( $reformulaMI ){ ?>
				jQuery('#td_tbl_salvar').html("Pregões não cadastrados para o Estado!");
			<?php } else { ?>
				jQuery('#td_tbl_salvar').html("Não existem registros!");
			<?php } ?>
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
			
//			alert(item.itccodigoitemcodigo+" - "+item.itcid);
			
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
			
			var readonly = "<?=$readonly ?>";
			var stAtivo = '<?=$stAtivo ?>';

			// Verifico se são obras MI
			<? if($reformulaMI){?>
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
// 			if( boFilho < 1 && (itcquantidade > 0) ) {
			if( trim(umdeesc) != '' ) {
				if( ptopreencher != 't' ){
					html += "<td>"+mascaraglobal('###.###.###.###,##',parseFloat(ppovalorunitario).toFixed(2));
					html += "<input type=\"hidden\" class=\"normal itcvalorunitario\" name=\"ppovalorunitario[]\" value=\""+parseFloat(ppovalorunitario).toFixed(2).replace(".",",")+"\">";
					html += "<input type=\"hidden\" name=\"ppovalorunitario_ant[]\" value=\"\">";
				}else{
					html += "<td><input type=\"text\" class=\"normal itcvalorunitario\" id=\"item_"+itcid+"\" "+readonly+" name=\"ppovalorunitario[]\" value=\""+parseFloat(ppovalorunitario).toFixed(2).replace(".",",")+"\">";
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
			
			// Verifico se são obras MI
			<? if($esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI || $reformulaMI){?>
				if( boFilho < 1 ) {
					html += "<td><input type=\"text\" class=\"normal quantidade\" onblur=\"this.value = mascaraglobal('###.###.###.###,##',this.value);\" onkeyup=\"this.value = mascaraglobal('###.###.###.###,##',this.value);\" size=\"17\" maxlength=\"17\" id=\"quantidade_"+itcid+" \" "+stAtivo+" name=\"itcquantidade[]\" value=\""+itcquantidade.replace(".",",")+"\">";
					//html += "<td class=\"quantidade\" align=\"right\" id=\"quantidade_"+itcid+" \">"+itcquantidade+"</td>";
				} else {
					html += "<td id=\"qtde_pai_"+itcid+"\"></td>";
				}
			<? } else { ?>
				if( boFilho < 1 ) {
					// Quantidade = itcquantidade
					html += "<td class=\"quantidade\" align=\"right\" id=\"quantidade_"+itcid+" \">"+itcquantidade+"</td>";
				} else {
					html += "<td id=\"qtde_pai_"+itcid+"\"></td>";
				}
			<? } ?>

			if( boFilho < 1 ) {
				// Valor e porcentagem
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
			html2 += "<td id=\"totalValor\" align=\"right\"><strong>" + parseFloat(total_valor).toFixed(2).replace(".",",") + "</strong></td>";
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

<?php if( $valorReferencia ){ ?>
document.getElementById('tabela_planilha').style.display = "none";
<? } ?>

</script>
<center>
	<div id="aguarde_"
		style="display: none; position: absolute; color: #000033; top: 50%; left: 35%; width: 300; font-size: 12px; z-index: 0;">
		<br>
		<img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
	</div>
</center>