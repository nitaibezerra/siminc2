<?php
criaSessaoProfissionais(); 
$profissionais = $db->pegaLinha("SELECT *
                                   FROM projovemurbano.profissionais
                                   WHERE proid='".$_SESSION['projovemurbano']['proid']."'
                                     --AND tprid = {$_SESSION['projovemurbano']['tprid']}");

$coordenadorgeral = pegarCoordenadorGeral($_SESSION['projovemurbano']['proid']);

if($coordgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordgeral['tprid'] != ''){
			$coordgera = $coordgeral['cgevlrtotal'];
}

$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'A');
$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'P');
$assistenteadministrativo_M = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'M', $tprid = $_SESSION['projovemurbano']['tprid']);


$assistenteadministrativo_AP = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'AP', $tprid = $_SESSION['projovemurbano']['tprid']);
$assistenteadministrativo_PP = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'PP', $tprid = $_SESSION['projovemurbano']['tprid']);

if($_SESSION['projovemurbano']['tprid'] == 2 ){
	$casoprisionais1 = $assistenteadministrativo_AP['coaqtd']+$assistenteadministrativo_PP['coaqtd'];
	$casoprisionais2 = $assistenteadministrativo_AP['coavlrtotal']+$assistenteadministrativo_PP['coavlrtotal'];
}

if($coordenadorgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordenadorgeral['tprid'] != ''){
	$assitente_A1 =  $assistenteadministrativo_A['coaqtd']+$assistenteadministrativo_P['coaqtd'];
	$assitente_A2 = $assistenteadministrativo_A['coavlrtotal']+$assistenteadministrativo_P['coavlrtotal'];
}

$coordassistentes = array("coaqtd"		=> $assitente_A1+$assistenteadministrativo_M['coaqtd']+$casoprisionais1,
						  "coavlrtotal" => $assitente_A2+$assistenteadministrativo_M['coavlrtotal']+$casoprisionais2); 

$diretorpolo = pegarDiretorPolo($_SESSION['projovemurbano']['proid']);
$dirassistentes_A = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'A');
$dirassistentes_P = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'P');

if( $coordenadorgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordenadorgeral['tprid'] != ''){
	$dirassistentes   = array("dasqtdefetivo40hr" 	 => $dirassistentes_A['dasqtdefetivo40hr']+$dirassistentes_P['dasqtdefetivo40hr'],
							  "dasqtdrecursoproprio" => $dirassistentes_A['dasqtdrecursoproprio']+$dirassistentes_P['dasqtdrecursoproprio'],
							  "creqtd" 				 => $dirassistentes_A['creqtd']+$dirassistentes_P['creqtd'],
							  "crevlrtotal" 		 => $dirassistentes_A['crevlrtotal']+$dirassistentes_P['crevlrtotal']);
}

if( $coordgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordgeral['tprid'] != ''){
			$diretorpol = pegarDiretorPolo($_SESSION['projovemurbano']['proid']);
}
$educadores_F = pegarEducadores($_SESSION['projovemurbano']['proid'], 'F');
$educadores_Q = pegarEducadores($_SESSION['projovemurbano']['proid'], 'Q');
$educadores_P = pegarEducadores($_SESSION['projovemurbano']['proid'], 'P');
$educadores_M = pegarEducadores($_SESSION['projovemurbano']['proid'], 'M');
$educadores_T = pegarEducadores($_SESSION['projovemurbano']['proid'], 'T');
$educadores_E = pegarEducadores($_SESSION['projovemurbano']['proid'], 'E');

//$totalUtilizado = 	$educadores_F['crevlrtotal']+
//					$educadores_Q['crevlrtotal']+
//					$educadores_P['crevlrtotal']+
//					$educadores_M['crevlrtotal']+
//					$educadores_T['crevlrtotal']+
//					$educadores_E['ccmvlrtotal'];


	$totalUtilizado =   $coordgera+
						$coordassistentes['coavlrtotal']+
// 						$dirassistentes['crevlrtotal']+
						$diretorpol['ccmvlrtotal']+
						$educadores_F['crevlrtotal']+
						$educadores_Q['crevlrtotal']+
						$educadores_P['crevlrtotal']+
						$educadores_M['crevlrtotal']+
						$educadores_T['crevlrtotal']+
						$educadores_E['ccmvlrtotal']+
						$educadores_F['ccmvlrtotal']+
						$educadores_Q['ccmvlrtotal']+
						$educadores_P['ccmvlrtotal'];

						
//ver(					$coordgera,
//						$coordassistentes['coavlrtotal'],
//						$dirassistentes['crevlrtotal'],
//						$diretorpol['ccmvlrtotal'],
//						$educadores_F['crevlrtotal'],
//						$educadores_Q['crevlrtotal'],
//						$educadores_P['crevlrtotal'],
//						$educadores_M['crevlrtotal'],
//						$educadores_T['crevlrtotal'],
//						$educadores_E['ccmvlrtotal'],
//						$educadores_F['ccmvlrtotal'],
//						$educadores_Q['ccmvlrtotal'],
//						$educadores_P['ccmvlrtotal']);
$contagemPolos = pegarNumeroPolos( true );

$contagemPolos = ( $contagemPolos ? $contagemPolos : array());
$tenhoPolo = false;
$numeropolos = 0;

$sql = "SELECT pmupossuipolo
		FROM projovemurbano.polomunicipio
		WHERE
		pjuid='".$_SESSION['projovemurbano']['pjuid']."'
		AND tprid = {$_SESSION['projovemurbano']['tprid']} AND pmustatus='A'";
//ver($sql);
$tenhoPolo = $db->pegaUm($sql);

// -- Consulta os polos agrupados para ver se o polo foi definido para este público
if ($tenhoPolo == 't' ) {
    foreach ($contagemPolos as $poloTotal) {
//         if ($_SESSION['projovemurbano']['tprid'] == $poloTotal['tprid']) {
//            $tenhoPolo = true;
//         }
        $numeropolos += (int)$poloTotal['count'];
    }
}
$numeronucleos = pegarNumeroNucleos($tenhoPolo);
//ver($numeropolos);
//ver($numeronucleos);
//ver($_SESSION['projovemurbano']['muncod']); 
if($_SESSION['projovemurbano']['muncod']) 
    $propercmax="77";
else if($_SESSION['projovemurbano']['estuf']) 
    $propercmax="75.5";

$sugestaoampliacao = carregarSugestaoAmpliacao();
$meta = carregarMeta($sugestaoampliacao);


$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/projovemurbano.js"></script>
<script>
jQuery(document).ready(function(){

	jQuery('.zerarCoord').click(function(){
		jQuery('#requisicao').val('zerarCoord');
		jQuery('#form').submit();
	});

	jQuery('[name*="cgeqtdmeses"]').blur(function(){
		if( parseFloat(jQuery(this).val()) > parseFloat('22') ){
			jQuery(this).val('22');
		}
	});

	jQuery('[name*="coaqtdmeses"]').blur(function(){
		if( parseFloat(jQuery(this).val()) > parseFloat('22') ){
			jQuery(this).val('22');
		}
	});

	calculaPorcentTotal();
});

function gravarProfissionais() {

	if(document.getElementById('inldatainstlegal').value=='') {
		alert('Preencha a Data do instrumento');
		return false;
	}
	
	if(!validaData(document.getElementById('inldatainstlegal'))) {
		alert('Data do instrumento inválida. Formato deve ser dd/mm/YYYY');
                rolaTela('inldatainstlegal');
		return false;
	}
	
    if(document.getElementById('inlnuminstlegal').value=='') {
		alert('Preencha o número do instrumento');
                rolaTela('inlnuminstlegal');
		return false;
	}

        if ('' == jQuery('#tpdid').val()) {
            alert('Selecione o tipo de documento');
            rolaTela('tpdid');
            return false;
        }
        if ('' == jQuery('#arquivo').val()) {
            alert('Selecione o documento do instrumento legal');
            rolaTela('arquivo');
            return false
        }

	if(parseFloat(document.getElementById('propercutilizado').value) > parseFloat(document.getElementById('propercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
		return false;
	}
	
	var vlr1 = parseFloat(document.getElementById('totalMax').value);
	var vlr2 = parseFloat(document.getElementById('totalUtilizado').value);

	if(vlr2>vlr1) {
		alert('Valor Utilizado maior do que Valor Máximo previsto');
		return false;
	}
	
	<? if($numeropolos>0) : ?>
	if(!validaQuantidadeCargos('dipeqtdefetivo40hr','dipqtdrecursoproprio','ccmqtd','','dipqtd','sim')) {
		alert('Diretor Polo: O número do Efetivo + Contratado deve ser igual a quantidade');
		return false;
	}
	<? endif; ?>
	<? if($numeropolos>0) : ?>
	if(!validaQuantidadeCargos('dasqtdefetivo40hr_AA','dasqtdrecursoproprio_AA','creqtd_A','','dasqtd_AA','sim')) {
		alert('Assistente Administrativo: O número do Efetivo + Contratado deve ser igual a quantidade');
		return false;
	}
	<? endif; ?>
	<? if($numeropolos>0) : ?>
	if(!validaQuantidadeCargos('dasqtdefetivo40hr_AP','dasqtdrecursoproprio_AP','creqtd_P','','dasqtd_AP','sim')) {
		alert('Assistente Pedagógico: O número do Efetivo + Contratado deve ser igual a quantidade');
		return false;
	}
	<? endif; ?>
	
	<? if($numeronucleos) : ?>
	if(!validaQuantidadeCargos('eduefetivo30hr_EF','eduqtdrecursoproprio_EF','creqtd_EF','ccmqtd_EF','eduqtd_EF','sim')) {
		alert('Educadores do Ensino Fundamental: O número do Efetivo + Contratado deve ser igual a quantidade');
		return false;
	}
	<? endif; ?>
	<? if($numeronucleos) : ?>
	if(!validaQuantidadeCargos('eduefetivo30hr_EQ','eduqtdrecursoproprio_EQ','creqtd_EQ','ccmqtd_EQ','eduqtd_EQ','sim')) {
		alert('Educadores do Qualificação Profissional: O número do Efetivo + Contratado deve ser igual a quantidade');
		return false;
	}
	<? endif; ?>
	<? if($numeronucleos) : ?>
	if(!validaQuantidadeCargos('eduefetivo30hr_EP','eduqtdrecursoproprio_EP','creqtd_EP','ccmqtd_EP','eduqtd_EP','sim')) {
		alert('Educadores de Participação Cidadã: O número do Efetivo + Contratado deve ser igual a quantidade');
		return false;
	}
	<? endif; ?>
	<? if($numeronucleos) : ?>
//	if(!validaQuantidadeCargos('eduefetivo30hr_EM','eduqtdrecursoproprio_EM','creqtd_EM','','eduqtd_EM','sim')) {
//		alert('Educadores para monitoramento do acolhimento ás crianças de 0 a 8 anos: O número do Efetivo + Contratado deve ser igual a quantidade');
//		return false;
//	}
	<? endif; ?>
	<? if($numeronucleos) : ?>
//	if(!validaQuantidadeCargos('eduefetivo30hr_ET','eduqtdrecursoproprio_ET','creqtd_ET','','eduqtd_ET','sim')) {
//		alert('Tradutor de Intérprete de Libras: O número do Efetivo + Contratado deve ser igual a quantidade');
//		return false;
//	}
	<? endif; ?>
	<? if($numeronucleos) : ?>
//	if(!validaQuantidadeCargos('eduefetivo30hr_EE','eduqtdrecursoproprio_EE','','','eduqtd_EE','sim')) {
//		alert('Educador para atendimento educacional especializado: O número do Efetivo + Contratado deve ser igual a quantidade');
//		return false;
//	}
	<? endif; ?>

	jQuery('#form').submit();
}

function excluirAnexo(arqid) {
    if (confirm('Deseja remover o arquivo?')) {
        jQuery('#arquivo').val(arqid);
        jQuery('#requisicao').val('excluirAnexo');
        jQuery('#form').submit();
    }
}

function calculaValorPorMes(vlrhora, qtdhora, result) {
	if(document.getElementById(vlrhora).value && document.getElementById(qtdhora).value) {
		var valor = replaceAll(replaceAll(document.getElementById(vlrhora).value,".",""),",",".");
		document.getElementById(qtdhora).value = mascaraglobal('#######',document.getElementById(qtdhora).value);
		var horas = document.getElementById(qtdhora).value;
		var compl = valor*horas;
		document.getElementById(result).value = mascaraglobal('###.###.###,##',compl.toFixed(2));
	} else {
		document.getElementById(result).value = '0,00';
	}
}

function calculaValorTotal(id_valor, id_meses, id_encargos, id_total, id_qtd) {
	if(document.getElementById(id_valor).value && document.getElementById(id_meses).value && document.getElementById(id_encargos).value) {
		document.getElementById(id_meses).value = mascaraglobal('#######',document.getElementById(id_meses).value);
		var valor = replaceAll(replaceAll(document.getElementById(id_valor).value,".",""),",",".");
		var meses = document.getElementById(id_meses).value;
		var valorencargos = valor * meses * (document.getElementById(id_encargos).value/100);
		var total = (valor*meses)+valorencargos;
		if(id_qtd) {
			if(document.getElementById(id_qtd).value!='') {
				var qtd = document.getElementById(id_qtd).value;
				total = qtd*total;
				document.getElementById(id_total).value = mascaraglobal('###.###.###,##',total.toFixed(2));		
			} else {
				document.getElementById(id_total).value = ''; 
			}
		} else {
			document.getElementById(id_total).value = mascaraglobal('###.###.###,##',total.toFixed(2));
		}
	}
}

function validaQuantidadeCargos(id_qtd1,id_qtd2,id_qtd3,id_qtd4,id_qtdtotal,submeter) {
	if(id_qtd1) {
		var qtd1 = document.getElementById(id_qtd1).value;
		if(qtd1==''){qtd1=0;}
	} else {
		var qtd1 = 0;
	}
	
	if(id_qtd2) {
		var qtd2 = document.getElementById(id_qtd2).value;
		if(qtd2==''){qtd2=0;}
	} else {
		var qtd2 = 0;
	}
	
	if(id_qtd3) {
		var qtd3 = document.getElementById(id_qtd3).value;
		if(qtd3==''){qtd3=0;}
	} else {
		var qtd3 = 0;
	}

	if(id_qtd4) {
		var qtd4 = document.getElementById(id_qtd4).value;
		if(qtd4==''){qtd4=0;}
	} else {
		var qtd4 = 0;
	}
 
	
	if(submeter=='sim') {
	
		if((document.getElementById(id_qtdtotal).value-qtd1-qtd2-qtd3-qtd4) != 0) {
			return false;
		} else {
			return true;
		}
	
	} else {
	
		if((document.getElementById(id_qtdtotal).value-qtd1-qtd2-qtd3-qtd4) < 0) {
			alert('O número do Efetivo + Contratado deve ser igual a quantidade');
			if(id_qtd1){document.getElementById(id_qtd1).value='';}
			if(id_qtd2){document.getElementById(id_qtd2).value='';}
			if(id_qtd3){document.getElementById(id_qtd3).value='';}
			if(id_qtd4){document.getElementById(id_qtd4).value='';}
		}
	
	}
}

function calculaPorcentTotal() {
        // -- Inclue estes campos no cálculo da porcentagem geral apenas se foram cadastrados no público atual, ou se ainda
        // -- não foram cadastrados
	<? if( $coordenadorgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] || $coordenadorgeral['tprid'] == ''  ){ ?>
	var cgevlrtotal=0;
	if(document.getElementById('cgevlrtotal')) {
		if(document.getElementById('cgevlrtotal').value!='') {
			cgevlrtotal 	= parseFloat(replaceAll(replaceAll(document.getElementById('cgevlrtotal').value,".",""),",","."));
		}
	}
	var coavlrtotal_A=0;
	if(document.getElementById('coavlrtotal_A')) {
		if(document.getElementById('coavlrtotal_A').value!='') {
			coavlrtotal_A 	= parseFloat(replaceAll(replaceAll(document.getElementById('coavlrtotal_A').value,".",""),",","."));
		}
	}
	var coavlrtotal_P=0;
	if(document.getElementById('coavlrtotal_P')) {
		if(document.getElementById('coavlrtotal_P').value!='') {
			coavlrtotal_P 	= parseFloat(replaceAll(replaceAll(document.getElementById('coavlrtotal_P').value,".",""),",","."));	
		}
	}
	var coavlrtotal_AP=0;
	if(document.getElementById('coavlrtotal_AP')) {
		if(document.getElementById('coavlrtotal_AP').value!='') {
			coavlrtotal_AP 	= parseFloat(replaceAll(replaceAll(document.getElementById('coavlrtotal_AP').value,".",""),",","."));
		}
	}
	var coavlrtotal_PP=0;
	if(document.getElementById('coavlrtotal_PP')) {
		if(document.getElementById('coavlrtotal_PP').value!='') {
			coavlrtotal_P 	= parseFloat(replaceAll(replaceAll(document.getElementById('coavlrtotal_PP').value,".",""),",","."));	
		}
	}
	
	var ccmvlrtotal=0;
	if(document.getElementById('ccmvlrtotal')) {
		if(document.getElementById('ccmvlrtotal').value!='') {
			ccmvlrtotal 	= parseFloat(replaceAll(replaceAll(document.getElementById('ccmvlrtotal').value,".",""),",","."));	
		}
	}
	var crevlrtotal_A=0;
	if(document.getElementById('crevlrtotal_A')) {
		if(document.getElementById('crevlrtotal_A').value!='') {
			crevlrtotal_A 	= parseFloat(replaceAll(replaceAll(document.getElementById('crevlrtotal_A').value,".",""),",","."));	
		}
	}
	var crevlrtotal_P=0;
	if(document.getElementById('crevlrtotal_P')) {
		if(document.getElementById('crevlrtotal_P').value!='') {
			crevlrtotal_P 	= parseFloat(replaceAll(replaceAll(document.getElementById('crevlrtotal_P').value,".",""),",","."));	
		}
	}
	<? } ?>

	var coavlrtotal_M=0;
	if(document.getElementById('coavlrtotal_M')) {
		if(document.getElementById('coavlrtotal_M').value!='') {
			coavlrtotal_M 	= parseFloat(replaceAll(replaceAll(document.getElementById('coavlrtotal_M').value,".",""),",","."));
		}
	}
	var crevlrtotal_EF=0;
	if(document.getElementById('crevlrtotal_EF')) {
		if(document.getElementById('crevlrtotal_EF').value!='') {
			crevlrtotal_EF 	= parseFloat(replaceAll(replaceAll(document.getElementById('crevlrtotal_EF').value,".",""),",","."));	
		}
	}
	var ccmvlrtotal_EF=0;
	if(document.getElementById('ccmvlrtotal_EF')) {
		if(document.getElementById('ccmvlrtotal_EF').value!='') {
			ccmvlrtotal_EF 	= parseFloat(replaceAll(replaceAll(document.getElementById('ccmvlrtotal_EF').value,".",""),",","."));	
		}
	}
	var crevlrtotal_EQ=0;
	if(document.getElementById('crevlrtotal_EQ')) {
		if(document.getElementById('crevlrtotal_EQ').value!='') {
			crevlrtotal_EQ 	= parseFloat(replaceAll(replaceAll(document.getElementById('crevlrtotal_EQ').value,".",""),",","."));	
		}
	}
	var ccmvlrtotal_EQ=0;
	if(document.getElementById('ccmvlrtotal_EQ')) {
		if(document.getElementById('ccmvlrtotal_EQ').value!='') {
			ccmvlrtotal_EQ 	= parseFloat(replaceAll(replaceAll(document.getElementById('ccmvlrtotal_EQ').value,".",""),",","."));	
		}
	}
	var crevlrtotal_EP=0;
	if(document.getElementById('crevlrtotal_EP')) {
		if(document.getElementById('crevlrtotal_EP').value!='') {
			crevlrtotal_EP 	= parseFloat(replaceAll(replaceAll(document.getElementById('crevlrtotal_EP').value,".",""),",","."));	
		}
	}
	var ccmvlrtotal_EP=0;
	if(document.getElementById('ccmvlrtotal_EP')) {
		if(document.getElementById('ccmvlrtotal_EP').value!='') {
			ccmvlrtotal_EP 	= parseFloat(replaceAll(replaceAll(document.getElementById('ccmvlrtotal_EP').value,".",""),",","."));	
		}
	}
	var crevlrtotal_EM=0;
	if(document.getElementById('crevlrtotal_EM')) {
		if(document.getElementById('crevlrtotal_EM').value!='') {
			crevlrtotal_EM 	= parseFloat(replaceAll(replaceAll(document.getElementById('crevlrtotal_EM').value,".",""),",","."));	
		}
	}
	var crevlrtotal_ET=0;
	if(document.getElementById('crevlrtotal_ET')) {
		if(document.getElementById('crevlrtotal_ET').value!='') {
			crevlrtotal_ET 	= parseFloat(replaceAll(replaceAll(document.getElementById('crevlrtotal_ET').value,".",""),",","."));	
		}
	}
        var coavlrtotal_AP = 0, coavlrtotal_PP = 0;
        <?php // -- Campos referentes apenas ao público da unidade prisional
        if (2 == $_SESSION['projovemurbano']['tprid']): ?>
        if (document.getElementById('coavlrtotal_AP')) {
            if ('' != document.getElementById('coavlrtotal_AP').value) {
                coavlrtotal_AP = parseFloat(replaceAll(replaceAll(document.getElementById('coavlrtotal_AP').value, '.', ''), ',', '.'));
            }
        }
        if (document.getElementById('coavlrtotal_PP')) {
            if ('' != document.getElementById('coavlrtotal_PP').value) {
                coavlrtotal_PP = parseFloat(replaceAll(replaceAll(document.getElementById('coavlrtotal_PP').value, '.', ''), ',', '.'));
            }
        }
        <?php endif; ?>

	var montante = parseFloat('<?=calcularMontante($meta) ?>');

	var total_efet = ccmvlrtotal_EF+ccmvlrtotal_EQ+ccmvlrtotal_EP+coavlrtotal_M;
	var total_cont = parseFloat(crevlrtotal_EF+crevlrtotal_EQ+crevlrtotal_EP+crevlrtotal_EM+crevlrtotal_ET+coavlrtotal_AP+coavlrtotal_PP);
	<? if( $coordenadorgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] || $coordenadorgeral['tprid'] == ''  ){ ?>
		total_efet = total_efet+cgevlrtotal+ccmvlrtotal;
		total_cont = total_cont+coavlrtotal_A+coavlrtotal_P+coavlrtotal_AP+coavlrtotal_PP+crevlrtotal_A+crevlrtotal_P;
	<? } ?>
	
	var total = parseFloat(total_efet)+parseFloat(total_cont);
	var totalFinal = 0;
	if( total > 0 ){
		totalFinal = (total*100)/montante;
	}
	var final = totalFinal;
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('totalUtilizado').value = total.toFixed(2);
	document.getElementById('propercutilizado').value = final.toFixed(1);

}

function verificaOpcoesCoordGeral(valor) {

	if(valor=='<?=OPP_EFETIVO_COMPLEMENTACAO ?>') {
		document.getElementById('coordenadorgeral_complementacao').style.display='';
	} else {
		document.getElementById('coordenadorgeral_complementacao').style.display='none';
		document.getElementById('cgehorpagrec').value='';
		document.getElementById('cgevlrhora').value='';
		document.getElementById('cgevlrcomp').value='';
		document.getElementById('cgeencargos').value='';
		document.getElementById('cgevlrtotal').value='';
		calculaPorcentTotal();
	}

}

function verificaOpcoesAssistenteCoordGeralA(valor) {

	if(valor=='<?=OPP_CONTRATO_RECURSOS_PROGRAMA_A ?>') {
		document.getElementById('assistente_A_complementacao').style.display='';
	} else {
		document.getElementById('assistente_A_complementacao').style.display='none';
		document.getElementById('coavlrbrutorem_A').value='';
		document.getElementById('coaencargos_A').value='';
		document.getElementById('coavlrtotal_A').value='';
		calculaPorcentTotal();
	}

}

function verificaOpcoesAssistenteCoordGeralP(valor) {

	if(valor=='<?=OPP_CONTRATO_RECURSOS_PROGRAMA_P ?>') {
		document.getElementById('assistente_P_complementacao').style.display='';
	} else {
		document.getElementById('assistente_P_complementacao').style.display='none';
		document.getElementById('coavlrbrutorem_P').value='';
		document.getElementById('coaencargos_P').value='';
		document.getElementById('coavlrtotal_P').value='';
		calculaPorcentTotal();
	}

}

function verificaOpcoesAssistenteCoordGeralM(valor) {

	if(valor=='7') {
		document.getElementById('assistente_M_complementacao').style.display='';
	} else {
		document.getElementById('assistente_M_complementacao').style.display='none';
		document.getElementById('coavlrbrutorem_M').value='';
		document.getElementById('coaencargos_M').value='';
		document.getElementById('coavlrtotal_M').value='';
		calculaPorcentTotal();
	}

}

function verificaOpcoesAssistenteCoordGeralAP(valor) {

	if(valor=='<?=OPP_CONTRATO_RECURSOS_PROGRAMA_A ?>') {
		document.getElementById('assistente_AP_complementacao').style.display='';
	} else {
		document.getElementById('assistente_AP_complementacao').style.display='none';
		document.getElementById('coavlrbrutorem_AP').value='';
		document.getElementById('coaencargos_AP').value='';
		document.getElementById('coavlrtotal_AP').value='';
		calculaPorcentTotal();
	}

}

function verificaOpcoesAssistenteCoordGeralPP(valor) {

	if(valor=='<?=OPP_CONTRATO_RECURSOS_PROGRAMA_P ?>') {
		document.getElementById('assistente_PP_complementacao').style.display='';
	} else {
		document.getElementById('assistente_PP_complementacao').style.display='none';
		document.getElementById('coavlrbrutorem_PP').value='';
		document.getElementById('coaencargos_PP').value='';
		document.getElementById('coavlrtotal_PP').value='';
		calculaPorcentTotal();
	}

}

<? if($esdid != ESD_EMELABORACAO && !$db->testa_superuser()) :  ?>
<? $desabilitado = true; ?>
jQuery(document).ready(function() {
	jQuery("[name^='coordgeral[']").attr("disabled","disabled");
	jQuery("[name^='coordgeral[']").attr("className","disabled");
	jQuery("[name^='coordassistentes[']").attr("disabled","disabled");
	jQuery("[name^='coordassistentes[']").attr("className","disabled");
	jQuery("[name^='diretorpolo[']").attr("disabled","disabled");
	jQuery("[name^='diretorpolo[']").attr("className","disabled");
	jQuery("[name^='dirassistentes[']").attr("disabled","disabled");
	jQuery("[name^='dirassistentes[']").attr("className","disabled");
	jQuery("[name^='educadores[']").attr("disabled","disabled");
	jQuery("[name^='educadores[']").attr("className","disabled");
	jQuery("[name^='contratadocomp[']").attr("disabled","disabled");
	jQuery("[name^='contratadocomp[']").attr("className","disabled");
	jQuery("[name^='contratadorecurso[']").attr("disabled","disabled");
	jQuery("[name^='contratadorecurso[']").attr("className","disabled");
	
	jQuery("[name^='pronuminstlegal']").attr("disabled","disabled");
	jQuery("[name^='pronuminstlegal']").attr("className","disabled");

	jQuery("[name^='prodatainstlegal']").attr("disabled","disabled");
	jQuery("[name^='prodatainstlegal']").attr("className","disabled");
});
<? endif; ?>

<? if( $coordenadorgeral['tprid'] != $_SESSION['projovemurbano']['tprid'] && $coordenadorgeral['tprid'] != '' ) :  ?>
jQuery(document).ready(function() {
	jQuery("[name^='coordgeral[']").attr("disabled","disabled");
	jQuery("[name^='coordassistentes[a]']").attr("disabled","disabled");
	jQuery("[name^='coordassistentes[p]']").attr("disabled","disabled");
	jQuery("[id$='_P']").attr("disabled","disabled");
	jQuery("[id$='_P']").addClass("disabled");
	jQuery("[id$='_A']").attr("disabled","disabled");
	jQuery("[id$='_A']").addClass("disabled");
	
	jQuery("#ccmqtd").attr("disabled","disabled");
	jQuery("#ccmqtd").addClass("disabled");
	jQuery("#ccmqtdhoras").attr("disabled","disabled");
	jQuery("#ccmqtdhoras").addClass("disabled");
	jQuery("#ccmvlrhora").attr("disabled","disabled");
	jQuery("#ccmvlrhora").addClass("disabled");
	jQuery("#ccmencargos").attr("disabled","disabled");
	jQuery("#ccmencargos").addClass("disabled");
});
<? endif; ?>
<?php if ( '2' != $_SESSION['projovemurbano']['tprid']): ?>
jQuery(document).ready(function(){
    jQuery('[id*=encargos]').each(function() {
        jQuery(this).keyup(function(){
            if (jQuery(this).val() > 70) {
                alert('O valor do campo "Encargos sociais e/ou trabalhistas" não pode ser maior que 70%.');
                jQuery(this).val('70.0');
            }
        });
    });
});
<?php endif; ?>

</script>
<form id="form" name="form" method="POST" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="gravarProfissionais" id="requisicao">
<input type="hidden" name="tprid_origem" value="<?=$coordenadorgeral['tprid'] ?>">
<input type="hidden" name="aba" value="<?=$_REQUEST['aba'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="15%">Orientações:</td>
		<td>
			<font color=blue>
				<?=$arrOrientacoes[4] ?>
			</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual da ação</td>
		<td>
		<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
				<td class="SubTituloCentro">Percentual Utilizado(%)</td>
			</tr>
			<tr>
				<td align="center">
					<?=campo_texto('profissionais[propercmax]', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="propercmax"', '', (($profissionais['propercmax'])?$profissionais['propercmax']:$propercmax), '' ); ?> 
					R$ 
					<span id="span_totalmaximo"><?=number_format(calcularMontante($meta)*(($profissionais['propercmax'])?$profissionais['propercmax']:$propercmax)/100,2,",",".") ?>
					</span>
					<input type="hidden" id="totalMax" value="<?=calcularMontante($meta)*(($profissionais['propercmax'])?$profissionais['propercmax']:$propercmax)/100; ?>">
				</td>
				<td align="center">
					<?=campo_texto('profissionais[propercutilizado]', 'N', 'N', 'Percentual Utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="propercutilizado"', '', (($profissionais['propercutilizado'])?$profissionais['propercutilizado']:'0.0'), '' ); ?> R$ 
					<span id="span_totalutilizado">
						<?=number_format($totalUtilizado,2,",",".") ?>
					</span>
					<input type="hidden" id="totalUtilizado" name="totalUtilizado" value="<?=$totalUtilizado ?>">
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<? if(($esdid == ESD_EMELABORACAO) && ($coordenadorgeral['tprid'] == $_SESSION['projovemurbano']['tprid']) && ($coordenadorgeral['tprid'] != '') ) { ?>
			<tr>
				<td class="SubTituloCentro" colspan="2" ><div style="color:red;cursor:pointer;" class="zerarCoord">Zerar Coordenação e Direção</div></td>
			</tr>
	<?php } ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">Coordenação Local</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Coordenador Geral</td>
		<td>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('coordgeral[cgeqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="cgeqtd"', '', (($coordenadorgeral['cgeqtd'])?$coordenadorgeral['cgeqtd']:'1'), '' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td>
					<?php $sql = "SELECT oppid as codigo, oppdesc as descricao 
									FROM projovemurbano.opcoesprofissionais 
									WHERE oppstatus='A' AND opptipo='C' AND oppid != 13
									ORDER BY 1";?>
					<? $db->monta_combo('coordgeral[oppid]', $sql, $habilita, 'Selecione', 'verificaOpcoesCoordGeral', '', '', '', 'N', '', '', $coordenadorgeral['oppid']); ?></td>
			</tr>
			
			<tr id="coordenadorgeral_complementacao" <?=(($coordenadorgeral['oppid']==OPP_EFETIVO_COMPLEMENTACAO)?"":"style=\"display:none;\"") ?> >
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">Número de horas pagas<br> com recursos do programa/mês</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>
					</tr>
					<tr>
						<td align="center"><? echo campo_texto('coordgeral[cgehorpagrec]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "##", "", '', '', 0, 'id="cgehorpagrec"', 'if(this.value>90){alert(\'Não pode ser maior que 90\');this.value=\'90\'}', $coordenadorgeral['cgehorpagrec'], 'calculaValorPorMes(\'cgevlrhora\',\'cgehorpagrec\',\'cgevlrcomp\');calculaValorTotal(\'cgevlrcomp\',\'cgeqtdmeses\',\'cgeencargos\',\'cgevlrtotal\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('coordgeral[cgevlrhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="cgevlrhora"', '', (($coordenadorgeral['cgevlrhora'])?number_format($coordenadorgeral['cgevlrhora'],2,",","."):''), 'calculaValorPorMes(\'cgevlrhora\',\'cgehorpagrec\',\'cgevlrcomp\');calculaValorTotal(\'cgevlrcomp\',\'cgeqtdmeses\',\'cgeencargos\',\'cgevlrtotal\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('coordgeral[cgevlrcomp]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="cgevlrcomp"', '', (($coordenadorgeral['cgevlrcomp'])?number_format($coordenadorgeral['cgevlrcomp'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('coordgeral[cgeqtdmeses]', 'N', $habilita, 'Meses', 8, 7, "##", "", '', '', 0, 'id="cgeqtdmeses"', '', (($coordenadorgeral['cgeqtdmeses'])?$coordenadorgeral['cgeqtdmeses']:'22'), 'calculaValorTotal(\'cgevlrcomp\',\'cgeqtdmeses\',\'cgeencargos\',\'cgevlrtotal\');calculaPorcentTotal();' ); ?></td>						
						<td align="center"><? echo campo_texto('coordgeral[cgeencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.#", "", '', '', 0, 'id="cgeencargos"', null, $coordenadorgeral['cgeencargos'], 'calculaValorTotal(\'cgevlrcomp\',\'cgeqtdmeses\',\'cgeencargos\',\'cgevlrtotal\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('coordgeral[cgevlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="cgevlrtotal"', '', (($coordenadorgeral['cgevlrtotal'])?number_format($coordenadorgeral['cgevlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Assistente Administrativo</td>
		<td>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td><? echo campo_texto('coordassistentes[A][coaqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="coaqtd_A"', '', (($assistenteadministrativo_A['coaqtd'])?$assistenteadministrativo_A['coaqtd']:'1'), '' ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td><? $db->monta_combo('coordassistentes[A][oppid]', "SELECT oppid as codigo, oppdesc as descricao FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND opptipo='A'AND oppid !=16", $habilita, 'Selecione', 'verificaOpcoesAssistenteCoordGeralA', '', '', '', 'N', '', '', $assistenteadministrativo_A['oppid']); ?></td>
				</tr>
				<tr id="assistente_A_complementacao" <?=(($assistenteadministrativo_A['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_A)?"":"style=\"display:none;\"") ?>>
					<td colspan="2">
						<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
							<tr>
								<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
								<td class="SubTituloCentro">Meses</td>
								<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
								<td class="SubTituloCentro">Valor Total</td>
							</tr>
							<tr>
								<td align="center"><? echo campo_texto('coordassistentes[A][coavlrbrutorem]', 'N', $habilita, 'Valor bruto ad remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrbrutorem_A"', '', (($assistenteadministrativo_A['coavlrbrutorem'])?number_format($assistenteadministrativo_A['coavlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'coavlrbrutorem_A\',\'coaqtdmeses_A\',\'coaencargos_A\',\'coavlrtotal_A\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[A][coaqtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="coaqtdmeses_A"', '', (($assistenteadministrativo_A['coaqtdmeses'])?$assistenteadministrativo_A['coaqtdmeses']:'22'), 'calculaValorTotal(\'coavlrbrutorem_A\',\'coaqtdmeses_A\',\'coaencargos_A\',\'coavlrtotal_A\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[A][coaencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.#", "", '', '', 0, 'id="coaencargos_A"', null, $assistenteadministrativo_A['coaencargos'], 'calculaValorTotal(\'coavlrbrutorem_A\',\'coaqtdmeses_A\',\'coaencargos_A\',\'coavlrtotal_A\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[A][coavlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrtotal_A"', '', (($assistenteadministrativo_A['coavlrtotal'])?number_format($assistenteadministrativo_A['coavlrtotal'],2,",","."):''), '' ); ?></td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Assistente Pedagógico</td>
		<td>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td><? echo campo_texto('coordassistentes[P][coaqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="coaqtd_P"', '', (($assistenteadministrativo_P['coaqtd'])?$assistenteadministrativo_P['coaqtd']:'1'), '' ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td><? $db->monta_combo('coordassistentes[P][oppid]', "SELECT oppid as codigo, oppdesc as descricao FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND opptipo='P' AND oppid !=14 ", $habilita, 'Selecione', 'verificaOpcoesAssistenteCoordGeralP', '', '', '', 'N', '', '', $assistenteadministrativo_P['oppid']); ?></td>
				</tr>
				<tr id="assistente_P_complementacao" <?=(($assistenteadministrativo_P['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_P)?"":"style=\"display:none;\"") ?> >
					<td colspan="2">
						<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
							<tr>
								<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
								<td class="SubTituloCentro">Meses</td>
								<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
								<td class="SubTituloCentro">Valor Total (R$)</td>
							</tr>
							<tr>
								<td align="center"><? echo campo_texto('coordassistentes[P][coavlrbrutorem]', 'N', $habilita, 'Valor bruto ad remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrbrutorem_P"', '', (($assistenteadministrativo_P['coavlrbrutorem'])?number_format($assistenteadministrativo_P['coavlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'coavlrbrutorem_P\',\'coaqtdmeses_P\',\'coaencargos_P\',\'coavlrtotal_P\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[P][coaqtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="coaqtdmeses_P"', '', (($assistenteadministrativo_P['coaqtdmeses'])?$assistenteadministrativo_P['coaqtdmeses']:'22'), 'calculaValorTotal(\'coavlrbrutorem_P\',\'coaqtdmeses_P\',\'coaencargos_P\',\'coavlrtotal_P\');calculaPorcentTotal();'  ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[P][coaencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.#", "", '', '', 0, 'id="coaencargos_P"', null, $assistenteadministrativo_P['coaencargos'], 'calculaValorTotal(\'coavlrbrutorem_P\',\'coaqtdmeses_P\',\'coaencargos_P\',\'coavlrtotal_P\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[P][coavlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrtotal_P"', '', (($assistenteadministrativo_P['coavlrtotal'])?number_format($assistenteadministrativo_P['coavlrtotal'],2,",","."):''), '' ); ?></td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Profissionais de Apoio</td>
	</tr>
	<? if( $_SESSION['projovemurbano']['tprid'] == 2 ){ ?>
	<tr>
		<td class="SubTituloDireita">Assistente Administrativo para Unidade Prisional</td>
		<td>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td><? echo campo_texto('coordassistentes[AP][coaqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="coaqtd_AP"', '', (($assistenteadministrativo_AP['coaqtd'])?$assistenteadministrativo_AP['coaqtd']:'1'), '' ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td><? $db->monta_combo('coordassistentes[AP][oppid]', "SELECT oppid as codigo, oppdesc as descricao FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND opptipo='A'AND oppid !=16", $habilita, 'Selecione', 'verificaOpcoesAssistenteCoordGeralAP', '', '', '', 'N', '', '', $assistenteadministrativo_AP['oppid']); ?></td>
				</tr>
				<tr id="assistente_AP_complementacao" <?=(($assistenteadministrativo_AP['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_A)?"":"style=\"display:none;\"") ?>>
					<td colspan="2">
						<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
							<tr>
								<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
								<td class="SubTituloCentro">Meses</td>
								<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
								<td class="SubTituloCentro">Valor Total</td>
							</tr>
							<tr>
								<td align="center"><? echo campo_texto('coordassistentes[AP][coavlrbrutorem]', 'N', $habilita, 'Valor bruto ad remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrbrutorem_AP"', '', (($assistenteadministrativo_AP['coavlrbrutorem'])?number_format($assistenteadministrativo_AP['coavlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'coavlrbrutorem_AP\',\'coaqtdmeses_AP\',\'coaencargos_AP\',\'coavlrtotal_AP\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[AP][coaqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="coaqtdmeses_AP"', '', (($assistenteadministrativo_AP['coaqtdmeses'])?$assistenteadministrativo_AP['coaqtdmeses']:'22'), '' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[AP][coaencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.#", "", '', '', 0, 'id="coaencargos_AP"', null, $assistenteadministrativo_AP['coaencargos'], 'calculaValorTotal(\'coavlrbrutorem_AP\',\'coaqtdmeses_AP\',\'coaencargos_AP\',\'coavlrtotal_AP\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[AP][coavlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrtotal_AP"', '', (($assistenteadministrativo_AP['coavlrtotal'])?number_format($assistenteadministrativo_AP['coavlrtotal'],2,",","."):''), '' ); ?></td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Assistente Pedagógico para Unidade Prisional</td>
		<td>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td><? echo campo_texto('coordassistentes[PP][coaqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="coaqtd_PP"', '', (($assistenteadministrativo_PP['coaqtd'])?$assistenteadministrativo_PP['coaqtd']:'1'), '' ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td><? $db->monta_combo('coordassistentes[PP][oppid]', "SELECT oppid as codigo, oppdesc as descricao FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND opptipo='P' AND oppid !=14", $habilita, 'Selecione', 'verificaOpcoesAssistenteCoordGeralPP', '', '', '', 'N', '', '', $assistenteadministrativo_PP['oppid']); ?></td>
				</tr>
				<tr id="assistente_PP_complementacao" <?=(($assistenteadministrativo_PP['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_P)?"":"style=\"display:none;\"") ?> >
					<td colspan="2">
						<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
							<tr>
								<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
								<td class="SubTituloCentro">Meses</td>
								<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
								<td class="SubTituloCentro">Valor Total (R$)</td>
							</tr>
							<tr>
								<td align="center"><? echo campo_texto('coordassistentes[PP][coavlrbrutorem]', 'N', $habilita, 'Valor bruto ad remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrbrutorem_PP"', '', (($assistenteadministrativo_PP['coavlrbrutorem'])?number_format($assistenteadministrativo_PP['coavlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'coavlrbrutorem_PP\',\'coaqtdmeses_PP\',\'coaencargos_PP\',\'coavlrtotal_PP\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[PP][coaqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="coaqtdmeses_PP"', '', (($assistenteadministrativo_PP['coaqtdmeses'])?$assistenteadministrativo_PP['coaqtdmeses']:'22'), '' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[PP][coaencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.#", "", '', '', 0, 'id="coaencargos_PP"', null, $assistenteadministrativo_PP['coaencargos'], 'calculaValorTotal(\'coavlrbrutorem_PP\',\'coaqtdmeses_PP\',\'coaencargos_PP\',\'coavlrtotal_PP\');calculaPorcentTotal();' ); ?></td>
								<td align="center"><? echo campo_texto('coordassistentes[PP][coavlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrtotal_PP"', '', (($assistenteadministrativo_PP['coavlrtotal'])?number_format($assistenteadministrativo_PP['coavlrtotal'],2,",","."):''), '' ); ?></td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<? }?>
	<tr>
    	<td class="SubTituloDireita">Pessoal de Apoio para a Etapa de Matrícula</td>
       	<td>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td>
					<?php
                        $arrOpt = Array();
                        if( $numeronucleos > 0 ){
                            for( $x = $numeronucleos; $x <= (2*$numeronucleos); $x++ ){
                                $arrOpt[] = Array('codigo'=>$x,'descricao'=>$x);
                            }
                        }
                        $db->monta_combo('coordassistentes[M][coaqtd]', $arrOpt, $habilita, 'Selecione', 'calculaValorTotal(\'coavlrbrutorem_M\',\'coaqtdmeses_M\',\'coaencargos_M\',\'coavlrtotal_M\',\'coordassistentesqtd_M\');calculaPorcentTotal();', '', '', '', 'N', 'coordassistentesqtd_M', '', $assistenteadministrativo_M['coaqtd']); 
                    
                    ?>
					</td>
           		</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td>
					<? 
					
					$sql = "SELECT 
								oppid as codigo,
								oppdesc as descricao
							FROM 
								projovemurbano.opcoesprofissionais
							WHERE 
								oppstatus='A'
								AND opptipo='A'
								AND oppid != 8
								AND oppid !=16";
					
					$db->monta_combo('coordassistentes[M][oppid]', $sql, $habilita, 'Selecione', 'verificaOpcoesAssistenteCoordGeralM', '', '', '', 'N', '', '', $assistenteadministrativo_M['oppid']); 
					?>
					</td>
             	</tr>
				<tr id="assistente_M_complementacao" <?=(($assistenteadministrativo_M['oppid']==7)?"":"style=\"display:none;\"") ?>>
					<td colspan="2">
					<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
							<td class="SubTituloCentro">Meses</td>
							<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
							<td class="SubTituloCentro">Valor Total</td>
						</tr>
						<tr>
							<td align="center"><? echo campo_texto('coordassistentes[M][coavlrbrutorem]', 'N', $habilita, 'Valor bruto ad remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrbrutorem_M"', '', (($assistenteadministrativo_M['coavlrbrutorem'])?number_format($assistenteadministrativo_M['coavlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'coavlrbrutorem_M\',\'coaqtdmeses_M\',\'coaencargos_M\',\'coavlrtotal_M\');calculaPorcentTotal();' ); ?></td>
							<td align="center"><? echo campo_texto('coordassistentes[M][coaqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="coaqtdmeses_M"', '', 3, '' ); ?></td>
							<td align="center"><? echo campo_texto('coordassistentes[M][coaencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.#", "", '', '', 0, 'id="coaencargos_M"', null, $assistenteadministrativo_M['coaencargos'], 'calculaValorTotal(\'coavlrbrutorem_M\',\'coaqtdmeses_M\',\'coaencargos_M\',\'coavlrtotal_M\',\'coordassistentesqtd_M\');calculaPorcentTotal();' ); ?></td>
							<td align="center"><? echo campo_texto('coordassistentes[M][coavlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="coavlrtotal_M"', '', (($assistenteadministrativo_M['coavlrtotal'])?number_format($assistenteadministrativo_M['coavlrtotal'],2,",","."):''), '' ); ?></td>
						</tr>
					</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Direção de polo</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Diretor de polo</td>
		<td>
		<? if($numeropolos>0) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('diretorpolo[dipqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="dipqtd"', '', $numeropolos, '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 40 horas</td>
						<td><? echo campo_texto('diretorpolo[dipeqtdefetivo40hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="dipeqtdefetivo40hr"', '', $diretorpolo['dipeqtdefetivo40hr'], 'validaQuantidadeCargos(\'dipeqtdefetivo40hr\',\'dipqtdrecursoproprio\',\'ccmqtd\',\'\',\'dipqtd\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('diretorpolo[dipqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="dipqtdrecursoproprio"', '', $diretorpolo['dipqtdrecursoproprio'], 'validaQuantidadeCargos(\'dipeqtdefetivo40hr\',\'dipqtdrecursoproprio\',\'ccmqtd\',\'\',\'dipqtd\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de diretor de polo</td>
						<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
						<td align="center"><? echo campo_texto('contratadocomp[DP][ccmqtd]', 'N', $habilita, 'Número de diretor de polo', 8, 9, "#######", "", '', '', 0, 'id="ccmqtd"', '', $diretorpolo['ccmqtd'], 'validaQuantidadeCargos(\'dipeqtdefetivo40hr\',\'dipqtdrecursoproprio\',\'ccmqtd\',\'\',\'dipqtd\');calculaValorTotal(\'ccmvlrcomp\',\'ccmqtdmeses\',\'ccmencargos\',\'ccmvlrtotal\',\'ccmqtd\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[DP][ccmqtdhoras]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "##", "", '', '', 0, 'id="ccmqtdhoras"', 'if(this.value>90){alert(\'Não pode ser maior que 90\');this.value=\'90\'}', $diretorpolo['ccmqtdhoras'], 'calculaValorPorMes(\'ccmvlrhora\',\'ccmqtdhoras\',\'ccmvlrcomp\');calculaValorTotal(\'ccmvlrcomp\',\'ccmqtdmeses\',\'ccmencargos\',\'ccmvlrtotal\',\'ccmqtd\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[DP][ccmvlrhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrhora"', '', (($diretorpolo['ccmvlrhora'])?number_format($diretorpolo['ccmvlrhora'],2,",","."):''), 'calculaValorPorMes(\'ccmvlrhora\',\'ccmqtdhoras\',\'ccmvlrcomp\');calculaValorTotal(\'ccmvlrcomp\',\'ccmqtdmeses\',\'ccmencargos\',\'ccmvlrtotal\',\'ccmqtd\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[DP][ccmvlrcomp]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrcomp"', '', (($diretorpolo['ccmvlrcomp'])?number_format($diretorpolo['ccmvlrcomp'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[DP][ccmqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="ccmqtdmeses"', '', (($diretorpolo['ccmqtdmeses'])?$diretorpolo['ccmqtdmeses']:'22'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[DP][ccmencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.#", "", '', '', 0, 'id="ccmencargos"', null, $diretorpolo['ccmencargos'], 'calculaValorTotal(\'ccmvlrcomp\',\'ccmqtdmeses\',\'ccmencargos\',\'ccmvlrtotal\',\'ccmqtd\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[DP][ccmvlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrtotal"', '', (($diretorpolo['ccmvlrtotal'])?number_format($diretorpolo['ccmvlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem polos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Assistente Administrativo</td>
		<td>
		<? if($numeropolos>0) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('dirassistentes[A][dasqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="dasqtd_AA"', '', $numeropolos, '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 40 horas</td>
						<td><? echo campo_texto('dirassistentes[A][dasqtdefetivo40hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="dasqtdefetivo40hr_AA"', '', $dirassistentes_A['dasqtdefetivo40hr'], 'validaQuantidadeCargos(\'dasqtdefetivo40hr_AA\',\'dasqtdrecursoproprio_AA\',\'creqtd_A\',\'\',\'dasqtd_AA\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('dirassistentes[A][dasqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="dasqtdrecursoproprio_AA"', '', $dirassistentes_A['dasqtdrecursoproprio'], 'validaQuantidadeCargos(\'dasqtdefetivo40hr_AA\',\'dasqtdrecursoproprio_AA\',\'creqtd_A\',\'\',\'dasqtd_AA\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de assistentes</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[AA][creqtd]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="creqtd_A"', '', $dirassistentes_A['creqtd'], 'validaQuantidadeCargos(\'dasqtdefetivo40hr_AA\',\'dasqtdrecursoproprio_AA\',\'creqtd_A\',\'\',\'dasqtd_AA\');calculaValorTotal(\'crevlrbrutorem_A\',\'creqtdmeses_A\',\'creencargos_A\',\'crevlrtotal_A\',\'creqtd_A\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[AA][crevlrbrutorem]', 'N', 'S', 'Valor bruto da remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrbrutorem_A"', '', (($dirassistentes_A['crevlrbrutorem'])?number_format($dirassistentes_A['crevlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'crevlrbrutorem_A\',\'creqtdmeses_A\',\'creencargos_A\',\'crevlrtotal_A\',\'creqtd_A\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[AA][creqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="creqtdmeses_A"', '', (($dirassistentes_A['creqtdmeses'])?$dirassistentes_A['creqtdmeses']:'22'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[AA][creencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "##.#", "", '', '', 0, 'id="creencargos_A"', null, $dirassistentes_A['creencargos'], 'calculaValorTotal(\'crevlrbrutorem_A\',\'creqtdmeses_A\',\'creencargos_A\',\'crevlrtotal_A\',\'creqtd_A\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[AA][crevlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrtotal_A"', '', (($dirassistentes_A['crevlrtotal'])?number_format($dirassistentes_A['crevlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem polos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Assistente Pedagógico</td>
		<td>
		<? if($numeropolos>0) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('dirassistentes[P][dasqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="dasqtd_AP"', '', $numeropolos, '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 40 horas</td>
						<td><? echo campo_texto('dirassistentes[P][dasqtdefetivo40hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="dasqtdefetivo40hr_AP"', '', $dirassistentes_P['dasqtdefetivo40hr'], 'validaQuantidadeCargos(\'dasqtdefetivo40hr_AP\',\'dasqtdrecursoproprio_AP\',\'creqtd_P\',\'\',\'dasqtd_AP\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('dirassistentes[P][dasqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="dasqtdrecursoproprio_AP"', '', $dirassistentes_P['dasqtdrecursoproprio'], 'validaQuantidadeCargos(\'dasqtdefetivo40hr_AP\',\'dasqtdrecursoproprio_AP\',\'creqtd_P\',\'\',\'dasqtd_AP\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de assistentes</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[AP][creqtd]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="creqtd_P"', '', $dirassistentes_P['creqtd'], 'validaQuantidadeCargos(\'dasqtdefetivo40hr_AP\',\'dasqtdrecursoproprio_AP\',\'creqtd_P\',\'\',\'dasqtd_AP\');calculaValorTotal(\'crevlrbrutorem_P\',\'creqtdmeses_P\',\'creencargos_P\',\'crevlrtotal_P\',\'creqtd_P\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[AP][crevlrbrutorem]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrbrutorem_P"', '', (($dirassistentes_P['crevlrbrutorem'])?number_format($dirassistentes_P['crevlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'crevlrbrutorem_P\',\'creqtdmeses_P\',\'creencargos_P\',\'crevlrtotal_P\',\'creqtd_P\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[AP][creqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="creqtdmeses_P"', '', (($dirassistentes_P['creqtdmeses'])?$dirassistentes_P['creqtdmeses']:'22'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[AP][creencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "##.#", "", '', '', 0, 'id="creencargos_P"', null, $dirassistentes_P['creencargos'], 'calculaValorTotal(\'crevlrbrutorem_P\',\'creqtdmeses_P\',\'creencargos_P\',\'crevlrtotal_P\',\'creqtd_P\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[AP][crevlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrtotal_P"', '', (($dirassistentes_P['crevlrtotal'])?number_format($dirassistentes_P['crevlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem polos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Educadores</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores do Ensino Fundamental</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[F][eduqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="eduqtd_EF"', '', ($numeronucleos*5), '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo campo_texto('educadores[F][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EF"', '', $educadores_F['eduefetivo30hr'], 'validaQuantidadeCargos(\'eduefetivo30hr_EF\',\'eduqtdrecursoproprio_EF\',\'creqtd_EF\',\'ccmqtd_EF\',\'eduqtd_EF\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[F][eduqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="eduqtdrecursoproprio_EF"', '', $educadores_F['eduqtdrecursoproprio'], 'validaQuantidadeCargos(\'eduefetivo30hr_EF\',\'eduqtdrecursoproprio_EF\',\'creqtd_EF\',\'ccmqtd_EF\',\'eduqtd_EF\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][creqtd]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="creqtd_EF"', '', $educadores_F['creqtd'], 'validaQuantidadeCargos(\'eduefetivo30hr_EF\',\'eduqtdrecursoproprio_EF\',\'creqtd_EF\',\'ccmqtd_EF\',\'eduqtd_EF\');calculaValorTotal(\'crevlrbrutorem_EF\',\'creqtdmeses_EF\',\'creencargos_EF\',\'crevlrtotal_EF\',\'creqtd_EF\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][crevlrbrutorem]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrbrutorem_EF"', '', (($educadores_F['crevlrbrutorem'])?number_format($educadores_F['crevlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'crevlrbrutorem_EF\',\'creqtdmeses_EF\',\'creencargos_EF\',\'crevlrtotal_EF\',\'creqtd_EF\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][creqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="creqtdmeses_EF"', '', (($educadores_F['creqtdmeses'])?$educadores_F['creqtdmeses']:'18'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][creencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 6, "####.#", "", '', '', 0, 'id="creencargos_EF"', null, $educadores_F['creencargos'], 'calculaValorTotal(\'crevlrbrutorem_EF\',\'creqtdmeses_EF\',\'creencargos_EF\',\'crevlrtotal_EF\',\'creqtd_EF\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][crevlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrtotal_EF"', '', (($educadores_F['crevlrtotal'])?number_format($educadores_F['crevlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês (Número de horas do total de educadores)</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais<br> e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][ccmqtd]', 'N', $habilita, 'Número de diretor de polo', 8, 9, "#######", "", '', '', 0, 'id="ccmqtd_EF"', '', $educadores_F['ccmqtd'], 'validaQuantidadeCargos(\'eduefetivo30hr_EF\',\'eduqtdrecursoproprio_EF\',\'creqtd_EF\',\'ccmqtd_EF\',\'eduqtd_EF\');calculaValorTotal(\'ccmvlrcomp_EF\',\'ccmqtdmeses_EF\',\'ccmencargos_EF\',\'ccmvlrtotal_EF\',\'ccmqtd_EF\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][ccmqtdhoras]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "########", "", '', '', 0, 'id="ccmqtdhoras_EF"', 'if(document.getElementById(\'ccmqtd_EF\').value==\'\'){alert(\'Preencha o número de educadores\');this.value=\'\';return false;}', $educadores_F['ccmqtdhoras'], 'calculaValorPorMes(\'ccmvlrhora_EF\',\'ccmqtdhoras_EF\',\'ccmvlrcomp_EF\');calculaValorTotal(\'ccmvlrcomp_EF\',\'ccmqtdmeses_EF\',\'ccmencargos_EF\',\'ccmvlrtotal_EF\',\'ccmqtd_EF\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][ccmvlrhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrhora_EF"', '', (($educadores_F['ccmvlrhora'])?number_format($educadores_F['ccmvlrhora'],2,",","."):''), 'calculaValorPorMes(\'ccmvlrhora_EF\',\'ccmqtdhoras_EF\',\'ccmvlrcomp_EF\');calculaValorTotal(\'ccmvlrcomp_EF\',\'ccmqtdmeses_EF\',\'ccmencargos_EF\',\'ccmvlrtotal_EF\',\'ccmqtd_EF\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][ccmvlrcomp]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrcomp_EF"', '', (($educadores_F['ccmvlrcomp'])?number_format($educadores_F['ccmvlrcomp'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][ccmqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="ccmqtdmeses_EF"', '', (($educadores_F['ccmqtdmeses'])?$educadores_F['ccmqtdmeses']:'18'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][ccmencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "####.#", "", '', '', 0, 'id="ccmencargos_EF"', null, $educadores_F['ccmencargos'], 'calculaValorTotal(\'ccmvlrcomp_EF\',\'ccmqtdmeses_EF\',\'ccmencargos_EF\',\'ccmvlrtotal_EF\',\'ccmqtd_EF\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][ccmvlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrtotal_EF"', '', (($educadores_F['ccmvlrtotal'])?number_format($educadores_F['ccmvlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores do Qualificação Profissional</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[Q][eduqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="eduqtd_EQ"', '', $numeronucleos, '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo campo_texto('educadores[Q][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EQ"', '', $educadores_Q['eduefetivo30hr'], 'validaQuantidadeCargos(\'eduefetivo30hr_EQ\',\'eduqtdrecursoproprio_EQ\',\'creqtd_EQ\',\'ccmqtd_EQ\',\'eduqtd_EQ\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[Q][eduqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="eduqtdrecursoproprio_EQ"', '', $educadores_Q['eduqtdrecursoproprio'], 'validaQuantidadeCargos(\'eduefetivo30hr_EQ\',\'eduqtdrecursoproprio_EQ\',\'creqtd_EQ\',\'ccmqtd_EQ\',\'eduqtd_EQ\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][creqtd]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="creqtd_EQ"', '', $educadores_Q['creqtd'], 'validaQuantidadeCargos(\'eduefetivo30hr_EQ\',\'eduqtdrecursoproprio_EQ\',\'creqtd_EQ\',\'ccmqtd_EQ\',\'eduqtd_EQ\');calculaValorTotal(\'crevlrbrutorem_EQ\',\'creqtdmeses_EQ\',\'creencargos_EQ\',\'crevlrtotal_EQ\',\'creqtd_EQ\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][crevlrbrutorem]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrbrutorem_EQ"', '', (($educadores_Q['crevlrbrutorem'])?number_format($educadores_Q['crevlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'crevlrbrutorem_EQ\',\'creqtdmeses_EQ\',\'creencargos_EQ\',\'crevlrtotal_EQ\',\'creqtd_EQ\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][creqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="creqtdmeses_EQ"', '', (($educadores_Q['creqtdmeses'])?$educadores_Q['creqtdmeses']:'18'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][creencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "####.#", "", '', '', 0, 'id="creencargos_EQ"', null, $educadores_Q['creencargos'], 'calculaValorTotal(\'crevlrbrutorem_EQ\',\'creqtdmeses_EQ\',\'creencargos_EQ\',\'crevlrtotal_EQ\',\'creqtd_EQ\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][crevlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrtotal_EQ"', '', (($educadores_Q['crevlrtotal'])?number_format($educadores_Q['crevlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês (por profissional)</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][ccmqtd]', 'N', $habilita, 'Número de diretor de polo', 8, 9, "#######", "", '', '', 0, 'id="ccmqtd_EQ"', '', $educadores_Q['ccmqtd'], 'validaQuantidadeCargos(\'eduefetivo30hr_EQ\',\'eduqtdrecursoproprio_EQ\',\'creqtd_EQ\',\'ccmqtd_EQ\',\'eduqtd_EQ\');calculaValorTotal(\'ccmvlrcomp_EQ\',\'ccmqtdmeses_EQ\',\'ccmencargos_EQ\',\'ccmvlrtotal_EQ\',\'ccmqtd_EQ\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][ccmqtdhoras]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "########", "", '', '', 0, 'id="ccmqtdhoras_EQ"', 'if(document.getElementById(\'ccmqtd_EQ\').value==\'\'){alert(\'Preencha o número de educadores\');this.value=\'\';return false;}', $educadores_Q['ccmqtdhoras'], 'calculaValorPorMes(\'ccmvlrhora_EQ\',\'ccmqtdhoras_EQ\',\'ccmvlrcomp_EQ\');calculaValorTotal(\'ccmvlrcomp_EQ\',\'ccmqtdmeses_EQ\',\'ccmencargos_EQ\',\'ccmvlrtotal_EQ\',\'ccmqtd_EQ\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][ccmvlrhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrhora_EQ"', '', (($educadores_Q['ccmvlrhora'])?number_format($educadores_Q['ccmvlrhora'],2,",","."):''), 'calculaValorPorMes(\'ccmvlrhora_EQ\',\'ccmqtdhoras_EQ\',\'ccmvlrcomp_EQ\');calculaValorTotal(\'ccmvlrcomp_EQ\',\'ccmqtdmeses_EQ\',\'ccmencargos_EQ\',\'ccmvlrtotal_EQ\',\'ccmqtd_EQ\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][ccmvlrcomp]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrcomp_EQ"', '', (($educadores_Q['ccmvlrcomp'])?number_format($educadores_Q['ccmvlrcomp'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][ccmqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="ccmqtdmeses_EQ"', '', (($educadores_Q['ccmqtdmeses'])?$educadores_Q['ccmqtdmeses']:'18'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][ccmencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "####.#", "", '', '', 0, 'id="ccmencargos_EQ"', null, $educadores_Q['ccmencargos'], 'calculaValorTotal(\'ccmvlrcomp_EQ\',\'ccmqtdmeses_EQ\',\'ccmencargos_EQ\',\'ccmvlrtotal_EQ\',\'ccmqtd_EQ\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][ccmvlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrtotal_EQ"', '', (($educadores_Q['ccmvlrtotal'])?number_format($educadores_Q['ccmvlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[P][eduqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="eduqtd_EP"', '', $numeronucleos, '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo campo_texto('educadores[P][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EP"', '', $educadores_P['eduefetivo30hr'], 'validaQuantidadeCargos(\'eduefetivo30hr_EP\',\'eduqtdrecursoproprio_EP\',\'creqtd_EP\',\'ccmqtd_EP\',\'eduqtd_EP\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[P][eduqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="eduqtdrecursoproprio_EP"', '', $educadores_P['eduqtdrecursoproprio'], 'validaQuantidadeCargos(\'eduefetivo30hr_EP\',\'eduqtdrecursoproprio_EP\',\'creqtd_EP\',\'ccmqtd_EP\',\'eduqtd_EP\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][creqtd]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="creqtd_EP"', '', $educadores_P['creqtd'], 'validaQuantidadeCargos(\'eduefetivo30hr_EP\',\'eduqtdrecursoproprio_EP\',\'creqtd_EP\',\'ccmqtd_EP\',\'eduqtd_EP\');calculaValorTotal(\'crevlrbrutorem_EP\',\'creqtdmeses_EP\',\'creencargos_EP\',\'crevlrtotal_EP\',\'creqtd_EP\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][crevlrbrutorem]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrbrutorem_EP"', '', (($educadores_P['crevlrbrutorem'])?number_format($educadores_P['crevlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'crevlrbrutorem_EP\',\'creqtdmeses_EP\',\'creencargos_EP\',\'crevlrtotal_EP\',\'creqtd_EP\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][creqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="creqtdmeses_EP"', '', (($educadores_P['creqtdmeses'])?$educadores_P['creqtdmeses']:'18'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][creencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "####.#", "", '', '', 0, 'id="creencargos_EP"', null, $educadores_P['creencargos'], 'calculaValorTotal(\'crevlrbrutorem_EP\',\'creqtdmeses_EP\',\'creencargos_EP\',\'crevlrtotal_EP\',\'creqtd_EP\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][crevlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrtotal_EP"', '', (($educadores_P['crevlrtotal'])?number_format($educadores_P['crevlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês (por profissional)</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][ccmqtd]', 'N', $habilita, 'Número de educadores', 8, 9, "#######", "", '', '', 0, 'id="ccmqtd_EP"', '', $educadores_P['ccmqtd'], 'validaQuantidadeCargos(\'eduefetivo30hr_EP\',\'eduqtdrecursoproprio_EP\',\'creqtd_EP\',\'ccmqtd_EP\',\'eduqtd_EP\');calculaValorTotal(\'ccmvlrcomp_EP\',\'ccmqtdmeses_EP\',\'ccmencargos_EP\',\'ccmvlrtotal_EP\',\'ccmqtd_EP\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][ccmqtdhoras]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "########", "", '', '', 0, 'id="ccmqtdhoras_EP"', 'if(document.getElementById(\'ccmqtd_EP\').value==\'\'){alert(\'Preencha o número de educadores\');this.value=\'\';return false;}', $educadores_P['ccmqtdhoras'], 'calculaValorPorMes(\'ccmvlrhora_EP\',\'ccmqtdhoras_EP\',\'ccmvlrcomp_EP\');calculaValorTotal(\'ccmvlrcomp_EP\',\'ccmqtdmeses_EP\',\'ccmencargos_EP\',\'ccmvlrtotal_EP\',\'ccmqtd_EP\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][ccmvlrhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrhora_EP"', '', (($educadores_P['ccmvlrhora'])?number_format($educadores_P['ccmvlrhora'],2,",","."):''), 'calculaValorPorMes(\'ccmvlrhora_EP\',\'ccmqtdhoras_EP\',\'ccmvlrcomp_EP\');calculaValorTotal(\'ccmvlrcomp_EP\',\'ccmqtdmeses_EP\',\'ccmencargos_EP\',\'ccmvlrtotal_EP\',\'ccmqtd_EP\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][ccmvlrcomp]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrcomp_EP"', '', (($educadores_P['ccmvlrcomp'])?number_format($educadores_P['ccmvlrcomp'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][ccmqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="ccmqtdmeses_EP"', '', (($educadores_P['ccmqtdmeses'])?$educadores_P['ccmqtdmeses']:'18'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][ccmencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "####.#", "", '', '', 0, 'id="ccmencargos_EP"', null, $educadores_P['ccmencargos'], 'calculaValorTotal(\'ccmvlrcomp_EP\',\'ccmqtdmeses_EP\',\'ccmencargos_EP\',\'ccmvlrtotal_EP\',\'ccmqtd_EP\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][ccmvlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="ccmvlrtotal_EP"', '', (($educadores_P['ccmvlrtotal'])?number_format($educadores_P['ccmvlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores para monitoramento do acolhimento ás crianças de 0 a 8 anos, filhas do jovens atendidos pelo programa</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[M][eduqtd]', 'N', $habilita, 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="eduqtd_EM"', 'if(this.value>'.($numeronucleos*2).'){alert(\'Não pode ser maior que '.($numeronucleos*2).'\');this.value=\''.($numeronucleos*2).'\'}', (($educadores_M['eduqtd'])?$educadores_M['eduqtd']:''), '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 20 horas</td>
						<td><? echo campo_texto('educadores[M][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EM"', '', $educadores_M['eduefetivo30hr'], 'validaQuantidadeCargos(\'eduefetivo30hr_EM\',\'eduqtdrecursoproprio_EM\',\'creqtd_EM\',\'\',\'eduqtd_EM\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[M][eduqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="eduqtdrecursoproprio_EM"', '', $educadores_M['eduqtdrecursoproprio'], 'validaQuantidadeCargos(\'eduefetivo30hr_EM\',\'eduqtdrecursoproprio_EM\',\'creqtd_EM\',\'\',\'eduqtd_EM\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[EM][creqtd]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="creqtd_EM"', '', $educadores_M['creqtd'], 'validaQuantidadeCargos(\'eduefetivo30hr_EM\',\'eduqtdrecursoproprio_EM\',\'creqtd_EM\',\'\',\'eduqtd_EM\');calculaValorTotal(\'crevlrbrutorem_EM\',\'creqtdmeses_EM\',\'creencargos_EM\',\'crevlrtotal_EM\',\'creqtd_EM\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EM][crevlrbrutorem]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrbrutorem_EM"', '', (($educadores_M['crevlrbrutorem'])?number_format($educadores_M['crevlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'crevlrbrutorem_EM\',\'creqtdmeses_EM\',\'creencargos_EM\',\'crevlrtotal_EM\',\'creqtd_EM\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EM][creqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="creqtdmeses_EM"', '', (($educadores_M['creqtdmeses'])?$educadores_M['creqtdmeses']:'18'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EM][creencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "####.#", "", '', '', 0, 'id="creencargos_EM"', null, $educadores_M['creencargos'], 'calculaValorTotal(\'crevlrbrutorem_EM\',\'creqtdmeses_EM\',\'creencargos_EM\',\'crevlrtotal_EM\',\'creqtd_EM\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EM][crevlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrtotal_EM"', '', (($educadores_M['crevlrtotal'])?number_format($educadores_M['crevlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tradutor de Intérprete de Libras</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[T][eduqtd]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="eduqtd_ET"', '', $numeronucleos, '' ); ?></td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo campo_texto('educadores[T][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_ET"', '', $educadores_T['eduefetivo30hr'], 'validaQuantidadeCargos(\'eduefetivo30hr_ET\',\'eduqtdrecursoproprio_ET\',\'creqtd_ET\',\'\',\'eduqtd_ET\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[T][eduqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="eduqtdrecursoproprio_ET"', '', $educadores_T['eduqtdrecursoproprio'], 'validaQuantidadeCargos(\'eduefetivo30hr_ET\',\'eduqtdrecursoproprio_ET\',\'creqtd_ET\',\'\',\'eduqtd_ET\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][creqtd]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="creqtd_ET"', '', $educadores_T['creqtd'], 'validaQuantidadeCargos(\'eduefetivo30hr_ET\',\'eduqtdrecursoproprio_ET\',\'creqtd_ET\',\'\',\'eduqtd_ET\');calculaValorTotal(\'crevlrbrutorem_ET\',\'creqtdmeses_ET\',\'creencargos_ET\',\'crevlrtotal_ET\',\'creqtd_ET\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][crevlrbrutorem]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrbrutorem_ET"', '', (($educadores_T['crevlrbrutorem'])?number_format($educadores_T['crevlrbrutorem'],2,",","."):''), 'calculaValorTotal(\'crevlrbrutorem_ET\',\'creqtdmeses_ET\',\'creencargos_ET\',\'crevlrtotal_ET\',\'creqtd_ET\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][creqtdmeses]', 'N', 'N', 'Meses', 8, 7, "########", "", '', '', 0, 'id="creqtdmeses_ET"', '', (($educadores_T['creqtdmeses'])?$educadores_T['creqtdmeses']:'18'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][creencargos]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "####.#", "", '', '', 0, 'id="creencargos_ET"', null, $educadores_T['creencargos'], 'calculaValorTotal(\'crevlrbrutorem_ET\',\'creqtdmeses_ET\',\'creencargos_ET\',\'crevlrtotal_ET\',\'creqtd_ET\');calculaPorcentTotal();' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][crevlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="crevlrtotal_ET"', '', (($educadores_T['crevlrtotal'])?number_format($educadores_T['crevlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educador para atendimento educacional especializado</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[E][eduqtd]', 'N', $habilita, 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="eduqtd_EE"', '', (($educadores_E['eduqtd'])?$educadores_E['eduqtd']:''), '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo</td>
						<td><? echo campo_texto('educadores[E][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EE"', '', $educadores_E['eduefetivo30hr'], 'validaQuantidadeCargos(\'eduefetivo30hr_EE\',\'eduqtdrecursoproprio_EE\',\'\',\'\',\'eduqtd_EE\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[E][eduqtdrecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="eduqtdrecursoproprio_EE"', '', $educadores_E['eduqtdrecursoproprio'], 'validaQuantidadeCargos(\'eduefetivo30hr_EE\',\'eduqtdrecursoproprio_EE\',\'\',\'\',\'eduqtd_EE\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
	
		</td>
	</tr>
	<tr>
            <?php // -- Verificando se existe um arquivo carregado para este público
            $sql = "SELECT 
            			arq.arqid, arq.arqnome, arq.arqextensao,
				       	inl.inlnuminstlegal, inl.inldatainstlegal,
				       	inl.tpdid
				  	FROM projovemurbano.instrumentolegal inl
				    INNER JOIN projovemurbano.anexogeral ang USING (angid)
				    INNER JOIN public.arquivo arq USING (arqid)
				  	WHERE 
            			proid = {$_SESSION['projovemurbano']['proid']}
					    AND tprid = {$_SESSION['projovemurbano']['tprid']}
					    AND ang.ppuid = {$_SESSION['projovemurbano']['ppuid']}";
           
            $infoInstLegal = $db->pegaLinha($sql); ?>
            <td class="SubTituloDireita">Instrumento legal</td>
            <td>
                <table>
                    <tr>
                        <td align="right">Número do instrumento</td>
			<td><? echo campo_texto('inlnuminstlegal', 'S', $habilita, 'Número do instrumento', 8, 7, "########", "", '', '', 0, 'id="inlnuminstlegal" required', '', $infoInstLegal['inlnuminstlegal'], '' ); ?></td>
                    </tr>
                    <tr>
			<td align="right">Data do instrumento</td>
			<td><? echo campo_texto('inldatainstlegal', 'S', $habilita, 'Data do instrumento', 11, 10, "##/##/####", "", '', '', 0, 'id="inldatainstlegal" required', '', formata_data($infoInstLegal['inldatainstlegal']), '' ); ?></td>
                    </tr>
                    <tr>
                        <td align="right">Tipo do instrumento</td>
                        <td><?php $sql = <<<DML
SELECT tdil.tpdid AS codigo,
       tdil.descricaotipo AS descricao
  FROM projovemurbano.tipodocumentoinstrumentolegal tdil
  ORDER BY descricao
DML;
                            $db->monta_combo('tpdid', $sql, 'S', 'Selecione um tipo', null, '', '', '', 'S', 'muncod', null, $infoInstLegal['tpdid'], null, 'id="tpdid"'); ?>
                        </td>
                    </tr>
                    <tr>
                        <td align="right">Arquivo</td>
                        <td>
                            <?php if (isset($infoInstLegal['arqid']) && !empty($infoInstLegal['arqid'])): ?>
                            <input type="hidden" name="arquivo" id="arquivo" value="uploaded" />
                            <?if($habilita=='S'){?>
                            	<img src="../imagens/excluir.gif" style=cursor:pointer; onclick="excluirAnexo('<?php echo $infoInstLegal['arqid']; ?>');" />&nbsp;
                            <?}?>
                            <a href="projovemurbano.php?modulo=principal/anexoGeral&acao=A&download=S&arqid=<?php echo $infoInstLegal['arqid']; ?>">
                                <?php echo "{$infoInstLegal['arqnome']}.{$infoInstLegal['arqextensao']}"; ?>
                            </a>
                            <?php else: ?>
                            <input type="file" name="arquivo" id="arquivo" /><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                            <?php endif; ?>
                        </td>
                    </tr>
		</table>
            </td>
	</tr>

	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" 
					onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_REQUEST['aba'] ?>&aba2=poloNucleo&aba3=poloNucleoResumo';"> 
		<? if(!$desabilitado) : ?>
			<input type="button" name="salvar" value="Salvar" onclick="gravarProfissionais();"> 
		<? endif; ?>
		<input type="button" name="proximo" value="Próximo" 
				onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_REQUEST['aba'] ?>&aba2=profissionais&aba3=profissionaisResumo';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
