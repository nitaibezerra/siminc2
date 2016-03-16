<?php

cte_verificaSessao();

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
$stCampos = " dunid, duncpf, inuid, dunnome, dunfuncao, dunemail, sgmid, dunsegmento ";
$coDadosUnidade = $obDadosUnidade->recuperarTodos( $stCampos, array( "inuid = ". $_SESSION["inuid"], "tduid = 4" ) );

$coSegmentos = $obDadosUnidade->recuperarSegmentos();

if( isset( $_REQUEST['formulario'] ) && $habil == 'S' ){
	
	//dbg( $_REQUEST, 1 );
	
	if( $_REQUEST["duncpf"] ){

		$obDadosUnidade = new DadosUnidade();
		if( $_REQUEST["dunid"] ){
			$obDadosUnidade->carregarPorId( $_REQUEST["dunid"] );
		}
		
		$arDados = array( "dunid", "inuid", "dunnome", "dunemail", "dunfuncao", "dunsegmento"  );
		$obDadosUnidade->popularObjeto( $arDados );
		
		$obDadosUnidade->tduid  = CTE_TIPO_DADOS_UNIDADE_COMITE_LOCAL_COORDENADOR;
		$obDadosUnidade->duncpf = ereg_replace("[^0-9]", "", $_REQUEST["duncpf"] );
		
		$obDadosUnidade->salvar();
		$obDadosUnidade->commit();
		unset( $obDadosUnidade );
	}
	
	if( is_array( $_REQUEST["ardun"] ) ){
		
		foreach( $_REQUEST["ardun"] as $count ){

			$obNovo = "obDadosUnidade_$count";
			$$obNovo = new DadosUnidade();

			$dunid = $_REQUEST["dunid_$count"];
			
			if( $dunid ) $$obNovo->carregarPorId( $dunid );
			
			$eescpf = "duncpf_$count";
			$dunnome = "dunnome_$count";
			$dunfuncao = "dunfuncao_$count";
			$sgmid = "sgmid_$count";
			$dunemail = "dunemail_$count";
			$dunsegmento = "dunsegmento_$count";
			
			if( $_REQUEST["duncpf_$count"] ){
				
				$$obNovo->inuid = $_REQUEST["inuid"];
				$$obNovo->duncpf = ereg_replace("[^0-9]", "", $_REQUEST[$eescpf] );
				$$obNovo->dunnome = $_REQUEST[$dunnome];
				$$obNovo->dunfuncao = $_REQUEST[$dunfuncao];
				$$obNovo->dunsegmento = $_REQUEST[$dunsegmento];
				if( $_REQUEST[$sgmid] ){
					$$obNovo->sgmid = $_REQUEST[$sgmid];
				}				
				$$obNovo->dunemail = $_REQUEST[$dunemail];
				$$obNovo->tduid  = CTE_TIPO_DADOS_UNIDADE_COMITE_LOCAL;

				$$obNovo->salvar();
				$$obNovo->commit();
				unset( $$obNovo );
			}
		}
	    $db->sucesso( $modulo );
	}
	
}

	
if( $_REQUEST['req'] == 'removerLinha' && $habil == 'S' ){
	
	if( $_REQUEST["dunidExcluir"] ){
		$obDadosUnidade = new DadosUnidade();
		$obDadosUnidade->excluir( $_REQUEST["dunidExcluir"] );
		$obDadosUnidade->commit();
		unset( $obDadosUnidade );
		die();
	}
}

require_once( APPRAIZ. "www/includes/webservice/cpf.php" );
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&indid='. $_REQUEST['indid'] );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );

$obDadosUnidade = new DadosUnidade();
$idDadosUnidade = $obDadosUnidade->pegaUm( "select dunid from cte.dadosunidade where tduid = 5 and inuid = ". $_SESSION["inuid"] );
if( $idDadosUnidade ){
	$obDadosUnidade->carregarPorId( $idDadosUnidade );	
}

?>

<form action="" method="post" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="inuid" value="<?php echo $_SESSION["inuid"]; ?>"/>

	<table class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="95%" >
		<thead>
			<tr align="center" style=" background: #ccc; color: #000;">
				<td colspan="6" style="font-weight: bold; font-size: 15px;">Coordenador</td>	
			</tr>	
			<tr align="center">
				<td width="3%">&nbsp;</td>	
				<td width="12%">CPF</td>
				<td width="15%">Nome</td>	
				<td width="30%">Segmento/Órgão que representa</td>	
				<td width="15%">Função/Profissão</td>	
				<td width="20%">Email</td>	
			</tr>	
		</thead>
		<tbody>	
			<tr align="center" id="linha_<?php echo $i ?>" >
				<td>&nbsp;</td>
				<td>
					<input type="hidden" id="dunid" name="dunid" value="<?php echo $obDadosUnidade->dunid ?>" />
					<? echo campo_texto("duncpf", 'S', $habil, '', '18', '', '###.###.###-##', '', '', '', '', 'id="duncpf"', '', $obDadosUnidade->duncpf, 'validarduncpf( this ); procurarNomeCoordenador( this.value );'); ?>
				</td>
				<td>
					<label id="nomeCoordenador"><?php echo $obDadosUnidade->dunnome ?></label>
					<input type="hidden" id="dunnome" name="dunnome" value="<?php echo $obDadosUnidade->dunnome ?>" />
				</td>
				<td>
					<? echo campo_texto("dunsegmento", 'N', $habil, '', '48', '', '', '', '', '', '', 'id="dunsegmento"', '', $obDadosUnidade->dunsegmento ); ?>
				</td>
				<td>
					<? echo campo_texto("dunfuncao", 'N', $habil, '', '30', '', '', '', '', '', '', 'id="dunfuncao"', '', $obDadosUnidade->dunfuncao ); ?>
				</td>
				<td>
					<? echo campo_texto("dunemail", 'N', $habil, '', '30', '50', '', '', '', '', '', 'id="dunemail"', '', $obDadosUnidade->dunemail ); ?>
				</td>
			</tr>
		</tbody>
	</table>
	<br />
	<table id="tabelaEquipeLocal" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="95%" >
		<thead>
			<tr align="center" style=" background: #ccc; color: #000;">
				<td colspan="6" style="font-weight: bold; font-size: 15px;">Integrantes</td>	
			</tr>	
			<tr align="center">
				<td width="3%">&nbsp;</td>	
				<td width="12%">CPF</td>	
				<td width="15%">Nome</td>	
				<td width="30%">Segmento/Órgão que representa</td>	
				<td width="15%">Função/Profissão</td>	
				<td width="20%">Email</td>	
			</tr>	
		</thead>
		<tbody id="tbodyTabela">	
		</tbody>
	</table>
	<?php if ($habil=='S'): ?>
		<span style="margin-left: 3%; _margin-left: 30px; *margin-left: 30px; cursor:pointer" id="linkInserirLinha" onclick="return carregarEquipe( '', '', '', '', '', '', '' );"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Inserir Integrante</span>
	<?php endif; ?>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0" align="center">
			<td>
				<?php if ($habil=='S'): ?>
					<input type='button' class="botao" name='cadastra' value='Salvar' onclick="salvar();"/>
				<?php endif; ?>
					<input type='button' class="botao" name='fecha' value='Voltar' onclick="fechar();"/>
			</td>
		</tr>
	</table>		
</form>

<br>

<script language="javascript" type="text/javascript">
<!--
	
	function removerLinha( idLinha, dunid ){
	
		if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
			return false;
		}

		return new Ajax.Request(window.location.href,
	                                   {
	                                       method: 'post',
	                                       parameters: '&req=removerLinha&dunidExcluir=' + dunid,
	                                       onComplete: function(res)
	                                       {
												var tabela 	= document.getElementById( "tabelaEquipeLocal" );
												var linha 	= document.getElementById( idLinha );
												tabela.deleteRow( linha.rowIndex );
	                                       }
	                                   });
	                                   	

		
	}
	
	function procurarNome( cpf, qtd ){
		var data = '';
		var comp = new dCPF();
		var arNome = document.getElementById( 'dunnome_' + qtd );
		var label = document.getElementById( 'nome_' + qtd );
		
		comp.buscarDados( cpf );
		if (comp.dados.no_pessoa_rf != ''){
			arNome.value = comp.dados.no_pessoa_rf;
			label.innerHTML = comp.dados.no_pessoa_rf;
			arNome.readOnly = true;
		}
	}
	
	function procurarNomeCoordenador( cpf ){
		
		var comp = new dCPF();
		var arNome = document.getElementById( 'dunnome' );
		var label = document.getElementById( 'nomeCoordenador' );
		
		comp.buscarDados( cpf );
		if (comp.dados.no_pessoa_rf != ''){
			arNome.value = comp.dados.no_pessoa_rf;
			label.innerHTML = comp.dados.no_pessoa_rf;
		}
	}

	function eventosIE( obj, tipo ){
		
		switch( tipo ){
			case('1'): MouseBlur(obj); validarduncpf( obj ); break;
			case('2'): MouseOut(obj); break;
			case('3'): MouseClick(obj); obj.select(); break;
			case('4'): MouseOver(obj); break;
			case('5'): obj.value=mascaraglobal('###.###.###-##',obj.value); break;
		}
		
	}
	
	function eventosIECPF( obj, qtd ){
		procurarNome( obj.value, qtd );
	}
	
	function validarduncpf( obj ){
		
		if( obj.value ){
			if( !validar_cpf( obj.value  ) ){
				alert( "CPF inválido!\nFavor informar um cpf válido!" );
				obj.value = "";	
			}
		}
	}
	function carregarEquipe( dunid, duncpf, dunnome, dunfuncao, sgmid, dunemail, dunsegmento ){

		var tabela 	= document.getElementById( "tabelaEquipeLocal" );
		var tbody   = document.getElementById( "tbodyTabela" );
		var qtdLinha = tabela.rows.length;
		var qtd = qtdLinha - 1;
												
		linha = tabela.insertRow( tabela.rows.length );
		tbody.appendChild( linha );
	
		linha.setAttribute( "id", "linha_" + qtd );
		linha.setAttribute( "style", "background: #f5f5f5;" );
		linha.setAttribute( "align", "center" );
												
		var img = document.createElement( "img" );
		img.setAttribute( "src", "/imagens/excluir.gif" );
		img.setAttribute( "title", "Excluir" );
		img.setAttribute( "alt", "Excluir" );

		// OnClick
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			img.setAttribute( "onclick", "removerLinha( 'linha_" + qtd + "', '"+ dunid +"' )" );
		}
		else{ // IE
			img.attachEvent( "onclick", function() { removerLinha( "linha_" + qtd, dunid ) } );
		}
		
        var input_dunid     = document.createElement('input');
        input_dunid.setAttribute('type', 'hidden');
        input_dunid.setAttribute('name', 'dunid_' + qtd);
        input_dunid.setAttribute('value', dunid);									
						
		var cpf = document.createElement( "input" );
		cpf.setAttribute( "type", "text" );
		cpf.setAttribute( "name", "duncpf_" + qtd );
		cpf.setAttribute( "id", "duncpf_" + qtd );
		cpf.setAttribute( "value", duncpf );
		cpf.setAttribute( "style", "width: 21ex;" );
		cpf.setAttribute( "size", "19" );
		cpf.className = "normal";
		
		// OnBlur
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			cpf.setAttribute( "onblur", "MouseBlur(this); validarduncpf( this );" );
		}
		else{ // IE
			cpf.attachEvent( "onblur", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "1" ); } );
		}
		
		// OnMouseOut
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			cpf.setAttribute( "onmouseout", "MouseOut(this);" );
		}
		else{ // IE
			cpf.attachEvent( "onmouseout", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "2" ); } );
		}
		
		// OnFocus
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			cpf.setAttribute( "onfocus", "MouseClick(this); this.select();" );
		}
		else{ // IE

			cpf.attachEvent( "onfocus", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "3" ); } );
		}
		
		// OnMouseOver
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			cpf.setAttribute( "onmouseover", "MouseOver(this);" );
		}
		else{ // IE
			cpf.attachEvent( "onmouseover", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "4" ); } );
		}
		
		// OnKeyUp
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			cpf.setAttribute( "onkeyup", "this.value=mascaraglobal('###.###.###-##',this.value);" );
		}
		else{ // IE
			cpf.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}
		
		// OnChange
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			cpf.setAttribute( "onchange", "procurarNome( this.value, "+ qtd +" )" );
		}
		else{ // IE
			cpf.attachEvent( "onkeyup", function() { eventosIECPF( document.getElementById( 'duncpf_' + qtd ), qtd ); } );
		}
		
		// OnKeyUp
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			cpf.setAttribute( "onkeyup", "this.value=mascaraglobal('###.###.###-##',this.value);" );
		}
		else{ // IE
			cpf.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}

		var nome = document.createElement( "input" );
		nome.setAttribute( "type", "hidden" );
		nome.setAttribute( "name", "dunnome_" + qtd );
		nome.setAttribute( "id", "dunnome_" + qtd );
		nome.setAttribute( "value", dunnome );
		
		var label = document.createElement( "label" );
		label.setAttribute( "id", "nome_" + qtd );
		label.innerHTML = dunnome;
				
		var ardun = document.createElement( "input" );
		ardun.setAttribute( "type", "hidden" );
		ardun.setAttribute( "name", "ardun[]" );
		ardun.setAttribute( "value", qtd );
          
		celula = linha.insertCell( 0 );
		celula.appendChild( img );
		celula.appendChild( ardun );
		
		var imgObrigatorio = document.createElement( "img" );
		imgObrigatorio.setAttribute( "src", "../imagens/obrig.gif" );
		imgObrigatorio.setAttribute( "title", "Indica campo obrigatório." );												
		
		celula = linha.insertCell( 1 );
		celula.appendChild( input_dunid );
		celula.appendChild( cpf );
		celula.appendChild( imgObrigatorio );
		
		celula = linha.insertCell( 2 );
		celula.appendChild( nome );
		celula.appendChild( label );
		
		segmento = document.createElement( "input" );
		segmento.setAttribute( "type", "text" );
		segmento.setAttribute( "name", "dunsegmento_" + qtd );
		segmento.setAttribute( "id", "dunsegmento_" + qtd );
		segmento.setAttribute( "value", dunsegmento );
		segmento.setAttribute( "style", "width: 50ex;" );
		segmento.setAttribute( "size", "50" );
		segmento.className = "normal";
											
		celula = linha.insertCell( 3 );
		celula.appendChild( segmento );
		
		var funcao = document.createElement( "input" );
		funcao.setAttribute( "type", "text" );
		funcao.setAttribute( "name", "dunfuncao_" + qtd );
		funcao.setAttribute( "id", "dunfuncao_" + qtd );
		funcao.setAttribute( "value", dunfuncao );
		funcao.setAttribute( "style", "width: 33ex;" );
		funcao.setAttribute( "size", "30" );
		funcao.className = "normal";
											
		celula = linha.insertCell( 4 );
		celula.appendChild( funcao );
		
		
		var email = document.createElement( "input" );
		email.setAttribute( "type", "text" );
		email.setAttribute( "name", "dunemail_" + qtd );
		email.setAttribute( "id", "dunemail_" + qtd );
		email.setAttribute( "value", dunemail );
		email.setAttribute( "style", "width: 33ex;" );
		email.setAttribute( "size", "30" );
		email.className = "normal";
		email.maxLength = "50";
											
		celula = linha.insertCell( 5 );
		celula.appendChild( email );

	}
	
	function salvar(){
	
		document.formulario.action = '';
		document.formulario.submit();
	}

	
	function fechar(){
		window.location.href = '/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A';
	}
	
--></script>

<style type="text/css">
	.comboEstilo{
		border-right: #888888 1px solid;
	    border-top: #888888 1px solid;
	    font-size: 10px;
	    vertical-align: middle;
	    border-left: #000000 3px solid;
	    border-bottom: #888888 1px solid;
	    font-family: verdana;
	    background-color: #ffffff;
	    width: 300px;
	}
</style>

<?php 
	foreach( $coDadosUnidade as $qtd => $arDadosUnidade ){
		echo "<script type='text/javascript'>carregarEquipe( '{$arDadosUnidade["dunid"]}', '{$arDadosUnidade["duncpf"]}', '{$arDadosUnidade["dunnome"]}', '{$arDadosUnidade["dunfuncao"]}', '{$arDadosUnidade["sgmid"]}', '{$arDadosUnidade["dunemail"]}', '{$arDadosUnidade["dunsegmento"]}' );</script>";
	}
?>