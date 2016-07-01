<?php

cte_verificaSessao();

require_once( APPRAIZ. "www/includes/webservice/cpf.php" );
include_once( APPRAIZ. "cte/classes/DadosUnidade.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/territorios/Estado.class.inc" );

foreach ( $_REQUEST as $chave => $valor ) {
	if ( is_string( $valor ) ) {
		$_REQUEST[$chave] = stripslashes( str_replace("'","''", trim( stripslashes( $valor ) ) ) );
	}
}

//Em Fase de Análise - Equipe Técnica não permitir editar parecer 
//$habil = cte_podeEditarParecer($_SESSION['inuid']) ? 'S' : 'N';

// Habilitar para todos momentaneamente
$perfis = cte_arrayPerfil();
if( ( count($perfis == 1)) && ( in_array(CTE_PERFIL_CONSULTA_GERAL, $perfis ) 	|| 
								in_array(CTE_PERFIL_CONSULTA_ESTADUAL, $perfis ) || 
								in_array(CTE_PERFIL_CONSULTA_MUNICIPAL, $perfis ) )){
	//não altera
	$habil = 'N';
}else{
	//pode alterar
	$habil = 'S';
}	

$obDadosUnidade = new DadosUnidade();
$idDadosUnidade = $obDadosUnidade->pegaUm( "select dunid from cte.dadosunidade where tduid = 2 and inuid = ". $_SESSION["inuid"] );

if( $idDadosUnidade ){
	$obDadosUnidade->carregarPorId( $idDadosUnidade );
}

if( isset( $_REQUEST['formulario'] ) && $habil == 'S' ){
	
	$arDados = array( "dunid", "inuid", "dunnome", "dunrg", "dunorgaorg", "dunufrg", "duntelddd", "duncelddd", "dunemail", "dunemail2"  );
		
	$obDadosUnidade->popularObjeto( $arDados );
	
	$obDadosUnidade->tduid  = CTE_TIPO_DADOS_UNIDADE_DIRIGENTE_MUNICIPAL;
	$obDadosUnidade->duncpf = ereg_replace("[^0-9]", "", $_REQUEST["duncpf"] );
	$obDadosUnidade->duntel = ereg_replace("[^0-9]", "", $_REQUEST["duntel"] );
	$obDadosUnidade->duncel = ereg_replace("[^0-9]", "", $_REQUEST["duncel"] );
	
	$obDadosUnidade->salvar();
	$obDadosUnidade->commit();
    $obDadosUnidade->sucesso( $modulo );
	
}

include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&indid='. $_REQUEST['indid'] );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );

?>

<form action="" method="post" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="inuid" value="<?php echo $_SESSION["inuid"]; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr >
			<td class="SubTituloDireita" width="35%">CPF:</td>
			<td>
				<? echo campo_texto("duncpf", 'S', $habil, '', '18', '', '###.###.###-##', '', '', '', '', 'id="duncpf"', '', $obDadosUnidade->duncpf, 'validarCPF( this ); procurarNome( this.value )'); ?>
			</td>
		</tr>
		<tr >
			<td class="SubTituloDireita" width="35%">Nome:</td>
			<td>
				<? echo campo_texto("dunnome", 'S', 'N', '', '30', '', '', '', '', '', '', 'id="dunnome"', '', $obDadosUnidade->dunnome ); ?>
			</td>
		</tr>
		<tr >
			<td class="SubTituloDireita" width="35%">RG:</td>
			<td>
				<? echo campo_texto("dunrg", 'S', 'S', '', '15', '', '[#]', '', '', '', '', 'id="dunrg"', '', $obDadosUnidade->dunrg ); ?>
				<span style="margin-left: 20px;">Órgão Expedidor: </span>
				<? echo campo_texto("dunorgaorg", 'S', 'S', '', '7', '', '', '', '', '', '', 'id="dunorgaorg"', '', $obDadosUnidade->dunorgaorg ); ?>
				<span style="margin-left: 20px;">UF: </span> 
				<?php $obEstado = new Estado();
				$coEstado = $obEstado->recuperarTodos( "estuf as codigo, estuf as descricao", array(), estuf );
				$obEstado->monta_combo('dunufrg', $coEstado, $habil, "UF", '', '', '', '', 'S', 'dunufrg', '', $obDadosUnidade->dunufrg ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="35%">Telefone do Órgão/Gabinete:</td>
			<td>
				<? echo campo_texto("duntelddd", 'S', $habil, '', '3', '', '##', '', '', '', '', 'id="duntelddd"', '', $obDadosUnidade->duntelddd ); ?>
				<? echo campo_texto("duntel", 'S', $habil, '', '12', '', '####-####', '', '', '', '', 'id="duntel"', '', $obDadosUnidade->duntel ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="35%">Telefone Celular:</td>
			<td>
				<? echo campo_texto("duncelddd", 'S', $habil, '', '3', '', '##', '', '', '', '', 'id="duncelddd"', '', $obDadosUnidade->duncelddd ); ?>
				<? echo campo_texto("duncel", 'S', $habil, '', '12', '', '####-####', '', '', '', '', 'id="duncel"', '', $obDadosUnidade->duncel ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="35%">E-mail Institucional:</td>
			<td>
				<? echo campo_texto("dunemail", 'S', $habil, '', '30', '', '', '', '', '', '', 'id="dunemail"', '', $obDadosUnidade->dunemail ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="35%">E-mail Particular:</td>
			<td>
				<? echo campo_texto("dunemail2", 'S', $habil, '', '30', '', '', '', '', '', '', 'id="dunemail2"', '', $obDadosUnidade->dunemail2 ); ?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td ></td>
			<td>
				<div style="float: left;">
					<?php if ($habil=='S'): ?>
						<input type='button' class="botao" name='cadastra' value='Salvar' onclick="salvar();"/>
					<?php endif; ?>
						<input type='button' class="botao" name='fecha' value='Voltar' onclick="fechar();"/>
				</div>
			</td>
		</tr>
	</table>
</form>

<br>

<script language="javascript" type="text/javascript">
	
	function validarCPF( obj ){
		
		if( obj.value ){
			if( !validar_cpf( obj.value  ) ){
				alert( "CPF inválido!\nFavor informar um cpf válido!" );
				obj.value = "";	
				document.getElementById( "dunnome" ).value = "";
			}
		}
	}
	
	function procurarNome( cpf ){

		var comp = new dCPF();
		var nome = document.getElementById( 'dunnome' );
		var rg = document.getElementById( 'dunrg' );
		var orgaorg = document.getElementById( 'dunorgaorg' );
		var ufrg = document.getElementById( 'dunufrg' );
		
		comp.buscarDados( cpf );
		
		if( comp.dados.no_pessoa_rf != '' ){
			nome.value = comp.dados.no_pessoa_rf;
			rg.value = comp.dados.nu_rg;
			orgaorg.value = comp.dados.ds_orgao_expedidor_rg;
			nome.readOnly = true;
		}
	}	
	
	function validacaoFormulario( id ){
		
		var campos = "";
		
		if( document.getElementById( "duncpf" ).value == "" ) campos += " - CPF\n\r";
		if( document.getElementById( "dunrg" ).value == "" ) campos += " - RG\n\r";
		if( document.getElementById( "dunorgaorg" ).value == "" ) campos += " - Órgão Expedidor\n\r";
		if( document.getElementById( "dunufrg" ).value == "" ) campos += " - UF Órgão Expedidor\n\r";
		if( document.getElementById( "duntelddd" ).value == "" ) campos += " - DDD do Telefone Institucional\n\r";
		if( document.getElementById( "duntel" ).value == "" ) campos += " - Telefone Institucional\n\r";
		if( document.getElementById( "duncelddd" ).value == "" ) campos += " - DDD do Telefone Celular\n\r";
		if( document.getElementById( "duncel" ).value == "" ) campos += " - Telefone Celular\n\r";
		if( document.getElementById( "dunemail" ).value == "" ) campos += " - E-mail Institucional\n\r";
		if( document.getElementById( "dunemail2" ).value == "" ) campos += " - E-mail Particular\n\r";
				
		if( campos ){
			alert( "Favor preencher o(s) campo(s) listado(s) abaixo:\n\r\n\r" + campos );
			return false;
		}		
		
		return true;
	}
	
	function salvar(){
	
		if( validacaoFormulario( 1 ) ){
			document.formulario.action = '';
			document.formulario.submit();
		}		
	}

	
	function fechar(){
		window.location.href = '/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A';
	}
	
</script>