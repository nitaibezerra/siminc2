<?php
$_SESSION['siscap']['facid'] =  empty($_SESSION['siscap']['facid']) ? $_REQUEST['facid'] : $_SESSION['siscap']['facid'];

$_SESSION['siscap']['visualizar'] = empty($_SESSION['siscap']['visualizar']) ? $_REQUEST['visualizar'] : $_SESSION['siscap']['visualizar'];
$visualiza = $_SESSION['siscap']['visualizar'];

$facid = $_SESSION['siscap']['facid'];

if( $_POST['requisicao'] == 'salvar' ){
	extract( $_POST );

	if( $paiid != 1 ){
		$muncod = '';
	} else {
		$faccidade = '';
		$facestado = '';
	}
	if( !empty( $facrg ) ){
		$facorgaoexp = $facorgaoexp;
		$facdataexp = $facdataexp;
	} else {
		$facorgaoexp = '';
		$facdataexp = '';
	}

	$faccpf = str_replace(array(".","-"), "", $faccpf);

	$facdataexp = ( !empty($facdataexp) ? "'".formata_data_sql( $facdataexp )."'" : 'null' );
	$facdtnasc = ( !empty($facdtnasc) ? "'".formata_data_sql( $facdtnasc )."'" : 'null' );
	$facorgaoexp = ( !empty($facorgaoexp) ? "'".$facorgaoexp."'" : 'null' );
	$vincid = ( !empty($vincid) ? $vincid : 'null' );
	$escid = ( !empty($escid) ? $escid : 'null' );

	$facestado = ( !empty($facestado) ? "'".$facestado."'" : 'null' );
	$faccidade = ( !empty($faccidade) ? "'".$faccidade."'" : 'null' );

	$muncod = ( !empty($muncod) ? "'".$muncod."'" : 'null' );
	$paiid = ( !empty($paiid) ? $paiid : 'null' );

	if( empty($facid) ){
		$sql = "SELECT count(faccpf) as total FROM siscap.facilitador WHERE faccpf = '$faccpf'";
		$retorno = $db->pegaUm( $sql );

		if( $retorno == 0 ){
			$sql = "INSERT INTO siscap.facilitador(faccpf, vincid, escid, facnome, facsexo, facestadocivil, muncod,
	  										facrg, facorgaoexp, facdataexp, facnomemae, facnomepai, facdtnasc, facestado, faccidade, paiid, facdtinc)
					VALUES ('$faccpf', $vincid, $escid, '$facnome', '$facsexo', '$facestadocivil', $muncod,
	  										'$facrg', $facorgaoexp, $facdataexp, '$facnomemae', '$facnomepai', $facdtnasc, $facestado, $faccidade, $paiid, 'now()') RETURNING facid";

			$facid = $db->pegaUm($sql);

			$_SESSION['siscap']['facid'] = $facid;
		} else {
			echo "<script>
					alert('Facilitador já cadastrado');
					window.location.href = 'siscap.php?modulo=principal/dadosPessoais&acao=A';
				 </script>";
			exit();
		}
	} else {
		$sql = "UPDATE siscap.facilitador SET
				  vincid = $vincid,
				  escid = $escid,
				  facnome = '$facnome',
				  facsexo = '$facsexo',
				  facestadocivil = '$facestadocivil',
				  muncod = $muncod,
				  facrg = '$facrg',
				  facorgaoexp = $facorgaoexp,
				  facdataexp = $facdataexp,
				  facnomemae = '$facnomemae',
				  facnomepai = '$facnomepai',
				  facdtnasc = $facdtnasc,
				  facestado = $facestado,
				  faccidade	= $faccidade,
				  paiid	= $paiid
				WHERE
				  faccpf = '$faccpf'";

		$db->executar($sql);
	}

	if($db->commit()){
//		$db->sucesso( 'principal/dadosFuncionais' );
        echo "<script>
                alert('Operação realizada com sucesso.');
             </script>";
	} else {
		echo "<script>
				alert('Falha na Operação');
				window.location.href = 'siscap.php?modulo=principal/dadosPessoais&acao=A';
			 </script>";
		exit();
	}
}

if( $_POST['requisicao'] == 'combocidade' ){
	extract( $_POST );
	$displayNO = "style=\"display: none\";";
	$display = "style=\"display: ''\";";
}

if( !empty($facid) && $_POST['requisicao'] != 'combocidade' ){
	$sql = "SELECT a.faccpf, a.vincid, a.escid, a.facnome, a.facsexo, a.facestadocivil, a.muncod, a.facrg, a.facorgaoexp,
 					 a.facdataexp, a.facnomemae, a.facnomepai, a.facdtnasc, a.facestado, a.faccidade, t.paidescricao, a.paiid
			FROM siscap.facilitador a left join territorios.pais t on t.paiid = a.paiid WHERE facid = $facid";
	$arDados = $db->pegaLinha( $sql );

	extract( $arDados );
	$faccpf = formatar_cpf( $faccpf );
	$facdataexp = ( !empty($facdataexp) ? formata_data( $facdataexp ) : '' );
	$facdtnasc = ( !empty($facdtnasc) ? formata_data( $facdtnasc ) : '' );
	$facestadocivil = trim($facestadocivil);

	if( !empty($muncod) ){
		$sql = "SELECT estuf FROM territorios.municipio WHERE muncod = '$muncod'";
		$estuf = $db->pegaUm( $sql );
		$_POST['estuf'] = $estuf;

		$displayNO = "style=\"display: none\";";
		$display = "style=\"display: ''\";";
	} else {
		$displayNO = "style=\"display: ''\";";
		$display = "style=\"display: none\";";
	}
} else {
	if( empty($_POST['estuf']) ){
		$displayNO = "style=\"display: ''\";";
		$display = "style=\"display: none\";";
	}
}

if( !empty( $facrg ) ){
	$readonlytrue = "style=\"display: none\";";
	$orgaotrue = "style=\"display: ''\";";
	$readonlyfalse = "style=\"display: ''\";";
	$orgaofalse = "style=\"display: none\";";
} else {
	$readonlytrue = "style=\"display: ''\";";
	$orgaotrue = "style=\"display: none\";";
	$readonlyfalse = "style=\"display: none\";";
	$orgaofalse = "style=\"display: ''\";";
}

/*
 * if( valor != '' ){
		$('readonlytrue').style.display = 'none';
		$('orgaotrue').style.display = '';
		$('readonlyfalse').style.display = '';
		$('orgaofalse').style.display = 'none';
	} else {
		$('readonlytrue').style.display = '';
		$('orgaotrue').style.display = 'none';
		$('readonlyfalse').style.display = 'none';
		$('orgaofalse').style.display = '';
	}
*/

require_once APPRAIZ . "www/includes/webservice/cpf.php";
include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Dados Pessoais', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );
$habilita = 'S';
//if( !empty($_GET['faccpfAltera']) ) $habilita = 'N';

$habilita = $visualiza == 1 ? 'N' : 'S';

$habil = $habilita == 'N'  ? 'disabled="disabled"' : '';

if( empty($facrg) && $habilita == 'S' ){
	$habilitaRG = 'N';
} else {
	$habilitaRG = $habilita;
}
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="facid" id="facid" value="<?=$facid ?>">
	<input type="hidden" name="dataAtual" id="dataAtual" value="<?=date( 'd/m/Y' ); ?>">
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width="15%" class="subtitulodireita">CPF:</td>
			<td>
				<?
				echo campo_texto('faccpf', 'S', $habilita, 'CPF', 30, 15, '###.###.###-##', '', 'left', '', 0, "id=faccpf class='normal classcpf'" );
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Nome Completo:</td>
			<td>
				<?
				echo campo_texto('facnome_temp', 'S', 'N', 'Nome Completo', 100, 70, '', '', '', '', 0, 'id=facnome_temp style="text-transform: uppercase;"', '', $facnome);
				//echo campo_texto('facnome', 'S', $habilita, 'Nome Completo', 100, 70, '', '', '', '', 0, 'id=facnome style="text-transform: uppercase;"' );
				?>
                <input type="hidden" id="facnome" name="facnome" value="<?php echo $facnome ? $facnome : ''?>">
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Data de Nascimento:</td>
			<td>
				<?
				echo campo_texto('facdtnasc_temp', 'S', 'N', 'Data de Nascimento', 10, 10, '', '', '', '', 0, 'id=facdtnasc_temp', '', $facdtnasc);
				?>
                <input type="hidden" id="facdtnasc" name="facdtnasc" value="<?php echo $facdtnasc ? $facdtnasc : ''?>">
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Sexo:</td>
			<td>
				<input disabled='disabled' name="facsexo_temp" value="M" <?=$habil ?> id="facsexoM" <?=($facsexo == 'M' ? 'checked="checked"' : ''); ?> type="radio" title="Sexo"><label for="facsexoM">Masculino</label>
				<input disabled='disabled' name="facsexo_temp" value="F" <?=$habil ?> id="facsexoF" <?=($facsexo == 'F' ? 'checked="checked"' : ''); ?> type="radio" title="Sexo"><label for="facsexoF">Feminino</label>
				<img src="../imagens/obrig.gif" <?=$habil ?> title="Indica campo obrigatório." border="0">
                <input type="hidden" id="facsexo" name="facsexo" value="<?php echo $facsexo ? $facsexo : ''?>">
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Estado Civil:</td>
			<td>
				<?
				$arEstCivil = array(
								array("codigo" => 'C', "descricao" => 'Casado'),
								array("codigo" => 'S', "descricao" => 'Solteiro'),
								array("codigo" => 'D', "descricao" => 'Divorciado'),
								array("codigo" => 'V', "descricao" => 'Viúvo'),
								array("codigo" => 'SJ', "descricao" => 'Separado Judicialmente'),
								array("codigo" => 'UE', "descricao" => 'União Estável')
							  );
				$db->monta_combo("facestadocivil", $arEstCivil, $habilita, 'Selecione...', '', '', '', '200', 'S', 'facestadocivil', '', '', 'Estado Civil' );
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Nacionalidade:</td>
			<td>
				<?
				$sql = "SELECT paiid as codigo, paidescricao as descricao FROM territorios.pais ORDER BY paidescricao";
				$db->monta_combo("paiid", $sql, $habilita, 'Selecione...', 'verificaPais', '', '', '265', 'S', 'paiid', '', '', 'Nacionalidade' );
				?>
			</td>
		</tr>
		<tr id="estadoBrasil" <?=$display; ?>>
			<td width="15%" class="subtitulodireita">Naturalidade:</td>
			<td>
				<?
				$estuf = $_POST['estuf'];
				$sql = "SELECT estuf as codigo, estdescricao as descricao
						FROM territorios.estado
						ORDER BY estdescricao";
				$db->monta_combo("estuf", $sql, $habilita, 'Selecione...', 'montaComboMunicipios', '', '', '265', 'S', 'estuf', '', '', 'Naturalidade' );
				?>
			</td>
		</tr>
		<tr id="cidadeBrasil" <?=$display; ?>>
			<td width="15%" class="subtitulodireita">Cidade:</td>
			<td>
				<?
				if( $_POST['estuf'] ){
					$sql = "select e.muncod as codigo, e.mundescricao as descricao from territorios.municipio e where e.estuf = '".$_POST['estuf']."' order by e.mundescricao asc";
					$db->monta_combo("muncod", $sql, $habilita, 'Selecione uma Cidade', '', '', '', '', 'S', 'muncod', '', '', 'Cidade' );
				} else {
					$sql = "select e.muncod as codigo, e.mundescricao as descricao from territorios.municipio e order by e.mundescricao asc";
					$db->monta_combo("muncod", $sql, $habilita, 'Selecione uma Cidade', '', '', '', '', 'S', 'muncod', '', '', 'Cidade' );
				}
				?>
			</td>
		</tr>
		<tr id="estadoNOBrasil" <?=$displayNO; ?>>
			<td width="15%" class="subtitulodireita">Naturalidade:</td>
			<td>
				<?
				echo campo_texto('facestado', 'S', $habilita, 'Estado', 50, 80, '', '', '', '', 0, 'id=facestado' );
				?>
			</td>
		</tr>
		<tr id="cidadeNOBrasil" <?=$displayNO; ?>>
			<td width="15%" class="subtitulodireita">Cidade:</td>
			<td>
				<?
				echo campo_texto('faccidade', 'S', $habilita, 'Cidade', 50, 80, '', '', '', '', 0, 'id=faccidade' );
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">RG:</td>
			<td>
				<?
				echo campo_texto('facrg', 'N', $habilita, 'RG', 30, 25, '', '', '', '', 0, 'id=facrg', '', '', 'desabilitaCampos(this.value);');
				?>
			</td>
		</tr>
        <!--
        <tr>
            <td width="15%" class="subtitulodireita">Órgão Expedidor:</td>
            <td>
                <?
                echo campo_texto('facorgaoexp_temp', 'S', 'N', 'Órgão Expedidor', 5, 5, '', '', '', '', 0, 'id=facorgaoexp_temp style="text-transform: uppercase;"', '', $facorgaoexp);
                ?>
                <input type="hidden" id="facorgaoexp" name="facorgaoexp" value="<?php echo $facorgaoexp ? $facorgaoexp : ''?>">
            </td>
        </tr>
        -->
		<tr id="orgaotrue" <?=$orgaotrue; ?>>
			<td width="15%" class="subtitulodireita">Órgão Expedidor:</td>
			<td>
				<input type="text" class=" normal" style="text-transform: uppercase" title="Orgão Exp" id="facorgaoexp" onblur="MouseBlur(this);"
					onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);"
					value="<?=$facorgaoexp; ?>" maxlength="150" size="81" name="facorgaoexp" <?=$habil ?>>
			</td>
		</tr>
		<tr id="orgaofalse" <?=$orgaofalse; ?>>
			<td width="15%" class="subtitulodireita">Órgão Expedidor:</td>
			<td>
				<input type="text" class=" normal" style="text-transform: uppercase" title="Orgão Exp" id="facorgaoexp1" onblur="MouseBlur(this);"
					onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);"
					value="<?=$facorgaoexp; ?>" maxlength="150" size="81" name="facorgaoexp1" disabled="disabled" <?=$habil ?>>
			</td>
		</tr>
        <!--
        <tr>
            <td width="15%" class="subtitulodireita">Data Expedição::</td>
            <td>
                <?
                echo campo_texto('facdataexp_temp', 'S', 'N', 'Órgão Expedidor',10, 10, '', '', '', '', 0, 'id=facdataexp_temp style="text-transform: uppercase;"', '', $facdataexp);
                ?>
                <input type="hidden" id="facdataexp" name="facdataexp" value="<?php echo $facdataexp ? $facdataexp : ''?>">
            </td>
        </tr>
        -->
		<tr id="readonlyfalse" <?=$readonlyfalse; ?>>
			<td width="15%" class="subtitulodireita">Data Expedição:</td>
			<td>
                <?
                echo campo_data2('facdataexp', 'N',$habilita,'Data Expedição','','','');
                ?>
			</td>
		</tr>
		<tr id="readonlytrue" <?=$readonlytrue; ?>>
			<td width="15%" class="subtitulodireita">Data Expedição:</td>
			<td>
				<?
				echo campo_data2('facdataexp1', 'N', $habilita,'Data Expedição','','','');
				?>
			</td>
		</tr>
        <tr>
            <td width="15%" class="subtitulodireita">Nome da Mãe:</td>
            <td>
                <?
                echo campo_texto('facnomemae_temp', 'S', 'N', 'Nome da Mãe', 80, 120, '', '', '', '', 0, 'id=facnomemae_temp style="text-transform: uppercase;"', '', $facnomemae);
                ?>
                <input type="hidden" id="facnomemae" name="facnomemae" value="<?php echo $facnomemae ? $facnomemae : ''?>">
            </td>
        </tr>
        <!--
		<tr>
			<td width="15%" class="subtitulodireita">Nome da Mãe:</td>
			<td>
				<?
				echo campo_texto('facnomemae', 'N', $habilita, 'Nome da Mãe', 80, 120, '', '', '', '', 0, 'id=facnomemae onkeypress="return somenteLetra( event )"' );
				?>
			</td>
		</tr>
        -->
		<tr>
			<td width="15%" class="subtitulodireita">Nome do Pai:</td>
			<td>
				<?
				echo campo_texto('facnomepai', 'N', $habilita, 'Nome do Pai', 80, 120, '', '', '', '', 0, 'id=facnomepai onkeypress="return somenteLetra( event )"' );
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
            <td></td>
			<td style="text-align: left">
				<input type="button" id="btSalvar" value="Salvar" <?=$habil; ?> onclick="salvarDadosPessoais();" />
				<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/listaFacilitador&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript">
jQuery.noConflict();

jQuery(document).ready(function(){
	jQuery('input.classcpf').live('change',function(){
		if( !validar_cpf( jQuery(this).val()  ) ){
			alert( "CPF inválido!\nFavor informar um cpf válido!" );
			jQuery(this).val('');
			return false;
		}
		var comp  = new dCPF();

		comp.buscarDados( jQuery(this).val() );
		if (comp.dados.no_pessoa_rf != ''){
			jQuery('#facnome').val( comp.dados.no_pessoa_rf );
			jQuery('#facnome_temp').val( comp.dados.no_pessoa_rf );
			var dataNasc = comp.dados.dt_nascimento_rf.substr(6,2) +'/'+ comp.dados.dt_nascimento_rf.substr(4,2) +'/'+ comp.dados.dt_nascimento_rf.substr(0,4);

			jQuery('#facdtnasc').val( dataNasc );
			jQuery('#facdtnasc_temp').val( dataNasc );
			if( comp.dados.sg_sexo_rf == 'M' ) jQuery('#facsexoM').attr('checked', true);
			if( comp.dados.sg_sexo_rf == 'F' ) jQuery('#facsexoF').attr('checked', true);
			jQuery('#facsexo').val( comp.dados.sg_sexo_rf );
			if( comp.dados.nu_rg  ){
				jQuery('#facorgaoexp').val( comp.dados.ds_orgao_expedidor_rg );
				jQuery('#facorgaoexp_temp').val( comp.dados.ds_orgao_expedidor_rg );
				jQuery('#facdataexp').val( comp.dados.dt_emissao_rg );
				jQuery('#facdataexp_temp').val( comp.dados.dt_emissao_rg );
				jQuery('#facrg').val( comp.dados.nu_rg );
				jQuery('#facrg_temp').val( comp.dados.nu_rg );
			}
			jQuery('#facnomemae').val( comp.dados.no_mae_rf );
			jQuery('#facnomemae_temp').val( comp.dados.no_mae_rf );
		}
	});
});

function montaComboMunicipios( estuf ){
	$('requisicao').value = 'combocidade';
	document.getElementById('formulario').submit();
}

function desabilitaCampos( valor ){
	if( valor != '' ){
		$('readonlytrue').style.display = 'none';
		$('orgaotrue').style.display = '';
		$('readonlyfalse').style.display = '';
		$('orgaofalse').style.display = 'none';
	} else {
		$('readonlytrue').style.display = '';
		$('orgaotrue').style.display = 'none';
		$('readonlyfalse').style.display = 'none';
		$('orgaofalse').style.display = '';
	}
}

function salvarDadosPessoais(){

	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	campos[0] 			= "faccpf";
	campos[1] 			= "facnome";
	campos[2] 			= "facdtnasc";
	campos[3] 			= "facsexo";
	campos[4] 			= "facestadocivil";
	campos[5] 			= "paiid";
	if( $('paiid').value == 1 ){
		campos[6] 			= "estuf";
		campos[7] 			= "muncod";
		tiposDeCampos[6] 	= "select";
		tiposDeCampos[7] 	= "select";
	} else {
		campos[6] 			= "facestado";
		campos[7] 			= "faccidade";
		tiposDeCampos[6] 	= "texto";
		tiposDeCampos[7] 	= "texto";
	}

	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "texto";
	tiposDeCampos[2] 	= "texto";
	tiposDeCampos[3] 	= "radio";
	tiposDeCampos[4] 	= "select";
	tiposDeCampos[5] 	= "select";
	tiposDeCampos[6] 	= "select";

	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		var obDt  = new Data();

		var dataatual 	= $('dataAtual').value;
		var datanasc 	= $('facdtnasc').value;
		var dataexp 	= $('facdataexp').value;

		if( obDt.comparaData( datanasc, dataatual, '>' ) ){
			alert('A Data de Nascimento não pode ser maior que a data atual!');
			$('facdtnasc').focus();
			return false;
		}
		if( obDt.comparaData( dataexp, dataatual, '>' ) && dataexp != '' && $('readonlyfalse').style.display == '' ){
			alert('A Data Expedição não pode ser maior que a data atual!');
			$('facdataexp').focus();
			return false;
		}
		$('requisicao').value = 'salvar';
		$('formulario').submit();
	}
}

function verificaPais( id ){
	if( id == 1 ){
		$('estadoNOBrasil').style.display = 'none';
		$('estadoBrasil').style.display = '';
		$('cidadeNOBrasil').style.display = 'none';
		$('cidadeBrasil').style.display = '';
	} else if( id != '' ){
		$('estadoNOBrasil').style.display = '';
		$('estadoBrasil').style.display = 'none';
		$('cidadeNOBrasil').style.display = '';
		$('cidadeBrasil').style.display = 'none';
	}
}

function somenteLetra(e) {
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if( key > 48 || key < 58) return (key < 49 || key > 57);
    else return true;
}
</script>