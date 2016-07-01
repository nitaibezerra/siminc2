<?php
$percmax = '75.5';

$montante = calcularMontante();
$sql = "SELECT
			count(turid)
		FROM
			projovemcampo.turma tur
		INNER JOIN projovemcampo.adesaoprojovemcampo ads ON ads.secaid = tur.secaid
		WHERE
			apcid ={$_SESSION['projovemcampo']['apcid']} 
		AND tur.turstatus = 'A'";
$numeroturmas = $db ->pegaUm($sql);

$sql="
		SELECT 
			count(foo)
		from 
		(SELECT DISTINCT
				entid
			FROM
				projovemcampo.turma tur
			INNER JOIN projovemcampo.adesaoprojovemcampo ads ON ads.secaid = tur.secaid
			WHERE
				apcid ={$_SESSION['projovemcampo']['apcid']}
			AND tur.turstatus = 'A')
			as foo";
$numeroescolas = $db ->pegaUm($sql); 
/*Regra para quantidade de Coordenadore de Turma*/
if($numeroescolas>2||$numeroescolas==2){
	if($numeroturmas>10||$numeroturmas==10){
		$ct = round($numeroturmas/10);
	}else{
		$ct = '' ;
	}
}elseif($numeroturmas>10||$numeroturmas==10){
	$ct = round($numeroturmas/10);
}
/*Fim regra quantidade de coordenadores de turma*/
/*carregar dados tela*/
	$efetivorecprog = 3;
	$efeticocomp = 2;
	$eftivo30 = 1;
	$efetivorecprop = 4;
	
	$coordenadorgeral = pegarocpid('1');
	$coordenadorgeralrec = pegarprofissionais('1',$efetivorecprog);
	$coordenadorgeralccm = pegarprofissionais('1',$efeticocomp);
	
	$coordturma = pegarocpid('2');
	$coordturmarec = pegarprofissionais('2',$efetivorecprog);
	$coordturmaccm = pegarprofissionais('2',$efeticocomp);
	
	$assistente = pegarocpid('3');
	$assistenterec = pegarprofissionais('3',$efetivorecprog);

	$educadores_EF_eft= pegarprofissionais('4',$eftivo30);
	$educadores_EF_recprop= pegarprofissionais('4',$efetivorecprop);
	$educadores_EF_rec = pegarprofissionais('4',$efetivorecprog);
	$educadores_EF_ccm = pegarprofissionais('4',$efeticocomp);
	$educadores_EF_qtd = $educadores_EF_rec['qtdcontratado']+$educadores_EF_recprop['qtdcontratado']+$educadores_EF_eft['qtdcontratado'] + $educadores_EF_ccm['qtdcontratado'];
	
	$educadores_EQ_eft= pegarprofissionais('5',$eftivo30);
	$educadores_EQ_recprop= pegarprofissionais('5',$efetivorecprop);
	$educadores_EQ_rec = pegarprofissionais('5',$efetivorecprog);
	$educadores_EQ_ccm = pegarprofissionais('5',$efeticocomp);
	$educadores_EQ_qtd = $educadores_EQ_rec['qtdcontratado']+$educadores_EQ_recprop['qtdcontratado']+$educadores_EQ_eft['qtdcontratado'] + $educadores_EQ_ccm['qtdcontratado'];
	
	$educadores_EP_eft= pegarprofissionais('6',$eftivo30);
	$educadores_EP_recprop= pegarprofissionais('6',$efetivorecprop);
	$educadores_EP_rec = pegarprofissionais('6',$efetivorecprog);
	$educadores_EP_ccm = pegarprofissionais('6',$efeticocomp);
	$educadores_EP_qtd = $educadores_EP_rec['qtdcontratado']+$educadores_EP_recprop['qtdcontratado']+$educadores_EP_eft['qtdcontratado'] + $educadores_EP_ccm['qtdcontratado'];
	
	$educadores_ET_eft= pegarprofissionais('7',$eftivo30);
	$educadores_ET_recprop= pegarprofissionais('7',$efetivorecprop);
	$educadores_ET_rec = pegarprofissionais('7',$efetivorecprog);
	$educadores_ET_qtd = $educadores_ET_rec['qtdcontratado']+$educadores_ET_recprop['qtdcontratado']+$educadores_ET_eft['qtdcontratado'];
	
	$educadores_EE_eft= pegarprofissionais('8',$eftivo30);
	$educadores_EE_recprop= pegarprofissionais('8',$efetivorecprop);
	$educadores_E = $educadores_EE_eft['qtdcontratado'] + $educadores_EE_recprop['qtdcontratado'];
	
	$totalUtilizado = 	$coordenadorgeralrec['vlrtotal'] + $coordenadorgeralccm['vlrtotal']
						+ $coordturmarec['vlrtotal'] + $coordturmaccm['vlrtotal']
						+ $assistenterec['vlrtotal']
						+ $educadores_EF_rec['vlrtotal'] + $educadores_EF_ccm['vlrtotal']
						+ $educadores_EQ_rec['vlrtotal'] + $educadores_EQ_ccm['vlrtotal']
						+ $educadores_EP_rec['vlrtotal'] + $educadores_EP_ccm['vlrtotal']
						+ $educadores_ET_rec['vlrtotal'] 
	;
	$percutilizado = ($totalUtilizado*100)/$montante;
	
	
	
// -- Verificando se existe um arquivo carregado para este público
$sql = "SELECT
			'<center>''<img src=../imagens/excluir.gif title=\"Excluir\" border=0 style=cursor:pointer; onclick=\"excluirAnexo('||arq.arqid||');\">''</center>' as acao ,
			'<a href=\"projovemcampo.php?modulo=principal/anexoGeral&acao=A&download=S&arqid='||arq.arqid||'\">' ||'<center>'|| arq.arqnome ||'.'|| arq.arqextensao ||'</center>'|| '</a>' as nome,
			'<center>' || inl.inlnuminstlegal || '</center>' as numero,
			'<center>' || inl.inldatainstlegal || '</center>' as data,
			'<center>' || tdi.descricaotipo || '</center>' as tipo
		FROM projovemcampo.instrumentolegal inl
		INNER JOIN projovemcampo.anexogeral ang ON ang.angid = inl.angid
		INNER JOIN public.arquivo arq ON arq.arqid = ang.arqid
		INNER JOIN projovemcampo.tipodocumentoinstrumentolegal tdi ON tdi.tpdid = inl.tpdid
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']}
		AND arq.arqstatus = 'A'";
$infoInstLegal = $db->carregar($sql);

$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/projovemcampo.js"></script>
<script>
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
		document.getElementById(result).value = mascaraglobal('##.####.###,##',compl.toFixed(2));
		
	} else {
		document.getElementById(result).value = '0,00';
	}
}

function calculaValorTotal(id_valor, id_meses, id_encargos, id_total, id_qtdcontratado) { 
	if(document.getElementById(id_valor).value && document.getElementById(id_meses).value && document.getElementById(id_encargos).value) {
		document.getElementById(id_meses).value = mascaraglobal('#######',document.getElementById(id_meses).value);
		var valor = replaceAll(replaceAll(document.getElementById(id_valor).value,".",""),",",".");
		var meses = document.getElementById(id_meses).value;
		var valorencargos = valor * meses * (document.getElementById(id_encargos).value/100);
		var total = (valor*meses)+valorencargos;
		if(id_qtdcontratado) {
			if(document.getElementById(id_qtdcontratado).value!='') {
				var qtd = document.getElementById(id_qtdcontratado).value;
			
				total = qtd*total;
				
				document.getElementById(id_total).value = mascaraglobal('##.####.###,##',total.toFixed(2));		
			} else {
				document.getElementById(id_total).value = ''; 
			}
		} else {
			document.getElementById(id_total).value = mascaraglobal('##.####.###,##',total.toFixed(2));
		}
	}else{
		document.getElementById(id_total).value = ''; 
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

jQuery(document).ready(function(){

	jQuery('[name="coordgeral[ocpid]"]').change(function(){
		jQuery('.coordenadorgeral_1').hide();
		jQuery('[name*="coordgeral"][name!="coordgeral[ocpid]"][name!="coordgeral[qtdcontratado]"][name!="coordgeralrec[qtdmeses]"][name!="coordgeralccm[qtdmeses]"]').val(null);
		jQuery('.coordenadorgeral_2').hide();
		if(jQuery(this).val()== '2'){
			jQuery('.coordenadorgeral_2').show();
		}
		if(jQuery(this).val()== '3'){
			jQuery('.coordenadorgeral_1').show();
		} 
	});
	
	jQuery('[name="coordturma[ocpid]"]').change(function(){
		jQuery('.coordenadorturma_1').hide();
		jQuery('[name*="coordturma"][name!="coordturma[ocpid]"][name!="coordturma[qtdcontratado]"][name!="coordturmarec[qtdmeses]"][name!="coordturmaccm[qtdmeses]"]').val(null);
		jQuery('.coordenadorturma_2').hide();
		if(jQuery(this).val()== '2'){
			jQuery('.coordenadorturma_2').show();
		}
		if(jQuery(this).val()== '3'){
			jQuery('.coordenadorturma_1').show();
		} 
	});
	
	jQuery('[name="coordassistentes[ocpid]"]').change(function(){
		jQuery('[name*="coordassistentes"][name!="coordassistentes[ocpid]"][name!="coordassistentes[qtdcontratado]"][name!="coordassistentes[qtdmeses]"]').val(null);
		jQuery('.assistente_M_1').hide();
		if(jQuery(this).val()== '3'){
			jQuery('.assistente_M_1').show();
		} 
	});
	
	jQuery('.total').blur(function(){

		var total = 0;
				
		var coordgeralrecvlr = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="coordgeralrec[vlrtotal]"]').val(),".",""),",","."))!=''){
			coordgeralrecvlr = parseFloat(replaceAll(replaceAll(jQuery('[name="coordgeralrec[vlrtotal]"]').val(),".",""),",","."));
		}
		var coordgeralccmvlr = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="coordgeralccm[vlrtotal]"]').val(),".",""),",","."))!=''){
			coordgeralccmvlr = parseFloat(replaceAll(replaceAll(jQuery('[name="coordgeralccm[vlrtotal]"]').val(),".",""),",","."));
		}

		var coordturmarecvlr = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="coordturmarec[vlrtotal]"]').val(),".",""),",","."))!=''){
			coordturmarecvlr = Number(replaceAll(replaceAll(jQuery('[name="coordturmarec[vlrtotal]"]').val(),".",""),",","."));
		}
		
		var coordturmaccmvlr = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="coordturmaccm[vlrtotal]"]').val(),".",""),",","."))!=''){
			coordturmaccmvlr = Number(replaceAll(replaceAll(jQuery('[name="coordturmaccm[vlrtotal]"]').val(),".",""),",","."));
		}

		var coordassistentesrec = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="coordassistentes[vlrtotal]"]').val(),".",""),",","."))!=''){
			coordassistentesrec = Number(replaceAll(replaceAll(jQuery('[name="coordassistentes[vlrtotal]"]').val(),".",""),",","."));
		}

		var educadores_ef = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="contratadorecurso[EF][vlrtotal]"]').val(),".",""),",","."))!=''){
			educadores_ef = Number(replaceAll(replaceAll(jQuery('[name="contratadorecurso[EF][vlrtotal]"]').val(),".",""),",","."));
		}
		
		var educadores_ef2 = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="contratadocomp[EF][vlrtotal]"]').val(),".",""),",","."))!=''){
			educadores_ef2 = Number(replaceAll(replaceAll(jQuery('[name="contratadocomp[EF][vlrtotal]"]').val(),".",""),",","."));
		}
		
		var educadores_eq = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="contratadorecurso[EQ][vlrtotal]"]').val(),".",""),",","."))!=''){
			educadores_eq = Number(replaceAll(replaceAll(jQuery('[name="contratadorecurso[EQ][vlrtotal]"]').val(),".",""),",","."));
		}

		var educadores_eq2 = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="contratadocomp[EQ][vlrtotal]"]').val(),".",""),",","."))!=''){
			educadores_eq2 = Number(replaceAll(replaceAll(jQuery('[name="contratadocomp[EQ][vlrtotal]"]').val(),".",""),",","."));
		}

		var educadores_ep = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="contratadorecurso[EP][vlrtotal]"]').val(),".",""),",","."))!=''){
			educadores_ep = Number(replaceAll(replaceAll(jQuery('[name="contratadorecurso[EP][vlrtotal]"]').val(),".",""),",","."));
		}

		var educadores_ep2 = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="contratadocomp[EP][vlrtotal]"]').val(),".",""),",","."))!=''){
			educadores_ep2 = Number(replaceAll(replaceAll(jQuery('[name="contratadocomp[EP][vlrtotal]"]').val(),".",""),",","."));
		}

		var educadores_et = 0;
		if(Number(replaceAll(replaceAll(jQuery('[name="contratadorecurso[ET][vlrtotal]"]').val(),".",""),",","."))!=''){
			educadores_et = Number(replaceAll(replaceAll(jQuery('[name="contratadorecurso[ET][vlrtotal]"]').val(),".",""),",","."));
		}
		
		var montante = parseFloat('<?=$montante?>');
		
		var percentual = 0;

		valorrecurso = coordgeralrecvlr + coordturmarecvlr + coordassistentesrec + educadores_ef + educadores_eq +educadores_ep + educadores_et;

		valorcomplementacao = coordgeralccmvlr + coordturmaccmvlr +  educadores_ef2 + educadores_eq2 + educadores_ep2;
		
		total = Number(valorrecurso)  + Number(valorcomplementacao);

		
		if(total>0){
			percentual = (total*100)/montante;		
		}
		
		jQuery('[name="span_totalutilizado"]').html(mascaraglobal('##.####.###,##',total.toFixed(2)));

		jQuery('[name="totalUtilizado"]').val(total.toFixed(2));

		jQuery('[name="profissionais[propercutilizado]"]').val(percentual.toFixed(1));		
			
	});
	jQuery('[name="salvar"]').click(function(){
		
		if(''=='<?=$infoInstLegal['0']['numero']?>'||jQuery(":file").val()!=''){
			
			if(jQuery('#inldatainstlegal').val()=='') {
				alert('Preencha a Data do instrumento');
				rolaTela('inldatainstlegal');
				return false;
			}
// 			if(!validaData(jQuery('[name="inldatainstlegal"'))) {
// 				alert('Data do instrumento inválida. Formato deve ser dd/mm/YYYY');
// 		                rolaTela('inldatainstlegal');
// 				return false;
// 			}
			if ('' == jQuery('#tpdid').val()) {
	            		alert('Selecione o tipo de documento');
	            		rolaTela('tpdid');
	            		return false;
	        }
	        
        	if ('' == jQuery('#arquivo').val()) {
            		alert('Selecione o documento do instrumento legal');
            		rolaTela('arquivo');
           		return false;
        	}
        	
	    	if(jQuery('#inlnuminstlegal').val()=='') {
    				alert('Preencha o Número do instrumento');
	                rolaTela('inlnuminstlegal');
				return false;
			}
		}
		
		if(jQuery('[name="coordgeral[ocpid]"]').val()=='<?=$efetivorecprog ?>'){
			if(jQuery('#vlrtotal_CG').val()==''){
				alert('Preencha os campos para o Coordenador Geral.');
		        	rolaTela('vlrtotal_CG');
				return false;
			}
		}
		if(jQuery('[name="coordgeral[ocpid]"]').val()=='<?=$efeticocomp ?>'){
			if(jQuery('#vlrtotal_CG2').val()==''){
				alert('Preencha os campos para o Coordenador Geral.');
		        	rolaTela('vlrtotal_CG2');
				return false;
			}
		}
		
		if(jQuery('[name="coordgeral[qtdcontratado]"]').val()!=''){
			if(jQuery('[name="coordgeral[ocpid]"]').val()==''){
				alert('Selecione uma opção de contrato para o Coordenador Geral.');
		        	rolaTela('qtdcontratado_CG');
				return false;
			}
		}
// 		if(jQuery('[name="coordturma[qtdcontratado]"]').val()!=''){
// 			if(jQuery('[name="coordturma[ocpid]"]').val()==''){
// 				alert('Selecione uma opção de contrato para o Coordenador de Turma.');
// 		        	rolaTela('qtdcontratado_CT');
// 				return false;
// 			}
// 		}
		if(jQuery('[name="coordturma[ocpid]"]').val()!=''){
			if(jQuery('[name="coordturma[ocpid]"]').val()=='<?=$efetivorecprog ?>'){
				if(jQuery('#vlrtotal_CT').val()==''){
					alert('Preencha os campos para o Coordenador de Turma.');
			        	rolaTela('vlrtotal_CT');
					return false;
				}
			}
			if(jQuery('[name="coordturma[ocpid]"]').val()=='<?=$efeticocomp ?>'){
				if(jQuery('#vlrtota_CT2').val()==''){
					alert('Preencha os campos para o Coordenador de Turma.');
			        	rolaTela('vlrtotal_CT2');
					return false;
				}
			}
		}
		if(jQuery('[name="coordassistentes[qtdcontratado]"]').val()!=''){
			if(jQuery('[name="coordassistentes[ocpid]"]').val()==''){
				alert('Selecione uma opção de contrato para o Assistente de Coordenador.');
		        	rolaTela('qtdcontratado_AA');
				return false;
			}
		}
		if(jQuery('[name="coordassistentes[ocpid]"]').val()!=''){
			if(jQuery('[name="coordassistentes[qtdcontratado]"]').val()==''){
				alert('Escolha A quantidade para Assitente de Coordenador.');
				rolaTela('vlrtotal_AA');
				return false;
			}
		}
		if(jQuery('[name="coordassistentes[ocpid]"]').val()=='<?=$efetivorecprog ?>'){
			if(jQuery('#vlrtotal_AA').val()==''){
				alert('Preencha os campos para o Assistente de Coordenador.');
		        	rolaTela('vlrtotal_AA');
				return false;
			}
		}
		
		var vlr1 = parseFloat(jQuery('#totalMax').val());
		var vlr2 = parseFloat(jQuery('#totalUtilizado').val());
		
		if(vlr2>vlr1) {
			alert('Valor Utilizado maior do que Valor Máximo previsto');
			return false;
		}
		
		<? if($numeroescolas!='') : ?>
			if(!validaQuantidadeCargos('eduefetivo30hr_EF','qtdcontratadorecursoproprio_EF','qtdcontratado_EF','qtdcontratado_EF1','qtdcontrato_EF','sim')) {
				alert('Educadores do Ensino Fundamental: O número do Efetivo + Contratado deve ser igual a quantidade');
				return false;
			}
		<? endif; ?>
		
		<? if($numeroescolas) : ?>
		if(!validaQuantidadeCargos('eduefetivo30hr_EQ','qtdcontratadorecursoproprio_EQ','qtdcontratado_EQ','qtdcontratado_EQ1','qtdcontrato_EQ','sim')) {
			alert('Educadores do Qualificação Profissional: O número do Efetivo + Contratado deve ser igual a quantidade');
			return false;
		}
		<? endif; ?>
		
		<? if($numeroescolas) : ?>
		if(!validaQuantidadeCargos('eduefetivo30hr_EP','qtdcontratadorecursoproprio_EP','qtdcontratado_EP','qtdcontratado_EP1','qtdcontrato_EP','sim')) {
			alert('Educadores para acolhimento de crianças de zero a oito anos: O número do Efetivo + Contratado deve ser igual a quantidade');
			return false;
		}
		<? endif; ?>
		
		
		<? if($numeroescolas) : ?>
		if(!validaQuantidadeCargos('eduefetivo30hr_ET','qtdcontratadorecursoproprio_ET','qtdcontratado_ET','','qtdcontrato_ET','sim')) {
			alert('Tradutor de Intérprete de Libras: O número do Efetivo + Contratado deve ser igual a quantidade');
			return false;
		}
		<? endif; ?>
		<? if($numeroescolas) : ?>
		
		if(!validaQuantidadeCargos('eduefetivo30hr_EE','qtdcontratadorecursoproprio_EE','','','qtdcontratado_EE','sim')) {
			alert('Educador para atendimento educacional especializado: O número do Efetivo + Contratado deve ser igual a quantidade');
			return false;
		}
		<? endif; ?>
		jQuery('#form').submit();
	});
	
    jQuery('[id*=encargos]').each(function() {
        jQuery(this).change(function(){
            if (jQuery(this).val() > 70) {
                alert('O valor do campo "Encargos sociais e/ou trabalhistas" não pode ser maior que 70%.');
                jQuery(this).val('70.0');
            }
        });
    });
    jQuery('[id*=qtdmeses]').each(function() {
        jQuery(this).change(function(){
            if (jQuery(this).val() > 24) {
                alert('O valor do campo "Meses" não pode ser maior que 24.');
                jQuery(this).val('24');
            }
        });
    });
});


</script>
<form id="form" name="form" method="POST" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="gravarProfissionais" id="requisicao">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="15%">Orientações:</td>
		<td>
			<font color=blue>
				<p>
					A seguir será discriminado o pagamento dos professores ou educadores de linguagem códigos e suas tecnologias, ciências humanas, ciências da natureza e matemática, 
					ciências agrárias (qualificação social e profissional) e das salas de acolhimento; do coordenador geral e de turma; dos tradutores e intérpretes de Libras para atendimento 
					aos jovens surdos matriculados no curso; do pessoal de apoio para a etapa de matrícula; e/ou das complementações de remuneração previsto na Resolução CD/FNDE nº 11 de 16 de abril de 2014, 
					não podendo ultrapassar o percentual máximo previsto para a ação.<br>
					Ao final do lançamento deverá ser informado o instrumento legal que baliza as remunerações desses profissionais.
				
			   	</p>
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
					<?=campo_texto('profissionais[propercmax]', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "##.##", "", '', '', 0, 'id="propercmax"', '',number_format($percmax,2,",","."), '' ); ?> 
					R$ 
					<span id="span_totalmaximo"><?=number_format(($montante*$percmax)/100,2,",",".") ?>
					</span>
					<input type="hidden" id="totalMax" value="<?=($montante*$percmax)/100 ?>">
				</td>
				<td align="center">
					<?=campo_texto('profissionais[propercutilizado]', 'N', 'N', 'Percentual Utilizado(%)', 6, 5, "##.##", "", '', '', 0, 'id="propercutilizado"', '', (($percutilizado)?number_format($percutilizado,1,",","."):'0.0'), '' ); ?> R$ 
					<span name="span_totalutilizado">
						<?=number_format($totalUtilizado,2,",",".") ?>
					</span>
					<input type="hidden" id="totalUtilizado" name="totalUtilizado" value="<?=$totalUtilizado ?>">
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Coordenação Local</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Coordenador Geral</td>
		<td>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('coordgeral[qtdcontratado]', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="qtdcontratado_CG" ', '', (($coordenadorgeral['qtdcontratado'])?$coordenadorgeral['qtdcontratado']:'1'), '' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td>
					<?php $sql = "SELECT
										ocpid as codigo,
										nome as descricao
								  FROM
										projovemcampo.opcaocontratoprofissional";
					?>
					<? $db->monta_combo('coordgeral[ocpid]',$sql, $habilita, 'Selecione', '', '', '', '', 'N', '', '', $coordenadorgeral['ocpid'],'','id="coordgeral[ocpid]"'); ?></td>
			</tr>
			<tr class="coordenadorgeral_1" <?=(($coordenadorgeral['ocpid']==$efetivorecprog)?"":"style=\"display:none;\"") ?>>
				<td colspan="2">
					<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
							<td class="SubTituloCentro">Meses</td>
							<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
							<td class="SubTituloCentro">Valor Total</td>
						</tr>
						<tr>
							<td align="center"><? echo campo_texto('coordgeralrec[valorbrutoremuneracao]', 'N', $habilita, 'Valor bruto ad remuneração/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_CG" class="total" ', '', (($coordenadorgeralrec['valorbrutoremuneracao'])?number_format($coordenadorgeralrec['valorbrutoremuneracao'],2,",","."):''), 'calculaValorTotal(\'valorbrutoremuneracao_CG\',\'qtdmeses_CG\',\'encargossociais_CG\',\'vlrtotal_CG\',\'qtdcontratado_CG\');' ); ?></td>
							<td align="center"><? echo campo_texto('coordgeralrec[qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_CG" ', '', (($coordenadorgeralrec['qtdmeses'])?$coordenadorgeralrec['qtdmeses']:'28'), 'calculaValorTotal(\'valorbrutoremuneracao_CG\',\'qtdmeses_CG\',\'encargossociais_CG\',\'vlrtotal_CG\',\'qtdcontratado_CG\');' ); ?></td>
							<td align="center"><? echo campo_texto('coordgeralrec[encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "##.##", "", '', '', 0, 'id="encargossociais_CG" class="total" ', null, $coordenadorgeralrec['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_CG\',\'qtdmeses_CG\',\'encargossociais_CG\',\'vlrtotal_CG\',\'qtdcontratado_CG\');' ); ?></td>
							<td align="center"><? echo campo_texto('coordgeralrec[vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_CG" ', '', (($coordenadorgeralrec['vlrtotal'])?number_format($coordenadorgeralrec['vlrtotal'],2,",","."):''), '' ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
			<tr class="coordenadorgeral_2" <?=(($coordenadorgeral['ocpid']==$efeticocomp)?"":"style=\"display:none;\"") ?> >
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
						<td align="center"><? echo campo_texto('coordgeralccm[numerohoraspagas]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "##", "", '', '', 0, 'id="numerohoraspagas_CG2" class="total" ', 'if(this.value>90){alert(\'Não pode ser maior que 90\');this.value=\'90\'}', $coordenadorgeralccm['numerohoraspagas'], 'calculaValorPorMes(\'valorhora_CG2\',\'numerohoraspagas_CG2\',\'valorbrutoremuneracao_CG2\');calculaValorTotal(\'valorbrutoremuneracao_CG2\',\'qtdmeses_CG2\',\'encargossociais_CG2\',\'vlrtotal_CG2\',\'qtdcontratado_CG\');' ); ?></td>
						<td align="center"><? echo campo_texto('coordgeralccm[valorhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorhora_CG2" class="total" ', '', (($coordenadorgeralccm['valorhora'])?number_format($coordenadorgeralccm['valorhora'],2,",","."):''), 'calculaValorPorMes(\'valorhora_CG2\',\'numerohoraspagas_CG2\',\'valorbrutoremuneracao_CG2\');calculaValorTotal(\'valorbrutoremuneracao_CG2\',\'qtdmeses_CG2\',\'encargossociais_CG2\',\'vlrtotal_CG2\',\'qtdcontratado_CG\');' ); ?></td>
						<td align="center"><? echo campo_texto('coordgeralccm[valorbrutoremuneracao]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_CG2" ', '', (($coordenadorgeralccm['valorbrutoremuneracao'])?number_format($coordenadorgeralccm['valorbrutoremuneracao'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('coordgeralccm[qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "##", "", '', '', 0, 'id="qtdmeses_CG2" ', '', (($coordenadorgeralccm['qtdmeses'])?$coordenadorgeralccm['qtdmeses']:'28'), 'calculaValorTotal(\'valorbrutoremuneracao_CG2\',\'qtdmeses_CG2\',\'encargossociais_CG2\',\'vlrtotal_CG2\',\'qtdcontratado_CG\');' ); ?></td>						
						<td align="center"><? echo campo_texto('coordgeralccm[encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "##.##", "", '', '', 0, 'id="encargossociais_CG2" class="total" ', null, $coordenadorgeralccm['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_CG2\',\'qtdmeses_CG2\',\'encargossociais_CG2\',\'vlrtotal_CG2\',\'qtdcontratado_CG\');' ); ?></td>
						<td align="center"><? echo campo_texto('coordgeralccm[vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_CG2" ', '', (($coordenadorgeralccm['vlrtotal'])?number_format($coordenadorgeralccm['vlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Coordenador de Turma</td>
		<td>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td><? echo campo_texto('coordturma[qtdcontratado]', 'N', ($coordturma['qtdcontratado']?(($numeroturmas>='1')?$habilita:'N'):(($numeroturmas>'1')?$habilita:'N')), 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="qtdcontratado_CT"', '', (($coordturma['qtdcontratado'])?$coordturma['qtdcontratado']:$ct), '' ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td>
						<?php $sql = "SELECT
										ocpid as codigo,
										nome as descricao
									FROM
										projovemcampo.opcaocontratoprofissional";
						?>
						<? $db->monta_combo('coordturma[ocpid]', $sql, ($coordturma['qtdcontratado']?(($numeroturmas>='1')?$habilita:'N'):(($numeroturmas>'1')?$habilita:'N')), 'Selecione', '', '', '', '', 'N', '', '', $coordturma['ocpid'],'','id="coordturma[ocpid]"'); ?>
					</td>
				</tr>
				<tr class="coordenadorturma_1" <?=(($coordturma['ocpid']==$efetivorecprog)?"":"style=\"display:none;\"") ?> >
					<td colspan="2">
						<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
							<tr>
								<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
								<td class="SubTituloCentro">Meses</td>
								<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
								<td class="SubTituloCentro">Valor Total (R$)</td>
							</tr>
							<tr>
								<td align="center"><? echo campo_texto('coordturmarec[valorbrutoremuneracao]', 'N', $habilita, 'Valor bruto ad remuneração/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_CT" class="total" class="coordenadorturma"', '', (($coordturmarec['valorbrutoremuneracao'])?number_format($coordturmarec['valorbrutoremuneracao'],2,",","."):''), 'calculaValorTotal(\'valorbrutoremuneracao_CT\',\'qtdmeses_CT\',\'encargossociais_CT\',\'vlrtotal_CT\');' ); ?></td>
								<td align="center"><? echo campo_texto('coordturmarec[qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_CT" class="coordenadorturma"', '', (($coordturmarec['qtdmeses'])?$coordturmarec['qtdmeses']:'28'), 'calculaValorTotal(\'valorbrutoremuneracao_CT\',\'qtdmeses_CT\',\'encargossociais_CT\',\'vlrtotal_CT\');'  ); ?></td>
								<td align="center"><? echo campo_texto('coordturmarec[encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "##.##", "", '', '', 0, 'id="encargossociais_CT" class="total" class="coordenadorturma"', null, $coordturmarec['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_CT\',\'qtdmeses_CT\',\'encargossociais_CT\',\'vlrtotal_CT\');' ); ?></td>
								<td align="center"><? echo campo_texto('coordturmarec[vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_CT" class="coordenadorturma"', '', (($coordturmarec['vlrtotal'])?number_format($coordturmarec['vlrtotal'],2,",","."):''), '' ); ?></td>
							</tr>
						</table>
					</td>
				</tr>
				<tr class="coordenadorturma_2" <?=(($coordturma['ocpid']==$efeticocomp)?"":"style=\"display:none;\"") ?> >
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
								<td align="center"><? echo campo_texto('coordturmaccm[numerohoraspagas]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "##", "", '', '', 0, 'id="numerohoraspagas_CT2"class="total" class="coordenadorturma"', 'if(this.value>90){alert(\'Não pode ser maior que 90\');this.value=\'90\'}', $coordturmaccm['numerohoraspagas'], 'calculaValorPorMes(\'valorhora_CT2\',\'numerohoraspagas_CT2\',\'valorbrutoremuneracao_CT2\');calculaValorTotal(\'valorbrutoremuneracao_CT2\',\'qtdmeses_CT2\',\'encargossociais_CT2\',\'vlrtotal_CT2\');' ); ?></td>
								<td align="center"><? echo campo_texto('coordturmaccm[valorhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorhora_CT2" class="total" class="coordenadorturma"', '', (($coordturmaccm['valorhora'])?number_format($coordturmaccm['valorhora'],2,",","."):''), 'calculaValorPorMes(\'valorhora_CT2\',\'numerohoraspagas_CT2\',\'valorbrutoremuneracao_CT2\');calculaValorTotal(\'valorbrutoremuneracao_CT2\',\'qtdmeses_CT2\',\'encargossociais_CT2\',\'vlrtotal_CT2\');' ); ?></td>
								<td align="center"><? echo campo_texto('coordturmaccm[valorbrutoremuneracao]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_CT2" class="total" class="coordenadorturma"', '', (($coordturmaccm['valorbrutoremuneracao'])?number_format($coordturmaccm['valorbrutoremuneracao'],2,",","."):''), '' ); ?></td>
								<td align="center"><? echo campo_texto('coordturmaccm[qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "##", "", '', '', 0, 'id="qtdmeses_CT2" class="coordenadorturma"', '', (($coordturmaccm['qtdmeses'])?$coordturmaccm['qtdmeses']:'28'), 'calculaValorTotal(\'valorbrutoremuneracao_CT2\',\'qtdmeses_CT2\',\'encargossociais_CT2\',\'vlrtotal_CT2\');' ); ?></td>						
								<td align="center"><? echo campo_texto('coordturmaccm[encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "##.##", "", '', '', 0, 'id="encargossociais_CT2" class="total" class="coordenadorturma"', null, $coordturmaccm['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_CT2\',\'qtdmeses_CT2\',\'encargossociais_CT2\',\'vlrtotal_CT2\');' ); ?></td>
								<td align="center"><? echo campo_texto('coordturmaccm[vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_CT2" class="coordenadorturma"', '', (($coordturmaccm['vlrtotal'])?number_format($coordturmaccm['vlrtotal'],2,",","."):''), '' ); ?></td>
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
	<tr>
    	<td class="SubTituloDireita">Pessoal de Apoio para a Etapa de Matrícula</td>
       	<td>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td>
					<?php
                        $arrOpt = Array();
                        if( $numeroescolas > 0 ){
                            for( $x = 1; $x <= ($numeroescolas); $x++ ){
                                $arrOpt[] = Array('codigo'=>$x,'descricao'=>$x);
                            }
                        }
                      
                        $db->monta_combo('coordassistentes[qtdcontratado]', $arrOpt, $habilita, 'Selecione', 'calculaValorTotal(\'valorbrutoremuneracao_AA\',\'qtdmeses_AA\',\'encargossociais_AA\',\'vlrtotal_AA\',\'qtdcontratado_AA\');', '', '', '', 'N', 'qtdcontratado_AA', '', $assistente['qtdcontratado']); 
                    
                    ?>
					</td>
           		</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td>
					<? 
					
					$sql = "SELECT
								ocpid as codigo,
								nome as descricao
							FROM
								projovemcampo.opcaocontratoprofissional
							WHERE
								ocpid not in(1,2)";
					
					$db->monta_combo('coordassistentes[ocpid]', $sql, $habilita, 'Selecione', '', '', '', '', 'N', '', '', $assistente['ocpid']); 
					?>
					</td>
             	</tr>
				<tr class="assistente_M_1" <?=(($assistente['ocpid']==$efetivorecprog)?"":"style=\"display:none;\"") ?>>
					<td colspan="2">
					<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
							<td class="SubTituloCentro">Meses</td>
							<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
							<td class="SubTituloCentro">Valor Total</td>
						</tr>
						<tr>
							<td align="center"><? echo campo_texto('coordassistentes[valorbrutoremuneracao]', 'N', $habilita, 'Valor bruto ad remuneração/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_AA" class="total" class="assistente_M"', '', (($assistenterec['valorbrutoremuneracao'])?number_format($assistenterec['valorbrutoremuneracao'],2,",","."):''), 'calculaValorTotal(\'valorbrutoremuneracao_AA\',\'qtdmeses_AA\',\'encargossociais_AA\',\'vlrtotal_AA\',\'qtdcontratado_AA\');' ); ?></td>
							<td align="center"><? echo campo_texto('coordassistentes[qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_AA" class="assistente_M"', 'if(this.value>3){alert(\'Não pode ser maior que 3\');this.value=\'3\'}',(($assistenterec['qtdmeses'])?$assistenterec['qtdmeses']:'3'), '' ); ?></td>
							<td align="center"><? echo campo_texto('coordassistentes[encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "##.##", "", '', '', 0, 'id="encargossociais_AA" class="total" class="assistente_M"', null, $assistenterec['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_AA\',\'qtdmeses_AA\',\'encargossociais_AA\',\'vlrtotal_AA\',\'qtdcontratado_AA\');' ); ?></td>
							<td align="center"><? echo campo_texto('coordassistentes[vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_AA" class="assistente_M"', '', (($assistenterec['vlrtotal'])?number_format($assistenterec['vlrtotal'],2,",","."):''), '' ); ?></td>
						</tr>
					</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Educadores</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores/Professores de educação básica</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[F][qtdcontrato]', 'N', $habilita, 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="qtdcontrato_EF"', 'if(parseFloat(this.value)>parseFloat(\''.($numeroturmas*3).'\')){alert(\'Não pode ser maior que '.($numeroturmas*3).'\');this.value=\''.($numeroturmas*3).'\'}', ($educadores_EF_qtd?$educadores_EF_qtd:''), '' ); ?></td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo</td>
						<td><? echo campo_texto('educadores[F][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EF"', '', $educadores_EF_eft['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EF\',\'qtdcontratadorecursoproprio_EF\',\'qtdcontratado_EF\',\'qtdcontratado_EF1\',\'qtdcontrato_EF\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[F][qtdcontratadorecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="qtdcontratadorecursoproprio_EF"', '', $educadores_EF_recprop['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EF\',\'qtdcontratadorecursoproprio_EF\',\'qtdcontratado_EF\',\'qtdcontratado_EF1\',\'qtdcontrato_EF\');' ); ?></td>
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
						<td class="SubTituloDireita">Contratado com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][qtdcontratado]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="qtdcontratado_EF" class="total"', '', $educadores_EF_rec['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EF\',\'qtdcontratadorecursoproprio_EF\',\'qtdcontratado_EF\',\'qtdcontratado_EF1\',\'qtdcontrato_EF\');calculaValorTotal(\'valorbrutoremuneracao_EF\',\'qtdmeses_EF\',\'encargossociais_EF\',\'vlrtotal_EF\',\'qtdcontratado_EF\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][valorbrutoremuneracao]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_EF" class="total"', '', (($educadores_EF_rec['valorbrutoremuneracao'])?number_format($educadores_EF_rec['valorbrutoremuneracao'],2,",","."):''), 'calculaValorTotal(\'valorbrutoremuneracao_EF\',\'qtdmeses_EF\',\'encargossociais_EF\',\'vlrtotal_EF\',\'qtdcontratado_EF\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_EF"', '', (($educadores_EF_rec['qtdmeses'])?$educadores_EF_rec['qtdmeses']:'24'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 6, "###.##", "", '', '', 0, 'id="encargossociais_EF" class="total"', null, $educadores_EF_rec['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_EF\',\'qtdmeses_EF\',\'encargossociais_EF\',\'vlrtotal_EF\',\'qtdcontratado_EF\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EF][vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_EF"', '', (($educadores_EF_rec['vlrtotal'])?number_format($educadores_EF_rec['vlrtotal'],2,",","."):''), '' ); ?></td>
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
						<td align="center"><? echo campo_texto('contratadocomp[EF][qtdcontratado]', 'N', $habilita, 'Número de diretor de polo', 8, 9, "#######", "", '', '', 0, 'id="qtdcontratado_EF1" class="total"', '', $educadores_EF_ccm['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EF\',\'qtdcontratadorecursoproprio_EF\',\'qtdcontratado_EF\',\'qtdcontratado_EF1\',\'qtdcontrato_EF\');calculaValorTotal(\'valorbrutoremuneracao_EF1\',\'qtdmeses_EF1\',\'encargossociais_EF1\',\'vlrtotal_EF1\',\'qtdcontratado_EF1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][numerohoraspagas]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "########", "", '', '', 0, 'id="numerohoraspagas_EF1" class="total"', 'if(this.value>90){alert(\'Não pode ser maior que 90\');this.value=\'90\'}if(document.getElementById(\'qtdcontratado_EF1\').value==\'\'){alert(\'Preencha o número de educadores\');this.value=\'\';return false;}', $educadores_EF_ccm['numerohoraspagas'], 'calculaValorPorMes(\'valorhora_EF1\',\'numerohoraspagas_EF1\',\'valorbrutoremuneracao_EF1\');calculaValorTotal(\'valorbrutoremuneracao_EF1\',\'qtdmeses_EF1\',\'encargossociais_EF1\',\'vlrtotal_EF1\',\'qtdcontratado_EF1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][valorhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorhora_EF1" class="total"', '', (($educadores_EF_ccm['valorhora'])?number_format($educadores_EF_ccm['valorhora'],2,",","."):''), 'calculaValorPorMes(\'valorhora_EF1\',\'numerohoraspagas_EF1\',\'valorbrutoremuneracao_EF1\');calculaValorTotal(\'valorbrutoremuneracao_EF1\',\'qtdmeses_EF1\',\'encargossociais_EF1\',\'vlrtotal_EF1\',\'qtdcontratado_EF1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][valorbrutoremuneracao]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_EF1"', '', (($educadores_EF_ccm['valorbrutoremuneracao'])?number_format($educadores_EF_ccm['valorbrutoremuneracao'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_EF1" class="total"', '', (($educadores_EF_ccm['qtdmeses'])?$educadores_EF_ccm['qtdmeses']:'24'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.##", "", '', '', 0, 'id="encargossociais_EF1" class="total"', null, $educadores_EF_ccm['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_EF1\',\'qtdmeses_EF1\',\'encargossociais_EF1\',\'vlrtotal_EF1\',\'qtdcontratado_EF1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EF][vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_EF1"', '', (($educadores_EF_ccm['vlrtotal'])?number_format($educadores_EF_ccm['vlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores/Professores de Qualificação Social e Profissional</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[Q][qtdcontrato]', 'N', $habilita, 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="qtdcontrato_EQ"', 'if(parseFloat(this.value)>parseFloat(\''.$numeroturmas.'\')){alert(\'Não pode ser maior que '.$numeroturmas.'\');this.value=\''.$numeroturmas.'\'}', ($educadores_EQ_qtd?$educadores_EQ_qtd:''), '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo</td>
						<td><? echo campo_texto('educadores[Q][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EQ"', '', $educadores_EQ_eft['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EQ\',\'qtdcontratadorecursoproprio_EQ\',\'qtdcontratado_EQ\',\'qtdcontratado_EQ1\',\'qtdcontrato_EQ\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[Q][qtdcontratadorecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="qtdcontratadorecursoproprio_EQ"', '', $educadores_EQ_recprop['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EQ\',\'qtdcontratadorecursoproprio_EQ\',\'qtdcontratado_EQ\',\'qtdcontratado_EQ1\',\'qtdcontrato_EQ\');' ); ?></td>
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
						<td class="SubTituloDireita">Contratado com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][qtdcontratado]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="qtdcontratado_EQ" class="total"', '', $educadores_EQ_rec['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EQ\',\'qtdcontratadorecursoproprio_EQ\',\'qtdcontratado_EQ\',\'qtdcontratado_EQ1\',\'qtdcontrato_EQ\');calculaValorTotal(\'valorbrutoremuneracao_EQ\',\'qtdmeses_EQ\',\'encargossociais_EQ\',\'vlrtotal_EQ\',\'qtdcontratado_EQ\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][valorbrutoremuneracao]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_EQ" class="total"', '', (($educadores_EQ_rec['valorbrutoremuneracao'])?number_format($educadores_EQ_rec['valorbrutoremuneracao'],2,",","."):''), 'calculaValorTotal(\'valorbrutoremuneracao_EQ\',\'qtdmeses_EQ\',\'encargossociais_EQ\',\'vlrtotal_EQ\',\'qtdcontratado_EQ\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_EQ"', '', (($educadores_EQ_rec['qtdmeses'])?$educadores_EQ_rec['qtdmeses']:'24'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.##", "", '', '', 0, 'id="encargossociais_EQ" class="total"', null, $educadores_EQ_rec['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_EQ\',\'qtdmeses_EQ\',\'encargossociais_EQ\',\'vlrtotal_EQ\',\'qtdcontratado_EQ\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EQ][vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_EQ"', '', (($educadores_EQ_rec['vlrtotal'])?number_format($educadores_EQ_rec['vlrtotal'],2,",","."):''), '' ); ?></td>
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
						<td align="center"><? echo campo_texto('contratadocomp[EQ][qtdcontratado]', 'N', $habilita, 'Número de diretor de polo', 8, 9, "#######", "", '', '', 0, 'id="qtdcontratado_EQ1" class="total"', '', $educadores_EQ_ccm['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EQ\',\'qtdcontratadorecursoproprio_EQ\',\'qtdcontratado_EQ\',\'qtdcontratado_EQ1\',\'qtdcontrato_EQ\');calculaValorTotal(\'valorbrutoremuneracao_EQ1\',\'qtdmeses_EQ1\',\'encargossociais_EQ1\',\'vlrtotal_EQ1\',\'qtdcontratado_EQ1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][numerohoraspagas]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "########", "", '', '', 0, 'id="numerohoraspagas_EQ1" class="total"', 'if(this.value>90){alert(\'Não pode ser maior que 90\');this.value=\'90\'}if(document.getElementById(\'qtdcontratado_EQ1\').value==\'\'){alert(\'Preencha o número de educadores\');this.value=\'\';return false;}', $educadores_EQ_ccm['numerohoraspagas'], 'calculaValorPorMes(\'valorhora_EQ1\',\'numerohoraspagas_EQ1\',\'valorbrutoremuneracao_EQ1\');calculaValorTotal(\'valorbrutoremuneracao_EQ1\',\'qtdmeses_EQ1\',\'encargossociais_EQ1\',\'vlrtotal_EQ1\',\'qtdcontratado_EQ1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][valorhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorhora_EQ1" class="total"', '', (($educadores_EQ_ccm['valorhora'])?number_format($educadores_EQ_ccm['valorhora'],2,",","."):''), 'calculaValorPorMes(\'valorhora_EQ1\',\'numerohoraspagas_EQ1\',\'valorbrutoremuneracao_EQ1\');calculaValorTotal(\'valorbrutoremuneracao_EQ1\',\'qtdmeses_EQ1\',\'encargossociais_EQ1\',\'vlrtotal_EQ1\',\'qtdcontratado_EQ1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][valorbrutoremuneracao]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_EQ1"', '', (($educadores_EQ_ccm['valorbrutoremuneracao'])?number_format($educadores_EQ_ccm['valorbrutoremuneracao'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_EQ1"', '', (($educadores_EQ_ccm['qtdmeses'])?$educadores_EQ_ccm['qtdmeses']:'24'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.##", "", '', '', 0, 'id="encargossociais_EQ1" class="total"', null, $educadores_EQ_ccm['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_EQ1\',\'qtdmeses_EQ1\',\'encargossociais_EQ1\',\'vlrtotal_EQ1\',\'qtdcontratado_EQ1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EQ][vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_EQ1"', '', (($educadores_EQ_ccm['vlrtotal'])?number_format($educadores_EQ_ccm['vlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores para acolhimento de crianças de zero a oito anos, filhas de beneficiários do programa</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[P][qtdcontrato]', 'N', $habilita, 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="qtdcontrato_EP"', 'if(parseFloat(this.value)>parseFloat(\''.$numeroturmas.'\')){alert(\'Não pode ser maior que '.$numeroturmas.'\');this.value=\''.$numeroturmas.'\'}', ($educadores_EP_qtd?$educadores_EP_qtd:''), '' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo</td>
						<td><? echo campo_texto('educadores[P][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EP"', '', $educadores_EP_eft['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EP\',\'qtdcontratadorecursoproprio_EP\',\'qtdcontratado_EP\',\'qtdcontratado_EP1\',\'qtdcontrato_EP\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[P][qtdcontratadorecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="qtdcontratadorecursoproprio_EP"', '', $educadores_EP_recprop['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EP\',\'qtdcontratadorecursoproprio_EP\',\'qtdcontratado_EP\',\'qtdcontratado_EP1\',\'qtdcontrato_EP\');' ); ?></td>
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
						<td class="SubTituloDireita">Contratado com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][qtdcontratado]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="qtdcontratado_EP" class="total"', '', $educadores_EP_rec['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EP\',\'qtdcontratadorecursoproprio_EP\',\'qtdcontratado_EP\',\'qtdcontratado_EP1\',\'qtdcontrato_EP\');calculaValorTotal(\'valorbrutoremuneracao_EP\',\'qtdmeses_EP\',\'encargossociais_EP\',\'vlrtotal_EP\',\'qtdcontratado_EP\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][valorbrutoremuneracao]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_EP" class="total"', '', (($educadores_EP_rec['valorbrutoremuneracao'])?number_format($educadores_EP_rec['valorbrutoremuneracao'],2,",","."):''), 'calculaValorTotal(\'valorbrutoremuneracao_EP\',\'qtdmeses_EP\',\'encargossociais_EP\',\'vlrtotal_EP\',\'qtdcontratado_EP\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_EP"', '', (($educadores_EP_rec['qtdmeses'])?$educadores_EP_rec['qtdmeses']:'24'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.##", "", '', '', 0, 'id="encargossociais_EP" class="total"', null, $educadores_EP_rec['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_EP\',\'qtdmeses_EP\',\'encargossociais_EP\',\'vlrtotal_EP\',\'qtdcontratado_EP\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[EP][vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_EP"', '', (($educadores_EP_rec['vlrtotal'])?number_format($educadores_EP_rec['vlrtotal'],2,",","."):''), '' ); ?></td>
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
						<td align="center"><? echo campo_texto('contratadocomp[EP][qtdcontratado]', 'N', $habilita, 'Número de educadores', 8, 9, "#######", "", '', '', 0, 'id="qtdcontratado_EP1" class="total"', '', $educadores_EP_ccm['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EP\',\'qtdcontratadorecursoproprio_EP\',\'qtdcontratado_EP\',\'qtdcontratado_EP1\',\'qtdcontrato_EP\');calculaValorTotal(\'valorbrutoremuneracao_EP1\',\'qtdmeses_EP1\',\'encargossociais_EP1\',\'vlrtotal_EP1\',\'qtdcontratado_EP1\'); ' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][numerohoraspagas]', 'N', $habilita, 'Número de horas pagas com recursos do programa/mês', 8, 7, "########", "", '', '', 0, 'id="numerohoraspagas_EP1" class="total"', 'if(this.value>90){alert(\'Não pode ser maior que 90\');this.value=\'90\'}if(document.getElementById(\'qtdcontratado_EP1\').value==\'\'){alert(\'Preencha o número de educadores\');this.value=\'\';return false;}', $educadores_EP_ccm['numerohoraspagas'], 'calculaValorPorMes(\'valorhora_EP1\',\'numerohoraspagas_EP1\',\'valorbrutoremuneracao_EP1\');calculaValorTotal(\'valorbrutoremuneracao_EP1\',\'qtdmeses_EP1\',\'encargossociais_EP1\',\'vlrtotal_EP1\',\'qtdcontratado_EP1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][valorhora]', 'N', $habilita, 'Valor da hora (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorhora_EP1" class="total"', '', (($educadores_EP_ccm['valorhora'])?number_format($educadores_EP_ccm['valorhora'],2,",","."):''), 'calculaValorPorMes(\'valorhora_EP1\',\'numerohoraspagas_EP1\',\'valorbrutoremuneracao_EP1\');calculaValorTotal(\'valorbrutoremuneracao_EP1\',\'qtdmeses_EP1\',\'encargossociais_EP1\',\'vlrtotal_EP1\',\'qtdcontratado_EP1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][valorbrutoremuneracao]', 'N', 'N', 'Valor da complementação/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_EP1"', '', (($educadores_EP_ccm['valorbrutoremuneracao'])?number_format($educadores_EP_ccm['valorbrutoremuneracao'],2,",","."):''), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_EP1"', '', (($educadores_EP_ccm['qtdmeses'])?$educadores_EP_ccm['qtdmeses']:'24'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.##", "", '', '', 0, 'id="encargossociais_EP1" class="total"', null, $educadores_EP_ccm['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_EP1\',\'qtdmeses_EP1\',\'encargossociais_EP1\',\'vlrtotal_EP1\',\'qtdcontratado_EP1\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadocomp[EP][vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_EP1"', '', (($educadores_EP_ccm['vlrtotal'])?number_format($educadores_EP_ccm['vlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tradutor de Intérprete de Libras</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[T][qtdcontrato]', 'N', $habilita, 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="qtdcontrato_ET"',  'if(parseFloat(this.value)>parseFloat(\''.$numeroturmas.'\')){alert(\'Não pode ser maior que '.$numeroturmas.'\');this.value=\''.$numeroturmas.'\'} if(this.value==\'0\'){this.value=\'\' }', ($educadores_ET_qtd?$educadores_ET_qtd:''), '' ); ?></td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo</td>
						<td><? echo campo_texto('educadores[T][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_ET"', '', $educadores_ET_eft['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_ET\',\'qtdcontratadorecursoproprio_ET\',\'qtdcontratado_ET\',\'\',\'qtdcontrato_ET\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[T][qtdcontratadorecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="qtdcontratadorecursoproprio_ET"', '', $educadores_ET_recprop['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_ET\',\'qtdcontratadorecursoproprio_ET\',\'qtdcontratado_ET\',\'\',\'qtdcontrato_ET\');' ); ?></td>
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
						<td class="SubTituloDireita">Contratado com recursos do programa</td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][qtdcontratado]', 'N', $habilita, 'Número de assistentes', 8, 9, "#######", "", '', '', 0, 'id="qtdcontratado_ET" class="total"', '', $educadores_ET_rec['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_ET\',\'qtdcontratadorecursoproprio_ET\',\'qtdcontratado_ET\',\'\',\'qtdcontrato_ET\');calculaValorTotal(\'valorbrutoremuneracao_ET\',\'qtdmeses_ET\',\'encargossociais_ET\',\'vlrtotal_ET\',\'qtdcontratado_ET\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][valorbrutoremuneracao]', 'N', $habilita, 'Valor bruto da remuneração/mês (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="valorbrutoremuneracao_ET" class="total"', '', (($educadores_ET_rec['valorbrutoremuneracao'])?number_format($educadores_ET_rec['valorbrutoremuneracao'],2,",","."):''), 'calculaValorTotal(\'valorbrutoremuneracao_ET\',\'qtdmeses_ET\',\'encargossociais_ET\',\'vlrtotal_ET\',\'qtdcontratado_ET\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][qtdmeses]', 'N', $habilita, 'Meses', 8, 7, "########", "", '', '', 0, 'id="qtdmeses_ET"', '', (($educadores_ET_rec['qtdmeses'])?$educadores_ET_rec['qtdmeses']:'24'), '' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][encargossociais]', 'N', $habilita, 'Encargos sociais e/ou trabalhistas (%)', 6, 5, "###.##", "", '', '', 0, 'id="encargossociais_ET" class="total"', null, $educadores_ET_rec['encargossociais'], 'calculaValorTotal(\'valorbrutoremuneracao_ET\',\'qtdmeses_ET\',\'encargossociais_ET\',\'vlrtotal_ET\',\'qtdcontratado_ET\');' ); ?></td>
						<td align="center"><? echo campo_texto('contratadorecurso[ET][vlrtotal]', 'N', 'N', 'Valor Total (R$)', 15, 14, "##.####.###,##", "", '', '', 0, 'id="vlrtotal_ET"', '', (($educadores_ET_rec['vlrtotal'])?number_format($educadores_ET_rec['vlrtotal'],2,",","."):''), '' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educador/Professor para atendimento educacional especializado</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo campo_texto('educadores[E][qtdcontratado]', 'N', $habilita, 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="qtdcontratado_EE"','if(parseFloat(this.value)>parseFloat(\''.$numeroturmas.'\')){alert(\'Não pode ser maior que '.$numeroturmas.'\');this.value=\''.$numeroturmas.'\'}', (($educadores_E)?$educadores_E:''), 'verificaEducadorEspecializado(\'qtdcontratado_EE\');' ); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo</td>
						<td><? echo campo_texto('educadores[E][eduefetivo30hr]', 'N', $habilita, 'Efetivo com 40 horas', 8, 7, "########", "", '', '', 0, 'id="eduefetivo30hr_EE"', '', $educadores_EE_eft['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EE\',\'qtdcontratadorecursoproprio_EE\',\'\',\'\',\'qtdcontratado_EE\');' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo campo_texto('educadores[E][qtdcontratadorecursoproprio]', 'N', $habilita, 'Contratado com recursos próprios', 8, 7, "########", "", '', '', 0, 'id="qtdcontratadorecursoproprio_EE"', '', $educadores_EE_recprop['qtdcontratado'], 'validaQuantidadeCargos(\'eduefetivo30hr_EE\',\'qtdcontratadorecursoproprio_EE\',\'\',\'\',\'qtdcontratado_EE\');' ); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
	
		</td>
	</tr>
	<tr> 
		<td class="SubTituloDireita">Instrumento legal</td>
		<td>
            <table>
				<tr>
					<td align="right">Número do instrumento</td>
					<td><? echo campo_texto('inlnuminstlegal', 'S', $habilita, 'Número do instrumento', 8, 7, "########", "", '', '', 0, 'id="inlnuminstlegal" required', '', $infoInstLegal['inlnuminstlegal'], '' ); ?></td>
			</tr>
			<tr>
				<td align="right">Data do instrumento</td>
				<td><? echo campo_texto('inldatainstlegal', 'S', $habilita, 'Data do instrumento', 11, 10, "##/##/####", "", '', '', 0, 'id="inldatainstlegal" required', '', formata_data($infoInstLegal['inldatainstlegal']), '' ); ?>Formato:##/##/####</td>
			</tr>
			<tr>
				<td align="right">Tipo do instrumento</td>
				<td><?php $sql = "
							SELECT tdil.tpdid AS codigo,
							       tdil.descricaotipo AS descricao
							  FROM projovemcampo.tipodocumentoinstrumentolegal tdil
							  ORDER BY descricao
							";
                          $db->monta_combo('tpdid', $sql, 'S', 'Selecione um tipo', null, '', '', '', 'S', 'muncod', null, $infoInstLegal['tpdid'], null, 'id="tpdid"'); ?>
					</td>
				</tr>
				<tr>
					<td align="right">Arquivo</td>
					<td>
						<input type="hidden" name="arquivo" id="arquivo" value="uploaded" />
						<input type="file" name="arquivo" id="arquivo" /><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					</td>
				</tr>
                   <?if(is_array($infoInstLegal)){
                   		$cabecalho = array("","Arquivo","Número do instrumento","Data do instrumento","Tipo Instrumento");
						$db->monta_lista($infoInstLegal,$cabecalho,100,5,'N','center',$par2);
                  	}?>
			</table>
		</td>
	</tr>

	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="anterior" value="Anterior" 
				onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula';"> 
		<input type="button" name="salvar" value="Salvar" class="gravar"> 
		<input type="button" name="proximo" value="Próximo" 
				onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba3=profissionaisResumo';"></td>
	</tr>
</table>
</form>
<? 
registarUltimoAcesso(); ?>
