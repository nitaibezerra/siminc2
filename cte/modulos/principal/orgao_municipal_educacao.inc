<?php

cte_verificaSessao();

if( isset( $_REQUEST['servico'] ) &&  $_REQUEST['servico'] == 'listar_mun' ){	
	$sql = "SELECT muncod, mundescricao as mundsc 
			FROM territorios.municipio 
			WHERE estuf = '".$_REQUEST['estuf']."' ORDER BY mundsc";
	$dados = $db->carregar($sql);
	
	$enviar = '';
	if($dados) $dados = $dados; else $dados = array();
			$enviar .= "<option value=\"\"> Todos </option> \n";
	foreach ($dados as $data) {
		$enviar .= "<option value= ".$data['muncod'].">  ".simec_htmlentities($data['mundsc'])." </option> \n";
	}
	die( $enviar );
}

require_once( APPRAIZ. "www/includes/webservice/pj.php" );
include_once( APPRAIZ. "cte/classes/DadosUnidade.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/territorios/Estado.class.inc" );


//Em Fase de Análise - Equipe Técnica não permitir editar parecer 
//$habil = cte_podeEditarParecer($_SESSION['inuid']) ? 'S' : 'N';

// Habilitar para todos momentaneamente
//$habil = 'S';
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
$idDadosUnidade = $obDadosUnidade->pegaUm( "select dunid from cte.dadosunidade where tduid = 1 and inuid = ". $_SESSION["inuid"] );

if( $idDadosUnidade ){
	$obDadosUnidade->carregarPorId( $idDadosUnidade );
}

if( isset( $_REQUEST['formulario'] ) && $habil == 'S' ){
	
	$arDados = array( "dunid", "inuid", "dunnome", "duntelddd", "dunemail", "dunendereco", "muncod", "estuf" );
		
	$obDadosUnidade->popularObjeto( $arDados );
	
	$obDadosUnidade->tduid  = CTE_TIPO_DADOS_UNIDADE_ORGAO_MUNICIPAL;
	$obDadosUnidade->duncnpj = ereg_replace("[^0-9]", "", $_REQUEST["duncnpj"] );
	$obDadosUnidade->duncep = ereg_replace("[^0-9]", "", $_REQUEST["duncep"] );
	$obDadosUnidade->duntel = ereg_replace("[^0-9]", "", $_REQUEST["duntel"] );
	
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
			<td class="SubTituloDireita" width="35%">CNPJ:</td>
			<td>
				<? echo campo_texto("duncnpj", 'S', $habil, '', '25', '', '##.###.###/####-##', '', '', '', '', 'id="duncnpj"', '', $obDadosUnidade->duncnpj, 'validarCNPJ( this ); procurarNome( this.value )'); ?>
			</td>
		</tr>
		<tr >
			<td class="SubTituloDireita" width="35%">Nome:</td>
			<td>
				<? echo campo_texto("dunnome", 'S', 'N', '', '30', '', '', '', '', '', '', 'id="dunnome"', '', $obDadosUnidade->dunnome ); ?>
			</td>
		</tr>
		<tr >
			<td class="SubTituloDireita" width="35%">Endereço:</td>
			<td>
				<? echo campo_texto("dunendereco", 'S', $habil, '', '70', '', '', '', '', '', '', 'id="dunendereco"', '', $obDadosUnidade->dunendereco ); ?>
			</td>
		</tr>
		<tr >
			<td class="SubTituloDireita" width="35%">Unidade Federativa:</td>
			<td>
				<?php $obEstado = new Estado();
				$coEstado = $obEstado->recuperarTodos( "estuf as codigo, estuf as descricao", array(), estuf );
				$obEstado->monta_combo('estuf', $coEstado, $habil, "UF", 'listar_municipios', '', '', '', 'S', 'estuf', '', $obDadosUnidade->estuf ); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Município:</td>
			<td>
				<div id="muncod_on" style="display:none;"><select id="muncod" name="muncod" class="CampoEstilo"></select></div>
				<div id="muncod_off" style="color:#909090;">
					<select id="muncodOff" name="muncodOff" class="CampoEstilo" >
						<option value="">Selecione uma UF</option>
					</select>
					</div>
			</td>
		</tr>		
		<tr >
			<td class="SubTituloDireita" width="35%">CEP:</td>
			<td>
				<? echo campo_texto("duncep", 'S', $habil, '', '12', '', '#####-###', '', '', '', '', 'id="duncep"', '', $obDadosUnidade->duncep ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="35%">Telefone Geral:</td>
			<td>
				<? echo campo_texto("duntelddd", 'S', $habil, '', '3', '', '##', '', '', '', '', 'id="duntelddd"', '', $obDadosUnidade->duntelddd ); ?>
				<? echo campo_texto("duntel", 'S', $habil, '', '12', '', '####-####', '', '', '', '', 'id="duntel"', '', $obDadosUnidade->duntel ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="35%">E-mail Institucional:</td>
			<td>
				<? echo campo_texto("dunemail", 'S', $habil, '', '30', '', '', '', '', '', '', 'id="dunemail"', '', $obDadosUnidade->dunemail ); ?>
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
	
	function validarCNPJ( obj ){

		if( obj.value ){
			if( !validarCnpj( obj.value  ) ){
				alert( "CNPJ inválido!\nFavor informar um CNPJ válido!" );
				obj.value = "";	
				document.getElementById( "dunnome" ).value = "";
			}
		}	
		
	}
	
	function procurarNome( cnpj ){

		var comp = new dCNPJ();
		var nome = document.getElementById( 'dunnome' );
		comp.buscarDados( cnpj );
		
		if( comp.dados.no_empresarial_rf != '' ){
			nome.value = comp.dados.no_empresarial_rf;
			nome.readOnly = true;
		}
	}	
	
	function validacaoFormulario( id ){
		
		var campos = "";
		
		if( document.getElementById( "duncnpj" ).value == "" ) campos += " - CNPJ\n\r";
		if( document.getElementById( "duntelddd" ).value == "" ) campos += " - DDD do Telefone Institucional\n\r";
		if( document.getElementById( "duntel" ).value == "" ) campos += " - Telefone Institucional\n\r";
		if( document.getElementById( "dunemail" ).value == "" ) campos += " - E-mail Institucional\n\r";
		if( document.getElementById( "estuf" ).value == "" ) campos += " - UF\n\r";
		if( document.getElementById( "muncod" ).value == "" ) campos += " - Município\n\r";
		if( document.getElementById( "dunendereco" ).value == "" ) campos += " - Endereço\n\r";
		if( document.getElementById( "duncep" ).value == "" ) campos += " - CEP\n\r";
				
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

	var uf = '<?php echo $obDadosUnidade->estuf ?>';
	if( uf ){
	
    	validar_mun = true;    
        var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );        
		div_on.style.display = 'block';
		div_off.style.display = 'none';	
	
		new Ajax.Updater('muncod', '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=sistema/usuario/consusuario&acao=<?=$_REQUEST['acao'] ?>',
         {
         	
            method: 'post',
            parameters: '&servico=listar_mun&estuf=' + uf,
            onComplete: function(res)
            {	
            	var arOptions = document.getElementById( "muncod" ).options;
            	arOptions[0].innerHTML = "Selecione";
            	for( i=0; i<arOptions.length; i++ ){
            		if( arOptions[i].value == '<?php echo $obDadosUnidade->muncod ?>' ){
	            		arOptions[i].selected = true;
            		}
            	}
            }
        });
	}
	
	function listar_municipios( estuf )
    {
    	validar_mun = true;    
        var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );        
		div_on.style.display = 'block';
		div_off.style.display = 'none';
				
        return new Ajax.Updater( 'muncod', window.location.href,
         {
            method: 'post',
            parameters: '&servico=listar_mun&estuf=' + estuf,
            onComplete: function(res)
            {	
            	var arOptions = document.getElementById( "muncod" ).options;
            	arOptions[0].innerHTML = "Selecione";
            }
        });
    }	
	
</script>
