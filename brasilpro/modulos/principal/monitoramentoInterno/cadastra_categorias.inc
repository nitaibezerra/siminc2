<?php

if( $_POST['acao'] == 'gravar' || $_POST['acao'] == 'salvar' ){
	
	header('Content-type: text/html;charset=ISO-8859-1');
	
	global $db;
	
	if( $_POST['acao'] == 'salvar' ){
		
		$catid = $_POST['catid'];
		
		$sql = "UPDATE cte.categoria
				SET cordsc = '".utf8_decode($_POST['novaCategoria'])."'
				WHERE catid = ".$catid;
		
		$db->executar($sql);

		unset($sql);
		
		$sql = "DELETE FROM cte.fasescategoria
				WHERE catid = ".$catid;
		
		$db->executar($sql);
		
		unset($sql);
		
	}else{
		
		$sql = "INSERT INTO cte.categoria(cordsc)
				VALUES ('".utf8_decode($_POST['novaCategoria'])."')
				RETURNING catid";
		
		$catid = $db->pegaUm($sql);
		
		unset($sql);
		
	}
	
	
	for($x=1;$x<$_POST['fasesQtd'];$x++){
		unset($sql);
		$sql = 'INSERT INTO cte.fasescategoria (catid,facdsc,facordem,facobservacao,facpeso)
				VALUES ('.$catid.',
					  \''.utf8_decode($_POST['fase'.$x]).'\',
					  \''.$_POST['ordem'.$x].'\',
					  \''.utf8_decode($_POST['observacao'.$x]).'\',
					  \''.$_POST['peso'.$x].'\')';
		$db->executar($sql);
	}
	
	$db->commit();
	
	die();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$res = array( 0 =>
					array ("descricao" => "Pesquisa de Categorias",
							    "id"        => "1",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_categorias&acao=A"
							   )
			);

if( $db->testa_superuser() ){	
		array_push($res,
					array ("descricao" => "Cadastrar Categorias",
							    "id"        => "2",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_categorias&acao=A"
							   ),
		   			array ("descricao" => "Vincular Categorias nas Subações",
							    "id"		=> "3",
							    "link"		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/vincula_categorias&acao=A"
					  		   )
			  );
	}
	
echo montarAbasArray($res, "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_categorias&acao=A");
//cte_montaTitulo( 'Cadastrar Categorias', '&nbsp;' );
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>

<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td width="100%" align="center" gbcolor="#dcdcdc" colspan="3" bgcolor="#dcdcdc" >
			<label class="TituloTela" style="color: rgb(0, 0, 0);">Cadastro de categorias</label>
		</td>
	</tr>
	<tr>
		<td style="" align="center" bgcolor="#e9e9e9" colspan="3">
			&nbsp;
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width='10%'>
			Categoria:
		</td>
		<td width='50%'>
			<?= campo_texto( 'novaCategoria', 'S', 'S', '', 65, 65, '', '' ); ?>
			<?php 
			if( $_REQUEST['req'] == 'importar' ){
			?>
				<input type="hidden" id="catid" name="catid" value="<?=$_REQUEST['catid']; ?>" />
			<?php 
			}
			?>
		</td>
		<td></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
			Fase(s):
		</td>
		<td>
			<div style="margin: 0; padding: 0; height: 150px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
				<table id="itensFasesCategoria" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
					<colgroup>
						<col width="30px" />
						<col width="30px" />
						<col width="110px" />
						<col width="110px" />
						<col width="40px" />
					</colgroup>       
					<tr style="text-align: center">
						<td style="padding: 2px; font-weight: bold;">Excluir</td>
                        <td style="padding: 2px; font-weight: bold;">Fase</td>
                        <td style="padding: 2px; font-weight: bold;">Titulo</td>
                        <td style="padding: 2px; font-weight: bold;">Descrição</td>
                        <td style="padding: 2px; font-weight: bold;">Peso (%)</td>
					</tr>
				</table>                   
			</div>
		</td>
		<td>
			"Pelo menos uma fase deve ser adicionada."
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
		</td>
		<td>
			<span style="cursor:pointer" id="linkInserirFase" onclick="return novaFase();"><img src="/imagens/gif_inclui.gif" align="middle" style="border: none "  /> Inserir Fase</span> 
		</td>
		<td></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
		</td>
		<td>
		<?php 
		if( $_REQUEST['req'] == 'importar' ){
		?>
			<input type="button" id="btnSalvar" name="btnSalvar" value="Salvar Alterações" onclick="salvarCat()"/>
		<?php 
		}else{
		?>
			<input type="button" id="btnCriar" name="btnCriar" value="Gravar Novo" onclick="criarCat()"/>
		<?php 
		}
		?>
			<input type="hidden" id="acao" name="acao" value="0"></input>
			<input type="hidden" id="fasesQtd" name="fasesQtd" value="0"></input>
			<input type="button" id="btnVoltar" name="btnVoltar" value="Voltar" onclick="voltarInicio()"/>
		</td>
		<td></td>
	</tr>
</table>
</form>
<script>

//Redireciona para a primeira página.
function voltarInicio(){
	window.location.href = '/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_categorias&acao=A';
}

//Insere itens de fase novas
function novaFase(){
	inserirFase( '', '', '', '' );
}

//Insere o campo para nova fase.
function inserirFase( facid, facdsc, facobservacao, facpeso ){

	top.verified = 10;
        
    var input_fase;
        
    var tabela = document.getElementById( "itensFasesCategoria" );		
	var nrLinha = tabela.rows.length - 1;
 
       
    var btnExcluirFase = document.createElement('img');
    btnExcluirFase.src     = '/imagens/excluir.gif';
    btnExcluirFase.onclick = function(){
            var linha = $(this.parentNode.parentNode.getAttribute("id"));
            linha.parentNode.removeChild(linha);
        	linha.style.backgroundColor = '#f0f0f0';
    }

    var tabelaFasesCategoria = $('itensFasesCategoria');
    var novaLinha                    = tabelaFasesCategoria.insertRow(tabelaFasesCategoria.rows.length);
    novaLinha.id                     = 'linhaitensFasesCategoria' + (tabelaFasesCategoria.rows.length - 1);
    novaLinha.style.textAlign        = 'center';
    novaLinha.style.backgroundColor  = '#f0f0f0';

    var numRows = (Number(tabelaFasesCategoria.rows.length) - 1);

    //hidden, ordem da fase
    input_ordem     = document.createElement('input');
    input_ordem.setAttribute('type' , 'hidden');
    input_ordem.setAttribute('name' , 'ordem'+numRows);
    input_ordem.setAttribute('id'   , 'ordem'+numRows);
    input_ordem.setAttribute('value', numRows);

    //hidden, id da fase
    input_facid     = document.createElement('input');
    input_facid.setAttribute('type', 'hidden');
    input_facid.setAttribute('name', 'fieid'+numRows);
    input_facid.setAttribute('value', facid);
       
    //Campo Descrição
    input_fase    = document.createElement('input');
    input_fase.setAttribute('type', 'text');
    input_fase.setAttribute('class', 'normal');
    input_fase.setAttribute('name', 'fase'+numRows);
    input_fase.setAttribute('maxlength', '100');
    input_fase.setAttribute('size', '30');
    input_fase.setAttribute('value', facdsc);
    if(window.addEventListener){ // Mozilla, Netscape, Firefox
	    input_fase.setAttribute('onblur', 'MouseBlur(this)');
	    input_fase.setAttribute('onmouseout', 'MouseOut(this)');
	    input_fase.setAttribute('onfocus', 'MouseClick(this);'+'this.select()');
	    input_fase.setAttribute('onmouseover', 'MouseOver(this)');
   	}
    	else{ // IE
	    input_fase.attachEvent('onblur', function(){MouseBlur(this);});
	    input_fase.attachEvent('onmouseout', function(){MouseOut(this);});
	    input_fase.attachEvent('onfocus', function(){MouseClick(this);this.select();});
	    input_fase.attachEvent('onmouseover', function(){MouseOver(this);});
    }
        
    //Campo observação
    input_observacao    = document.createElement('input');
    input_observacao.setAttribute('type', 'text');
    input_observacao.setAttribute('class', 'normal');
    input_observacao.setAttribute('name', 'observacao'+numRows);
    input_observacao.setAttribute('maxlength', '250');
    input_observacao.setAttribute('size', '30');
    input_observacao.setAttribute('value', facobservacao );
    if(window.addEventListener){ // Mozilla, Netscape, Firefox
    	input_observacao.setAttribute('onblur', 'MouseBlur(this)');
    	input_observacao.setAttribute('onmouseout', 'MouseOut(this)');
    	input_observacao.setAttribute('onfocus', 'MouseClick(this);'+'this.select()');
    	input_observacao.setAttribute('onmouseover', 'MouseOver(this)');
   	}
    	else{ // IE
    	input_observacao.attachEvent('onblur', function(){MouseBlur(this);});
    	input_observacao.attachEvent('onmouseout', function(){MouseOut(this);});
    	input_observacao.attachEvent('onfocus', function(){MouseClick(this);this.select();});
    	input_observacao.attachEvent('onmouseover', function(){MouseOver(this);});
    }
        
    //Campo peso
    input_peso    = document.createElement('input');
    input_peso.setAttribute('type', 'text');
    input_peso.setAttribute('class', 'normal');
    input_peso.setAttribute('name', 'peso'+numRows);
    input_peso.setAttribute('maxlength', '3');
    input_peso.setAttribute('size', '5');
    input_peso.setAttribute('value', facpeso );
    if(window.addEventListener){ // Mozilla, Netscape, Firefox
    	input_peso.setAttribute('onblur', 'MouseBlur(this)');
    	input_peso.setAttribute('onmouseout', 'MouseOut(this)');
    	input_peso.setAttribute('onfocus', 'MouseClick(this);'+'this.select()');
    	input_peso.setAttribute('onmouseover', 'MouseOver(this)');
   	}
    	else{ // IE
    	input_peso.attachEvent('onblur', function(){MouseBlur(this);});
    	input_peso.attachEvent('onmouseout', function(){MouseOut(this);});
    	input_peso.attachEvent('onfocus', function(){MouseClick(this);this.select();});
    	input_peso.attachEvent('onmouseover', function(){MouseOver(this);});
    }

    input_num     = document.createElement('text');
    input_num.setAttribute('value',  numRows+'ª Fase' );
    input_num.setAttribute('name', 'numRows');
    input_num.setAttribute('style','font-family:verdana; color: blue;');
    input_num.innerHTML = numRows+'ª Fase' ;
    
    //Botao excluir campo fase        
    var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula0.appendChild(btnExcluirFase);

	//Numero da fase
    var novaCelulaNum = novaLinha.insertCell(novaLinha.cells.length);
    novaCelulaNum.appendChild(input_num); 
    novaCelulaNum.appendChild(input_ordem); 

    //Descrição da Fase        
    var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
    input_fase.setAttribute('id', 'fase'+numRows);
    novaCelula2.appendChild(input_fase);

    //Observação da Fase        
    var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
    input_observacao.setAttribute('id', 'observacao'+numRows);
    novaCelula2.appendChild(input_observacao);

    //Peso da Fase        
    var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
    input_peso.setAttribute('id', 'peso'+numRows);
    novaCelula2.appendChild(input_peso);

    return false;
}

//Valida o Formulário
function validaForm() {		

	var tabela    = document.getElementById( "itensFasesCategoria" );		
	var nrLinha   = tabela.rows.length;
	var categoria = document.getElementsByName( "novaCategoria" );

	if( categoria[0].value == '' ){
		alert("O preenchimento do campo \"Categoria\" é obrigatório!");
		categoria[0].focus();
		return false;				
	}
	
	if( categoria[0].value.length >= 65 ){
		alert("O campo Observação não pode ter mais que 65 caracteres!");
		categoria[0].focus();
		return false;				
	}
	
	if( nrLinha == '1' ){
		alert("Pelo menos uma fase deve ser preenchida!");
		return false;				
	}

	
	var total = 0;
	
	for (x=1; x<nrLinha; x++){
		var fase          = document.getElementById( "fase"+x );
		var peso          = document.getElementById( "peso"+x );
		var observacao    = document.getElementById( "observacao"+x );

		if( fase.value == ''){
			alert('O campo Descricao deve ser preenchido.');
			fase.focus();
			return false;
		};
		
		if( peso.value == ''){
			alert('O campo Peso deve ser preenchido.');
			peso.focus();
			return false;
		};
		
		if( fase.value.length >= 100 ){
			alert("O campo Descrição não pode ter mais que 100 caracteres!");
			fase.focus();
			return false;				
		}
		
		if( observacao.value.length >= 250 ){
			alert("O campo Observação não pode ter mais que 250 caracteres!");
			observacao.focus();
			return false;				
		}
		
		total = total + parseInt(peso.value);
	}

	if ( total != 100){
		alert('A soma de todos os pesos deve ser 100.');
		return false;
	}
	
	$('fasesQtd').value = nrLinha;
}	

function criarCat(){

	if ( validaForm() != false ){
	
		$('acao').value = 'gravar';
		
		return new Ajax.Request(window.location.href,{
			method: 'post',
			parameters: $('formulario').serialize('true'),
			onComplete: function(res){
				alert('Nova categoria gerada.'+res.responseText);
				window.location.href = '/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_categorias&acao=A';
			}
		});			

	}
}

function salvarCat(){

	if ( validaForm() != false ){

		$('acao').value = 'salvar';
		
		return new Ajax.Request(window.location.href,{
			method: 'post',
			parameters: $('formulario').serialize('true'),
			onComplete: function(res){
				alert('As alterações da categoria foram salvas com sucesso.'+res.responseText);
				window.location.href = '/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_categorias&acao=A';
			}
		});

	}
				
}

</script>
<?php 

if( $_REQUEST['req'] == 'importar'){
//	dbg($_REQUEST,1);
	global $db;
	
	$sql = "SELECT cordsc
			FROM cte.categoria
			WHERE catid =".$_REQUEST['catid'];
	
	$cordsc = $db->pegaUm($sql);
	
	echo "<script> document.getElementsByName( 'novaCategoria' )[0].value = '".$cordsc."';</script>";
	
	unset($sql);
	
	$sql = "SELECT facid, facdsc, facobservacao, facpeso
			FROM cte.fasescategoria
			WHERE catid = ".$_REQUEST['catid'];
	
	$dados = $db->carregar($sql);
	
	foreach( $dados as $fase){
		echo "<script>
				inserirFase( ".$fase['facid'].", '".$fase['facdsc']."', '".$fase['facobservacao']."', ".$fase['facpeso']." );
			  </script>";
	}
}

?>
