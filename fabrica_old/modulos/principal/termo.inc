<?php

/**
 *
 * Função de organiza o array e remove os indices iguais a ""
 * @param $array
 * @return Array
 * @author Rodrigo Pereira de Souza Silva
 */
function organizaArrayX($array = array()){
	for($i = 0; $i < count($array); $i++){
		if($array[$i] == ""){
			for($j = $i; $j < count($array); $j++){
				if($array[$j] != ""){
					$array[$i] = $array[$j];
					$array[$j] = "";
					break;
				}
			}
		}
	}
	// Removendo os indices iguais a ""
	for($i = count($array) - 1 ; $i >= 0; $i--){
		if($array[$i] == ""){
			unset($array[$i]);
		}else{
			break;
		}
	}
	return($array);
}

if( isset($_REQUEST['termo']) ){

	// incluindo as funções necessárias para gerar as páginas
	include  APPRAIZ."includes/classes/dateTime.inc";
	require_once(APPRAIZ."www/fabrica/funcoes_termo.php");

	if ($_REQUEST['ss']) {
		
		$oss = array();

		foreach ($_REQUEST['ss'] as $ss) {
			$oss_ss = buscaOSSolicitacaoServico($ss);
			$oss = array_merge($oss, $oss_ss);
			
		}
		
                
		// verificando se o usuário selecionou a opção TermoSolicitacaoServico
		// caso ele tenha selecionado então eu uso a solicitação de serviço pra esse termo
		if (in_array("TermoSolicitacaoServico", $_REQUEST['termo'])) {
							$tamanho_ss	= count( $_REQUEST['ss'] );
							$ct_ss 		= 1;
		
                            foreach ($_REQUEST['ss'] as $ss) {
                                    echo TermoSolicitacaoServico($ss);
                                   // echo "<div style='page-break-before: always'></div>";
                                   if( $ct_ss < $tamanho_ss ){
									echo '<div style="page-break-before:always;font-size:1;margin:0;border:0;"><span style="visibility: hidden;">-</span></div>';
									}
					
									$ct_ss++;
                            }
			
			// removendo o valor TermoSolicitacaoServico do array $_REQUEST['termo']
			$indice = array_search("TermoSolicitacaoServico", $_REQUEST['termo']);
			if (is_int($indice)) {
				$_REQUEST['termo'][$indice] = "";
				$_REQUEST['termo'] = organizaArrayX($_REQUEST['termo']);
			}
			
			if(count($_REQUEST['termo']) > 0 and count($oss)> 0){
				echo '<div style="page-break-before:always;font-size:1;margin:0;border:0;"><span style="visibility: hidden;">-</span></div>';
			}
		
		}
		
		
		if($_REQUEST['os'][0]){
			foreach ($_REQUEST['os'] as $os) {
				// verificando se a OS já está no array de OSs, caso já esteja então eu NÃO armazeno ela denovo
				if(!in_array($os, $oss)){
					array_push($oss, $os);
				}
			}
		}

	}else{
		foreach ($_REQUEST['os'] as $os) {
			$oss = buscaOSSolicitacaoServico($os);
		}

		if($_REQUEST['os'][0]){
			foreach ($_REQUEST['os'] as $os) {
				// verificando se a OS já está no array de OSs, caso já esteja então eu NÃO armazeno ela denovo
				if(!in_array($os, $oss)){
					array_push($oss, $os);
				}
			}
		}

        }


	$tamanho_termo	= count( $_REQUEST['termo'] );
	$ct_termo 		= 1;

	foreach ($_REQUEST['termo'] as $funcao) {



	if(!$_REQUEST['odsidAba']){
			$tamanho	= count( $oss );
			$ct 		= 1;		
		
			foreach ($oss as $os) {
	//			echo $funcao."({$os})";
				$html .=  $funcao($os);
				// quebra de página
				
				if( $ct < $tamanho ){
					$html .= '<div style="page-break-before:always;font-size:1;margin:0;border:0;"><span style="visibility: hidden;">-</span></div>';
				}
				
				$ct++;
			}
                    
		}
		else{
			$html .= $funcao($_REQUEST['odsidAba']);
		}
		if( $ct_termo < $tamanho_termo and count($oss)> 0){
			$html .= '<div style="page-break-before:always;font-size:1;margin:0;border:0;"><span style="visibility: hidden;">-</span></div>';
		}
		
		$ct_termo++;


	}
	echo $html;

	exit();

}

if(!$_REQUEST['odsidAba']){ // TELA CADOSEXECUCAO.INC FUNÇÃO: carregarMenuOS()

	//Chamada de programa
	include  APPRAIZ."includes/cabecalho.inc";
	echo '<br>';

}
else{
	$includesTermo = '
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" language="javascript" src="../includes/agrupador.js"></script>
	';

	echo montarAbasArray(carregarMenuOS());
}



switch($_REQUEST['men']) {
	case 'analise':
		$menu = carregarMenuAnaliseSolicitacao();
		echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
		break;

}



$titulo_modulo = "Termos de Referência";
monta_titulo( $titulo_modulo, "&nbsp;");

echo $includesTermo;
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript"><!--
$(document).ready(function() {

        var tipoTermo = null;

	$("#gerar").click(function () {

		<?php if(!$_REQUEST['odsidAba']){?>

                        if(tipoTermo == 'ss'){

                            selectAllOptions( formulario_.ss);
                            if(!$('#ss option:selected').val()){

                                alert('Selecione uma Solicitação de Serviço.');
                                return false;

                            }

                        }else if(tipoTermo == 'os'){

                            selectAllOptions( formulario_.os);
                            if(!$('#os option:selected').val()){

                                alert('Selecione uma Ordem de Serviço.');
                                return false;

                            }

                        }else{

                            alert('Selecione uma Ordem de Serviço ou uma Solicitação de Serviço.');
                            return false;

                        }
		<?php }?>

		var i = 0;
		// pegando todos os inputs que começam com termo e estejam checados
		$("input[id^='termo']:checked").each(function() {

				i++;
				window.open('','teste','width=800,height=800,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
				document.formulario_.submit();
				return true;
		})

		if(!i){
			alert('Selecione um dos Termos de Referência.');
			return false;
		}

		

	});

	$("#limpar").click(function () {
	    $('.checktermoOS').attr('disabled', false);
	    $('.checktermoSS').attr('disabled', false);
	    $('.checktermoSS').attr('checked', false);
	    $('.checktermoSS').attr('checked', false);
	});

        $('#ss, .checktermoSS, #combopopup_campo_busca_ss, .tdss').click(function(){

           tipoTermo = 'ss';

           $('.checktermoSS').attr('disabled', false);
           $('.checktermoOS').attr('disabled', true);
           
           $('.checktermoOS').attr('checked', false);
           
        });

        $('#os, .checktermoOS, #combopopup_campo_busca_os, .tdos').click(function(){

           tipoTermo = 'os';

           $('.checktermoOS').attr('disabled', false);
           $('.checktermoSS').attr('disabled', true);

           $('.checktermoSS').attr('checked', false);
           
        });
});
--></script>


<form method="post" name="formulario_" id="formulario_" target="teste" enctype="multipart/form-data">

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
		<?php if(!$_REQUEST['odsidAba']){ // TELA CADOSEXECUCAO.INC FUNÇÃO: carregarMenuOS() ?>
		<tr>
			<td class="SubtituloDireita">Solicitação de Serviço:</td>
			<td class="tdss">
				<?php

					$sql = "select distinct
								ss.scsid as codigo,
								'Nº SS '||ss.scsid::character varying(5)||') - '||trim(coalesce(sd.siddescricao,'Não cadastrado'))||' / '||trim(coalesce(replace(replace(replace(replace(replace(replace(substr(ss.scsnecessidade,0,100), chr(8226), ' - '), chr(8211), ' - '), chr(149), ' - '), chr(13)||chr(10), ' '), chr(39), ''), chr(34), ''),''))||'...' as descricao
								--'Nº SS '||ss.scsid::character varying(5)||') - '||trim(coalesce(sd.siddescricao,'Não cadastrado'))||' / '||  SUBSTR(trim(coalesce(ss.scsnecessidade,'')),1,77)||'...' as descricao
							from
								fabrica.solicitacaoservico ss
							inner join
								fabrica.analisesolicitacao s ON ss.scsid = s.scsid
							left join
								 fabrica.ordemservico os  ON s.scsid = os.scsid
							left join
								demandas.sistemadetalhe sd ON sd.sidid = ss.sidid
							left join
								fabrica.servicofaseproduto sfp ON sfp.ansid = s.ansid and sfp.tpeid = os.tpeid
							left join
								workflow.documento wok ON wok.docid = os.docid";

					combo_popup( 'ss', $sql, 'Selecione a(s) Ordem(ns) de Serviço', '400x400', 0, array(), '', 'S', true, true, 6, 400, null, null, null, null, null, true, true, null, true );
				?>
			</td>
			<td class="SubtituloDireita">Ordem de Serviço:</td>
			<td class="tdos">
				<?php

					$sql = "select distinct
								os.odsid as codigo,
								'Nº OS '||os.odsid::character varying(5)||') - '||trim(coalesce(sd.siddescricao,'Não cadastrado'))||' / '||trim(coalesce(replace(replace(replace(replace(replace(replace(substr(ss.scsnecessidade,0,100), chr(8226), ' - '), chr(8211), ' - '), chr(149), ' - '), chr(13)||chr(10), ' '), chr(39), ''), chr(34), ''),''))||'...' as descricao
							from
								fabrica.ordemservico os
							inner join
								fabrica.analisesolicitacao s ON s.scsid = os.scsid
							inner join
								fabrica.solicitacaoservico ss ON ss.scsid = s.scsid
							left join
								demandas.sistemadetalhe sd ON sd.sidid = ss.sidid
							left join
								fabrica.servicofaseproduto sfp ON sfp.ansid = s.ansid and sfp.tpeid = os.tpeid
							left join
								workflow.documento wok ON wok.docid = os.docid
							order by 1";

					combo_popup( 'os', $sql, 'Selecione a(s) Ordem(ns) de Serviço', '400x400', 0, array(), '', 'S', true, true, 6, 400, null, null, null, null, null, true, true, null, true );
				?>
			</td>
		</tr>
		<?php }else{
		$link = "<span style='cursor:pointer; color: #0066CC;' onclick=window.location.href='fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=".$_REQUEST['odsidAba']."'>".$_REQUEST['odsidAba']."</span>";
		$scsid = $db->pegaUm("Select scsid FROM fabrica.ordemservico where odsid = ".$_REQUEST['odsidAba']);
		    ?>
		<tr>
			<td class="SubtituloCentro" colspan="4" >Ordem de Serviço: <?=$link ?> 
				<input type="hidden" name="os[]" id="os" value="<?php echo $_REQUEST['odsidAba']?>">
				<input type="hidden" name="ss[]" id="ss" value="<?php echo $scsid?>">  
			</td>
		</tr>
		<?php }?>
		<tr bgcolor="">
			<td colspan="2" bgcolor="#ffffff" onmouseout="this.bgColor='#ffffff';" onmouseover="this.bgColor='#ffffcc';">
				<label>
					<input class="checktermoSS" type="checkbox" name="termo[]" id="termo" value="TermoSolicitacaoServico">&nbsp;Termo Solicitação Serviço
				</label>
			</td>
			<td colspan="2" bgcolor="#ffffff" onmouseout="this.bgColor='#ffffff';" onmouseover="this.bgColor='#ffffcc';">
				<label>
					<input class="checktermoOS" type="checkbox" name="termo[]" id="termo" value="TermoAberturaOrdemServico">&nbsp;Termo Abertura Ordem Serviço
				</label>
			</td>
                </tr>
                <tr>
			<td colspan="2" bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
				<label>
					<input class="checktermoSS" type="checkbox" name="termo[]" id="termo" value="TermoRecebimentoProvisorio">&nbsp;Termo Recebimento Provisório
				</label>
			</td>
                        <td colspan="2" bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
				<label>
					<input class="checktermoOS" type="checkbox" name="termo[]" id="termo" value="TermoRecebimentoProvisorio">&nbsp;Termo Recebimento Provisório
				</label>
			</td>
		</tr>
                <tr>
			<td colspan="2" bgcolor="#ffffff" onmouseout="this.bgColor='#ffffff';" onmouseover="this.bgColor='#ffffcc';">
				<label>
					<input class="checktermoSS" type="checkbox" name="termo[]" id="termo" value="TermoRecebimentoDefinitivo">&nbsp;Termo Recebimento Definitivo
				</label>
			</td>
			<td colspan="2" bgcolor="#ffffff" onmouseout="this.bgColor='#ffffff';" onmouseover="this.bgColor='#ffffcc';">
				<label>
					<input class="checktermoOS" type="checkbox" name="termo[]" id="termo" value="TermoRecebimentoDefinitivo">&nbsp;Termo Recebimento Definitivo
				</label>
			</td>
                </tr>
		<tr>
                    <td colspan="4" align="center">
				<input type="button" value="Gerar" id="gerar">
				&nbsp;
				<input type="reset" value="Limpar" id="limpar">
			</td>
		</tr>
	</table>
</form>