<?php

cte_verificaSessao();

require_once( APPRAIZ. "www/includes/webservice/cpf.php" );
include_once( APPRAIZ. "cte/classes/DadosUnidade.class.inc" );
include_once( APPRAIZ. "cte/classes/InstrumentoUnidade.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/territorios/Estado.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/public/TipoVinculoProfissional.class.inc" );

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
$idDadosUnidade = $obDadosUnidade->pegaUm( "select dunid from cte.dadosunidade where tduid = 6 and inuid = ". $_SESSION["inuid"] );

if( $idDadosUnidade ){
	$obDadosUnidade->carregarPorId( $idDadosUnidade );
}

if( isset( $_REQUEST['formulario'] ) && $habil == 'S' ){
	
	$arDados = array( "dunid", "inuid", "dunnome", "dunrg", "dunorgaorg", "dunufrg", "dunfuncao", "dunvinculo", "tvpid" );
		
	$obDadosUnidade->popularObjeto( $arDados );
	$obDadosUnidade->tduid  = 6;
	$obDadosUnidade->duncpf = ereg_replace("[^0-9]", "", $_REQUEST["duncpf"] );
	
	$obDadosUnidade->salvar();
	$obDadosUnidade->commit();
    $obDadosUnidade->sucesso( $modulo );
	
}

include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&indid='. $_REQUEST['indid'] );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );

$obInstrumentoUnidade = new InstrumentoUnidade( $_SESSION["inuid"] );
if( $obInstrumentoUnidade->inusituacaoadesao == 1 ){ ?>

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
				<td class="SubTituloDireita" width="35%">Vínculo:</td>
				<td>
					<?php $obTipoVinculoProfissional = new TipoVinculoProfissional();
					$coTipoVinculoProfissional = $obTipoVinculoProfissional->recuperarTodos( "tvpid as codigo, tvpdsc as descricao", array(), 'tvpdsc' );
					$obTipoVinculoProfissional->monta_combo('tvpid', $coTipoVinculoProfissional, $habil, "Selecione", 'alternarExibicaoVinculo', '', '', '', 'N', 'tvpid', '', $obDadosUnidade->tvpid ); ?>
					
					<?= campo_texto('dunvinculo','N',$editavel,'', '15', '','','', '', '', '', 'id="dunvinculo" style="display: none;"', '', $obDadosUnidade->dunvinculo); ?>
					<a href="javascript: alternarExibicaoVinculo( 'exibirOpcoes' )" id="linkVoltar" style="display: none;" > Exibir Opções</a>				
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="35%">Cargo/Função:</td>
				<td>
					<?php echo campo_texto("dunfuncao", 'S', 'S', '', '15', '', '', '', '', '', '', 'id="dunfuncao"', '', $obDadosUnidade->dunfuncao ); ?>
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

<?php }
else{ ?>

	<h1 style="color: red; text-align: center;" >Dados de Escola Ativa não encontrados!</h1>

<?php } ?>

<br>

<script language="javascript" type="text/javascript">
	
	function alternarExibicaoVinculo( tipo ){
		
		var tvpid = document.getElementById( 'tvpid' );
		var dunvinculo = document.getElementById( 'dunvinculo' );
		var link = document.getElementById( 'linkVoltar' );
		
		
		if( tipo != 'exibirOpcoes' ){
			if( tvpid.value == 4 ){
				dunvinculo.style.display = "";
				//dunvinculo.className = "";
				link.style.display = "";
				tvpid.style.display = "none";
				//link.className = "";
			}
		}
		else{
			dunvinculo.style.display = "none";
			dunvinculo.value = "";
			link.style.display = "none";
			//link.className = "objetoOculto";
			tvpid.style.display = "";
			tvpid.value = "";
		}
	}		
	
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
		if( document.getElementById( "dunfuncao" ).value == "" ) campos += " - Cargo/Função\n\r";
		if( document.getElementById( "tvpid" ).value == "" ) campos += " - Vínculo\n\r";
				
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

<?php if( $obDadosUnidade->tvpid == 4 ){
	echo "<script type='text/javascript'>alternarExibicaoVinculo( 4 );</script>";
} ?>