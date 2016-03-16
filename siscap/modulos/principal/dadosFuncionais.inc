<?php
$_SESSION['siscap']['facid'] =  empty($_SESSION['siscap']['facid']) ? $_REQUEST['facid'] : $_SESSION['siscap']['facid'];
$facid = $_SESSION['siscap']['facid'];

validaSessao( $facid, 'principal/dadosPessoais' );

if( $_REQUEST['buscadados'] == 'true' ){
	header('content-type: text/html; charset=ISO-8859-1');

	$sql = "SELECT nu_matricula_siape, co_uorg_lotacao_servidor, no_servidor,
				co_funcao, co_nivel_funcao, ds_funcao, ds_situacao_servidor,
				co_orgao_lotacao_servidor, sg_unidade_organizacional,
				ds_orgao, ds_cargo_emprego, nu_cpf
				FROM siscap.tb_cadastro_servidor
			WHERE
            	nu_matricula_siape = '".$_POST['matricula']."' limit 1";

	$arDados = $db->pegaLinha( $sql );
	echo simec_json_encode( $arDados );
	exit();
}

if( $_POST['requisicao'] == 'salvar' ){
	extract( $_POST );

	$daftelefax = str_replace(array("-"), "", $daftelefax);
	$daftelecomercial = str_replace(array("-"), "", $daftelecomercial);
	$dafcep = str_replace(array("-"), "", $dafcep);
	$daftelefax = trim($daftelefax);
	$daftelecomercial = trim($daftelecomercial);
	$dafoutrosvinculos = ( $dafsesimqual == 'OU' ? $dafoutrosvinculos : '' );
	$dafsesimqual = ( $dafvincservpubfed == 'S' ? "'".$dafsesimqual."'" : 'null' );
	$daftelecomercial = !empty( $daftelecomercial ) ? "'".$dafdddcomercial.$daftelecomercial."'" : 'null';
	$daftelefax = !empty( $daftelefax ) ? "'".$dafdddfax.$daftelefax."'" : 'null';

	if( empty($dafid) ){
		$sql = "INSERT INTO siscap.dadosfuncionais(dafvincservpubfed, dafsesimqual, dafmatriculasiap, dafnumpispasep,
							  dafnuminsprevsocial, dafcargoefetivo, daffuncao, daflotacao, daforgao, dafcargo,
							  dafcidade, dafestado, dafbairro, daflogradouro, dafcomplemento, dafemail, facid, dafcep, dafoutrosvinculos, daftelecomercial, daftelefax)
				VALUES ('$dafvincservpubfed', $dafsesimqual, '$dafmatriculasiap', '$dafnumpispasep',
							  '$dafnuminsprevsocial', '$dafcargoefetivo', '".trim($daffuncao)."', '".trim($daflotacao)."', '".trim($daforgao)."', '".trim($dafcargo)."',
							  '$dafcidade', '$dafestado', '".trim($dafbairro)."', '".trim($daflogradouro)."', '$dafcomplemento', '$dafemail', '$facid', '$dafcep', '$dafoutrosvinculos', $daftelecomercial, $daftelefax)";

		$db->executar($sql);
	} else {
		$sql = "UPDATE siscap.dadosfuncionais SET
				  dafvincservpubfed = '$dafvincservpubfed',
				  dafsesimqual = $dafsesimqual,
				  dafmatriculasiap = '$dafmatriculasiap',
				  dafnumpispasep = '$dafnumpispasep',
				  dafnuminsprevsocial = '$dafnuminsprevsocial',
				  dafcargoefetivo = '$dafcargoefetivo',
				  daffuncao = '".trim($daffuncao)."',
				  daflotacao = '".trim($daflotacao)."',
				  dafcidade = '$dafcidade',
				  dafestado = '$dafestado',
				  dafbairro = '$dafbairro',
				  daflogradouro = '$daflogradouro',
				  dafcomplemento = '$dafcomplemento',
				  dafemail = '$dafemail',
				  dafcep = '$dafcep',
				  dafoutrosvinculos = '$dafoutrosvinculos',
				  daftelecomercial = $daftelecomercial,
				  daftelefax = $daftelefax,
				  daforgao = '".trim($daforgao)."',
				  dafcargo = '".trim($dafcargo)."'
				WHERE
				  dafid = $dafid";

		$db->executar($sql);
	}

	if($db->commit()){
//		$db->sucesso( 'principal/contato' );
        echo "<script>
                alert('Operação realizada com sucesso.');
             </script>";
	} else {
		echo "<script>
				alert('Falha na Operação');
				window.location.href = 'siscap.php?modulo=principal/dadosFuncionais&acao=A';
			 </script>";
		exit();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Dados Funcionais', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

$arDados = $db->pegaLinha( "SELECT dafid, dafvincservpubfed, dafsesimqual, dafmatriculasiap, dafnumpispasep,
							  dafnuminsprevsocial, dafcargoefetivo, daffuncao, daflotacao, daforgao, dafcargo,
							  dafcidade, dafestado, dafbairro, daflogradouro, dafcomplemento, dafemail, dafcep, dafoutrosvinculos, daftelecomercial, daftelefax
							FROM siscap.dadosfuncionais f WHERE facid = '$facid'" );
$arDados = $arDados ? $arDados : array();
extract( $arDados );

$daftelefax = trim($daftelefax);
$daftelecomercial = trim($daftelecomercial);

if( !empty($dafcep) ) $dafcep = substr( $dafcep, 0, 5).'-'.substr( $dafcep, 5);
if( !empty($daftelecomercial) ){
	$dafdddcomercial = substr( $daftelecomercial, 0, 2);
	$daftelecomercial = substr( $daftelecomercial, 2, 4).'-'.substr( $daftelecomercial, 6);
}
if( !empty($daftelefax) ){
	$dafdddfax = substr( $daftelefax, 0, 2);
	$daftelefax = substr( $daftelefax, 2, 4).'-'.substr( $daftelefax, 6);
}


if( !empty($dafoutrosvinculos) ){
	$display = "style=\"display: ''\";";
} else {
	$display = "style=\"display: none\";";
}

if( $dafvincservpubfed == 'S' ){
	$displaysesim = "style=\"display: ''\";";
} else {
	$displaysesim = "style=\"display: none\";";
	$display = "style=\"display: none\";";
}

$visualiza = $_SESSION['siscap']['visualizar'];
$habilita = $visualiza == 1 ? 'N' : 'S';
$habil = $habilita == 'N'  ? 'disabled="disabled"' : '';

montaCabecalhoFacilitador($facid);
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="dafid" id="dafid" value="<?=$dafid; ?>">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width="15%" class="subtitulodireita">Possui algum vínculo com o Serviço Público Federal?:</td>
			<td>
				<input name="dafvincservpubfed" onclick="verificaTipoVinculo(this.value);" value="S" <?=$habil; ?> id="dafvincservpubfedS" <?=($dafvincservpubfed == 'S' ? 'checked="checked"' : ''); ?> type="radio" title="Possui algum vínculo com o Serviço Público Federal"><label for="dafvincservpubfedS">Sim</label>
				<input name="dafvincservpubfed" onclick="verificaTipoVinculo(this.value);" value="N" <?=$habil; ?> id="dafvincservpubfedN" <?=($dafvincservpubfed == 'N' ? 'checked="checked"' : ''); ?> type="radio" title="Não"><label for="dafvincservpubfedN">Não</label>
				<img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0">
			</td>
		</tr>
		<tr id="sesim" <?=$displaysesim; ?>>
			<td class="subtitulodireita"></td>
			<td>Selecione o vínculo:
				<?
				$arQual = array(
								array("codigo" => 'AP', "descricao" => 'Ativo Permanente' ),
								array("codigo" => 'ED', "descricao" => 'Excercício Descentralizado' ),
								array("codigo" => 'CT', "descricao" => 'Contrato Temporário' ),
								array("codigo" => 'OU', "descricao" => 'Outros' )
							);

				$db->monta_combo("dafsesimqual", $arQual, $habilita, 'Selecione...', 'verificaQual', '', '', '265', 'N', 'dafsesimqual', '', '', 'Se sim, qual?' );
				?>
			</td>
		</tr>
		<tr id="qualsim" <?=$display; ?>>
			<td class="subtitulodireita">Outros Vínculos:</td>
			<td>
				<?
				echo campo_texto('dafoutrosvinculos', 'S', $habilita, 'Outros Vinculos', 80, 100, '', '', '', '', 0, 'id=dafoutrosvinculos' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Matrícula Siape:</td>
			<td>
				<?
				echo campo_texto('dafmatriculasiap', 'N', $habilita, 'Matrícula Siape', 12, 7, '', '', '', '', 0, 'id=dafmatriculasiap', '', '', 'carregaServidor(this.value);' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº do PIS/PASEP:</td>
			<td>
				<?
				echo campo_texto('dafnumpispasep', 'S', $habilita, 'Nº do PIS/PASEP', 20, 11, '', '', '', '', 0, 'id=dafnumpispasep' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº da Inscrição na Previdência Social:</td>
			<td>
				<?
				echo campo_texto('dafnuminsprevsocial', 'S', $habilita, 'Nº da Inscrição na Previdencia Social', 20, 15, '[#]', '', '', '', 0, 'id=dafnuminsprevsocial' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Cargo Efetivo:</td>
			<td>
				<?
				$arCargo = array(
								array("codigo" => 'S', "descricao" => 'SIM' ),
								array("codigo" => 'N', "descricao" => 'NÃO' )
							);

				$db->monta_combo("dafcargoefetivo", $arCargo, $habilita, 'Selecione...', '', '', '', '100', 'S', 'dafcargoefetivo', '', '', 'Cargo Efetivo' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Orgão:</td>
			<td>
				<?
				echo campo_texto('daforgao', 'S', $habilita, 'Orgão', 80, 80, '', '', '', '', 0, 'id=daforgao');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Lotação:</td>
			<td>
				<?
				echo campo_texto('daflotacao', 'S', $habilita, 'Lotação', 80, 60, '', '', '', '', 0, 'id=daflotacao' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Cargo:</td>
			<td>
				<?
				echo campo_texto('dafcargo', 'S', $habilita, 'Cargo', 80, 60, '', '', '', '', 0, 'id=dafcargo' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Função:</td>
			<td>
				<?
				echo campo_texto('daffuncao', 'N', $habilita, 'Função', 80, 80, '', '', '', '', 0, 'id=daffuncao');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">CEP:</td>
			<td>
				<?
				echo campo_texto('dafcep', 'S', $habilita, 'CEP', 15, 9, '#####-###', '', '', '', 0, 'id=dafcep', '', '', "getEnderecoPeloCEP(this.value,'');" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Cidade:</td>
			<td>
				<?
				echo campo_texto('dafcidade', 'S', $habilita, 'Cidade', 80, 100, '', '', '', '', 0, 'id=mundescricao onkeypress="return somenteLetras( event )"' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Estado:</td>
			<td>
				<?
				echo campo_texto('dafestado', 'S', $habilita, 'Estado', 15, 2, '', '', '', '', 0, 'id=estuf onkeypress="return somenteLetras( event )"' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Bairro:</td>
			<td>
				<?
				echo campo_texto('dafbairro', 'S', $habilita, 'Bairro', 80, 100, '', '', '', '', 0, 'id=endbai' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Logradouro:</td>
			<td>
				<?
				echo campo_texto('daflogradouro', 'S', $habilita, 'Logradouro', 80, 100, '', '', '', '', 0, 'id=endlog' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Complemento:</td>
			<td>
				<?
				echo campo_texto('dafcomplemento', 'N', $habilita, 'Complemento', 80, 20, '', '', '', '', 0, 'id=dafcomplemento' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">DDD/Telefone:</td>
			<td>
				<?
				echo campo_texto('dafdddcomercial', 'N', $habilita, 'DDD do Telefone', 4, 2, '##', '', '', '', 0, 'id=dafdddcomercial', "pulaCampo('formulario', '2', 'dafdddcomercial', 'daftelecomercial', event)" ).'&nbsp;';
				echo campo_texto('daftelecomercial', 'S', $habilita, 'Telefone Comercial', 12, 9, '#####-####', '', '', '', 0, 'id=daftelecomercial', '', '', 'validaTelefone( this )' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Fax:</td>
			<td>
				<?
				echo campo_texto('dafdddfax', 'N', $habilita, 'DDD do Fax', 4, 2, '##', '', '', '', 0, 'id=dafdddfax', "pulaCampo('formulario', '2', 'dafdddfax', 'daftelefax', event)" ).'&nbsp;';
				echo campo_texto('daftelefax', 'N', $habilita, 'Fax', 12, 9, '#####-####', '', '', '', 0, 'id=daftelefax' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">E-mail Profissional:</td>
			<td>
				<?
				echo campo_texto('dafemail', 'S', $habilita, 'e-mail Profissional', 80, 100, '', '', '', '', 0, 'id=dafemail' );
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
            <td></td>
			<td style="text-align: left">
				<input type="button" id="btSalvar" value="Salvar" <?=$habil; ?> onclick="salvarDadosFuncionais();" />
				<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/listaFacilitador&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<div id="erro"></div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script src="../includes/entidadesn.js"></script>
<script type="text/javascript">
	jQuery.noConflict();

function verificaTipoVinculo( valor ){
	if(valor == 'N'){
		$('sesim').style.display = 'none';
		$('qualsim').style.display = 'none';
	} else {
		$('sesim').style.display = '';
		if( $('dafsesimqual').value == 'OU' )
			$('qualsim').style.display = '';
	}
}

function validaTelefone( dados ){
	tel = dados.value;
	id = dados.id;

	if( tel != '' ){
		numero = tel.substring( 0, 4 );

		if( numero == '0000' || numero == '1111' || numero == '2222' || numero == '3333' || numero == '4444' || numero == '5555' || numero == '6666' || numero == '7777' || numero == '8888' || numero == '9999' ){
			alert( 'Telefone inválido.' );
			setTimeout("$('"+id+"').focus()",50);
			//$('daftelecomercial').focus();
		}
	}
}

function carregaServidor(matricula){
	if( matricula != '' ){
		var myajax = new Ajax.Request('siscap.php?modulo=principal/dadosFuncionais&acao=A', {
				        method:     'post',
				        parameters: '&buscadados=true&matricula='+matricula,
				        asynchronous: false,
				        onComplete: function (res){
				        	var json = res.responseText.evalJSON();
				        	$('daffuncao').setValue(json.ds_funcao+json.co_nivel_funcao);
				        	$('daflotacao').setValue(json.sg_unidade_organizacional);
				        	$('daforgao').setValue(json.ds_orgao);
				        	$('dafcargo').setValue(json.ds_cargo_emprego);
				        }
				  });
	}
}

function salvarDadosFuncionais(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	campos[0] = 'dafvincservpubfed';
	campos[1] = 'dafnumpispasep';
	campos[2] = 'dafnuminsprevsocial';
	campos[3] = 'dafcargoefetivo';
	campos[4] = 'daforgao';
	campos[5] = 'daflotacao';
	campos[6] = 'dafcargo';
	campos[7] = 'dafcep';
	campos[8] = 'dafcidade';
	campos[9] = 'dafestado';
	campos[10] = 'dafbairro';
	campos[11] = 'daflogradouro';
	campos[12] = 'dafaddcomercial';
	campos[13] = 'daftelecomercial';
	campos[14] = 'dafemail';

	tiposDeCampos[0] = 'radio';
	tiposDeCampos[1] = 'texto';
	tiposDeCampos[2] = 'texto';
	tiposDeCampos[3] = 'select';
	tiposDeCampos[4] = 'texto';
	tiposDeCampos[5] = 'texto';
	tiposDeCampos[6] = 'texto';
	tiposDeCampos[7] = 'texto';
	tiposDeCampos[8] = 'texto';
	tiposDeCampos[9] = 'texto';
	tiposDeCampos[10] = 'texto';
	tiposDeCampos[11] = 'texto';
	tiposDeCampos[12] = 'texto';
	tiposDeCampos[13] = 'texto';
	tiposDeCampos[14] = 'texto';

	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		if( $('dafvincservpubfedS').checked == true ){
			if( $('dafsesimqual').value == '' ){
				alert( 'O campo Se sim, qual? é obrigatório.' );
				$('dafsesimqual').focus();
				return false;
			}
			if( $('dafsesimqual').value == 'OU' ){
				if( $('dafoutrosvinculos').value == '' ){
					alert( 'O campo Outros Vinculos é obrigatório.' );
					$('dafoutrosvinculos').focus();
					return false;
				}
			}

		}
		if( !validaEmail( $F('dafemail') ) ){
			alert('O e-mail informado está invalido!');
			$('dafemail').focus();
			return false;
		}

		var fieldName = "#daffuncao#daflotacao#dafcidade#dafestado#dafbairro#daflogradouro";
		var labelCampo = "#Função#Lotação#Cidade#Estado#Bairro#Logradouro";

		if(naoSomenteNumeros('formulario', fieldName, labelCampo)){
			$('requisicao').value = 'salvar';
			$('formulario').submit();
		}
	}
}
function verificaQual( id ){
	if( id == 'OU' ){
		$('qualsim').style.display = '';
	} else {
		$('qualsim').style.display = 'none';
	}
}
</script>