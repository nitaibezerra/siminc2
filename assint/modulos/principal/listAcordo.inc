<?php

if($_REQUEST['req'] == 'listarIntituicao' ){
	
	if( $_REQUEST['tipo'] == 'T' ):
		$tipo		= "'I','U'";
		$textoTipo 	= 'Selecione uma Instituição ou Universidade...';
	elseif( $_REQUEST['tipo'] == 'I' ):
		$tipo		= "'I'";
		$textoTipo 	= 'Selecione uma Instituição...';
	elseif( $_REQUEST['tipo'] == 'U' ):
		$tipo		= "'U'";
		$textoTipo 	= 'Selecione uma Universidade...';
	endif;
	
	$sql="SELECT 
			e.entid as codigo, 
			e.entnome as descricao 
		  FROM 
		  	entidade.entidade e
		  INNER JOIN 
		  	assint.entidadeassessoriainternacional ea ON ea.entid = e.entid AND 
		  												 ea.entstatus = 'A'
		  WHERE
		  	ea.enttipo in ({$tipo})
		  ORDER BY 
		  	e.entnome";
	
	echo $db->monta_combo("entid",$sql,'S',$textoTipo,'','');
	die();
}

$comp = new AssInt();

switch ($_REQUEST['evento']){
	case 'excluir':
		if ($_GET['acoid']){
			$dados 		 = array("acostatus" => 'I');
			$paramUpdate = array('acoid' 	 => $_GET['acoid']);
			
			if ($comp->manterAcordo($dados, $paramUpdate )){	
				$db->sucesso("principal/listAcordo", $paramUrl);	
			}else{
				redir('', 'Operação não Realizada!\nHouve Alguma Incompatibilidade nos Dados Enviados.');
			}	
		}
	break;
}

// Monta Cabeçalho
include APPRAIZ."includes/cabecalho.inc";

// Monta título da página
echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Lista de Acordos','');

extract($_POST);

$sql = "SELECT DISTINCT
			enttipo
		FROM 
			assint.usuarioresponsabilidade ur
		INNER JOIN
			assint.entidadeassessoriainternacional ei ON ei.entid = ur.entid 
		WHERE
			ur.usucpf = '{$_SESSION['usucpf']}'";

$tipos = $db->carregar($sql);
?>
<script src="/assint/js/assint.js"></script>
<script src="../includes/prototype.js"></script>
<form method="POST" name="pesqPrograma" id="pesqPrograma">
<input type="hidden" name="tipoPesquisa" id="tipoPesquisa" value="lista" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
    	<td align='right' colspan="2" class="SubTituloCentro">Filtro de Pesquisa</td>
    </tr>
    <tr>
       <td align='right' width="30%" class="SubTituloDireita">Título do Convênio:</td>
       <td>
		<?=campo_texto('acoconvenio','N','S','',80,200,'','');?>
       </td>
    </tr>
	<tr>
		<td class="SubTituloDireita" width="250px"> Tipo da Entidade:
		</td>
		<td>	
			<?php 
				if( empty($_REQUEST['enttipo']) || $_REQUEST['enttipo'] == 'T' )
				{
					$tipo      = "'I','U'";
					$enttipoT  = 'checked="checked"';
					$textoTipo = 'Selecione uma Instituição ou Universidade...';
				}
				elseif( $_REQUEST['enttipo'] == 'I' )
				{
					$tipo      = "'".$_REQUEST['enttipo']."'";
					$enttipoI  = 'checked="checked"';
					$textoTipo = 'Selecione uma Instituição...';
				}
				elseif( $_REQUEST['enttipo'] == 'U' )
				{
					$tipo      = "'".$_REQUEST['enttipo']."'";
					$enttipoU  = 'checked="checked"';
					$textoTipo = 'Selecione uma Universidade...';
				}
			?>
			<input type="radio" name="enttipo" id="enttipoT" value="T" <?=$enttipoT; ?> onclick="carregaInstituicao(this.value);"/> Todos
			<input type="radio" name="enttipo" id="enttipoI" value="I" <?=$enttipoI; ?> onclick="carregaInstituicao(this.value);"/> Instituição
			<input type="radio" name="enttipo" id="enttipoU" value="U" <?=$enttipoU; ?> onclick="carregaInstituicao(this.value);"/> Universidade
			<!--<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">-->
		</td>
	</tr> 
    <tr>
        <td align='right' class="SubTituloDireita">Instituição:</td>
        <td>
        <div id="divEntidade">
	        <?
			$sql="SELECT 
					e.entid as codigo, 
					e.entnome as descricao 
				  FROM 
				  	entidade.entidade e
				  INNER JOIN 
				  	assint.entidadeassessoriainternacional ea ON     ea.entid = e.entid AND 
				  												 ea.entstatus = 'A'
				  WHERE
				  	ea.enttipo in ({$tipo})
				  ORDER BY 
				  	e.entnome";
			$db->monta_combo("entid",$sql,'S',$textoTipo,'','');
		 	?>
       	</div>
        </td>
    </tr>    
    <tr>
    	<td align='right' class="SubTituloDireita">País:</td>
    	<td>
    	<?
		$sql="select paidescricao as codigo, paidescricao as descricao 
		from territorios.pais 
		where paiid <> 1
		order by paidescricao asc";
		
		$db->monta_combo("paiid",$sql,'S',"Selecione um País...",'','');
	 	?>
    	</td>
    </tr>
    <tr>
    	<td align='right' class="SubTituloDireita">Organismo Internacional:</td>
    	<td>
    	<?
		$sql="select oridescricao as codigo, oridescricao as descricao 
		from entidade.organismointernacional 
		order by oridescricao asc";
		
		$db->monta_combo("oriid",$sql,'S',"Selecione um Organismo Internacional...",'','');
	 	?>
    	</td>
    </tr>
    <tr>
    	<td align='right' class="SubTituloDireita">Fonte de Financiamento:</td>
    	<td>
    	<?
		$sql="select fofdesc as codigo, fofdesc as descricao 
		from assint.fontefinanciamento 
		where fofstatus = 'A'
		order by fofdesc asc";
		
		$db->monta_combo("fofid",$sql,'S',"Selecione uma Fonte de Financiamento...",'','');
	 	?>
    	</td>
    </tr>
 	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	    	<input type="button" name="btalterar" value="Pesquisar" onclick="pesquisa();" class="botao">
	    	&nbsp;&nbsp;&nbsp;&nbsp;
	    	<input type="button" name="btcancelar" value="Novo Acordo" onclick="redireciona('?modulo=principal/cadAcordo&acao=E');" class="botao">
	    	&nbsp;&nbsp;&nbsp;&nbsp;
	    	<input type="button" name="btexcel" value="Visualizar XLS" onclick="visualizarXls();" class="botao">
	   </td>
	</tr>           
</table>
</form>
<script>

function pesquisa()
{
	$('tipoPesquisa').value = 'lista';
	$('pesqPrograma').submit();
}

function visualizarXls()
{
	$('tipoPesquisa').value = 'xls';
	$('pesqPrograma').submit();
}

function carregaInstituicao( tipo ){
	
	var div = $('divEntidade');
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=listarIntituicao&tipo=' + tipo,
		onComplete: function(res){
			div.innerHTML = res.responseText;
		}
	});	
}

</script>
<?

if( !$_POST['tipoPesquisa'] || $_POST['tipoPesquisa'] == 'lista' )
{
	$comp->listaAcordo($_POST);
}
else
{
	ob_clean();
	$sql = $comp->listaAcordo($_POST);
	
	$cab = array( "Título do Convênio",
				  "Instituição/Universidade",
				  "País(es)",
				  "Organismo(s) Internacional(is)",			  
				  "Fonte(s) de Financiamento(s)"
				);
	
	$db->sql_to_excel($sql,"SIMEC_Relat",$cab);
}

?>