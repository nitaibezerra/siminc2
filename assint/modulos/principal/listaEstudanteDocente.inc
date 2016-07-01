<?php

$comp = new AssInt();

switch($_REQUEST['evento'])
{
	case 'excluir':
		if ($_GET['benid']){
			$dados 		 = array("benstatus" => 'I');
			$paramUpdate = array('benid' => $_GET['benid']);
			
			if ($comp->manterBeneficiario($dados, $paramUpdate )){	
				$db->sucesso($_REQUEST['modulo'], $paramUrl);	
			}else{
				redir('', 'Operação não Realizada!\nHouve Alguma Incompatibilidade nos Dados Enviados.');
			}	
		}
	break;
}

if( $_REQUEST['benid'] )
{
	$dados = $db->carregar('SELECT benindbenef,prgid FROM assint.beneficiario WHERE benid = '.$_REQUEST['benid']);
	
	if( $dados )
	{
		if( $dados[0]['benindbenef'] == 'E' )
		{
			if( is_null($dados[0]['prgid']) )
			{
				unset($_SESSION['assint']['prgid']);
				redirecionar('principal/cadBenEstudante', 'C', array('benid' => $_REQUEST['benid']));
			}
			else
			{
				$_SESSION['assint']['prgid'] = $dados[0]['prgid'];
				redirecionar('principal/cadBenEstudante', 'A', array('benid' => $_REQUEST['benid']));
			}
		}
		else
		{
			if( is_null($dados[0]['prgid']) )
			{
				unset($_SESSION['assint']['prgid']);
				redirecionar('principal/cadBenDocente', 'C', array('benid' => $_REQUEST['benid']));
			}
			else
			{
				$_SESSION['assint']['prgid'] = $dados[0]['prgid'];
				redirecionar('principal/cadBenDocente', 'A', array('benid' => $_REQUEST['benid']));
			}
		}
			
		die;
	}
}

if( $_REQUEST['req'] == 'tipoEntAlteraComboOrigem' )
{
	$join = "";
    if( $_REQUEST['tipo'] )
    {
    	if( $_REQUEST['tipo'] == 'I' )
    	{
    		$join = " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = 'I' AND (eas.entid = e.entid OR eas.entid = e2.entid) ";
    	}
    	if( $_REQUEST['tipo'] == 'U' )
    	{
    		$join = " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = 'U' AND (eas.entid = e.entid OR eas.entid = e2.entid) ";
    	}
    }
    
	$sql="select distinct
	case when ben.prgid is null then e.entid else e2.entid end as codigo,
	case when ben.prgid is null then e.entnome else e2.entnome end as descricao
	--ent.entid as codigo,
	--ent.entnome as descricao 
	from assint.beneficiario ben
	LEFT JOIN
		entidade.entidade e ON e.entid = ben.entidorigem
	LEFT JOIN
		assint.programa pr ON pr.prgid = ben.prgid
	LEFT JOIN
		entidade.entidade e2 ON e2.entid = pr.entid
	 {$join} 
	WHERE
		 	e.entid is not null OR e2.entid is not null
	order by descricao asc";
	
	echo $db->monta_combo("entidorigem",$sql,'S',"Selecione uma Instituição de Origem...",'','','','350');
	die();
}

if( $_REQUEST['req'] == 'tipoEntAlteraComboPrograma' )
{
	$join = "";
    if( $_REQUEST['tipo'] )
    {
    	if( $_REQUEST['tipo'] == 'I' )
    	{
    		$join = " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = 'I' AND eas.entid = prg.entid ";
    	}
    	if( $_REQUEST['tipo'] == 'U' )
    	{
    		$join = " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = 'U' AND eas.entid = prg.entid ";
    	}
    }
    
	$sql="select distinct prg.prgid as codigo, prg.prgnome as descricao 
	from assint.beneficiario ben
	inner join assint.programa prg on prg.prgid = ben.prgid
	".$join."
	where prg.prgstatus = 'A'
	order by prg.prgnome asc";
	
	echo $db->monta_combo("prgid_prog_projeto",$sql,'S',"Selecione um Programa/Projeto...",'','','','350','N','prgid_prog_projeto');
	die();
}

// Monta Cabeçalho
include APPRAIZ."includes/cabecalho.inc";

// Monta título da página
echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Lista de Estudantes/Docentes','');

extract($_POST);

?>

<script src="/assint/js/assint.js"></script>
<script src="../includes/prototype.js"></script>

<form method="POST" name="pesqEstudanteDocente" id="pesqEstudanteDocente">
<input type="hidden" name="tipoPesquisa" id="tipoPesquisa" value="lista" />

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
    	<td align='right' colspan="2" class="SubTituloCentro">Filtro de Pesquisa</td>
    </tr>
    <!--<tr>
       <td align='right' width="30%" class="SubTituloDireita">CPF:</td>
       <td>
		<? /*echo campo_texto('entnumcpfcnpj','N','S','',20,14,'###.###.###-##','');*/ ?>
       </td>
    </tr>-->
    <tr>
		<td class="SubTituloDireita" width="250px">Tipo da Entidade:
		</td>
		<td>	
			<input type="radio" value="T" name="enttipo" <?=((!$_REQUEST['enttipo'] || $_REQUEST['enttipo'] == 'T') ? 'checked="checked"' : '')?> onclick="tipoEntAlteraCombo(this);" />Todos
	       	&nbsp;
	       	<input type="radio" value="I" name="enttipo" <?=(($_REQUEST['enttipo'] == 'I') ? 'checked="checked"' : '')?> onclick="tipoEntAlteraCombo(this);" />Instituição
	       	&nbsp;
	       	<input type="radio" value="U" name="enttipo" <?=(($_REQUEST['enttipo'] == 'U') ? 'checked="checked"' : '')?> onclick="tipoEntAlteraCombo(this);" />Universidade
		</td>
	</tr> 
    <tr>
		<td class="SubTituloDireita" width="250px">Nome:
		</td>
		<td>	
		<?=campo_texto('bennome','N','S','',50,200,'','');?>
		</td>
	</tr>   
    <tr>
       <td align='right' class="SubTituloDireita">Tipo:</td>
       <td>
       	<input type="radio" value="T" name="bentipo" <?=((!$_REQUEST['bentipo'] || $_REQUEST['bentipo'] == 'T') ? 'checked="checked"' : '')?> />Todos
       	&nbsp;
       	<input type="radio" value="B" name="bentipo" <?=(($_REQUEST['bentipo'] == 'B') ? 'checked="checked"' : '')?> />Brasileiro no Exterior
       	&nbsp;
       	<input type="radio" value="E" name="bentipo" <?=(($_REQUEST['bentipo'] == 'E') ? 'checked="checked"' : '')?> />Estrangeiro na Instituição
    </td>
    </tr>
    <tr>
    	<td align='right' class="SubTituloDireita">País de Origem/Destino:</td>
    	<td>
    	<?
		$sql="SELECT distinct
				paiid as codigo, 
				paidescricao as descricao 
			FROM
				territorios.pais 
			--WHERE
				--paiid <> 1
			ORDER BY
				paidescricao ASC";
		
		$db->monta_combo("paiid",$sql,'S',"Selecione um País...",'','');
	 	?>
    	</td>
    </tr>
    <tr>
    	<td align='right' class="SubTituloDireita">Instituição:</td>
    	<td>
    	<?=campo_texto('benuniversidade','N','S','',50,100,'','');?>
    	</td>
    </tr>
    <tr>
    	<td align='right' class="SubTituloDireita">Instituição de Origem:</td>
    	<td id="td_inst_origem">
    	<?
    	$join = "";
    	if( $_REQUEST['enttipo'] )
    	{
    		if( $_REQUEST['enttipo'] == 'I' )
    		{
    			$join = " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = 'I' AND (eas.entid = e.entid OR eas.entid = e2.entid) ";
    		}
    		if( $_REQUEST['enttipo'] == 'U' )
    		{
    			$join = " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = 'U' AND (eas.entid = e.entid OR eas.entid = e2.entid) ";
    		}
    	}
    	
		$sql="select distinct
		case when ben.prgid is null then e.entid else e2.entid end as codigo,
		case when ben.prgid is null then e.entnome else e2.entnome end as descricao
		--ent.entid as codigo,
		--ent.entnome as descricao 
		from assint.beneficiario ben
		LEFT JOIN
			entidade.entidade e ON e.entid = ben.entidorigem
		LEFT JOIN
			assint.programa pr ON pr.prgid = ben.prgid
		LEFT JOIN
			entidade.entidade e2 ON e2.entid = pr.entid
		 {$join} 
		WHERE
		 	e.entid is not null OR e2.entid is not null
		order by descricao asc";
		
		$db->monta_combo("entidorigem",$sql,'S',"Selecione uma Instituição de Origem...",'','','','350');
	 	?>
    	</td>
    </tr>
    <tr>
       <td align='right' class="SubTituloDireita">Categoria:</td>
       <td>
	       <input type="radio" value="T" name="benindbeneficiario" <?=((!$_REQUEST['benindbeneficiario'] || $_REQUEST['benindbeneficiario'] == 'T') ? 'checked="checked"' : '')?> />Todos
	       &nbsp;
	       <input type="radio" value="E" name="benindbeneficiario" <?=(($_REQUEST['benindbeneficiario'] == 'E') ? 'checked="checked"' : '')?> />Estudante
	       &nbsp;
	       <input type="radio" value="D" name="benindbeneficiario" <?=(($_REQUEST['benindbeneficiario'] == 'D') ? 'checked="checked"' : '')?> />Docente
    	</td>
    </tr>
    <tr>
       <td align='right' class="SubTituloDireita">Ligados a Programa/Projeto:</td>
       <td>
	       <input type="radio" value="T" name="ligados_programa" <?=((!$_REQUEST['ligados_programa'] || $_REQUEST['ligados_programa'] == 'T') ? 'checked="checked"' : '')?> onclick="mostraComboPrograma('T');" />Todos
	       &nbsp;
	       <input type="radio" value="S" name="ligados_programa" <?=(($_REQUEST['ligados_programa'] == 'S') ? 'checked="checked"' : '')?> onclick="mostraComboPrograma('S');" />Sim
	       &nbsp;
	       <input type="radio" value="N" name="ligados_programa" <?=(($_REQUEST['ligados_programa'] == 'N') ? 'checked="checked"' : '')?> onclick="mostraComboPrograma('N');" />Não
    	</td>
    </tr>
    <tr id="tr_programa" style="display:<?=(($_REQUEST['ligados_programa'] && $_REQUEST['ligados_programa'] == 'S') ? 'table-row' : 'none')?>">
       <td align='right' class="SubTituloDireita">Programa/Projeto:</td>
       <td id="td_programa_projeto">
       <?
        $join = "";
    	if( $_REQUEST['enttipo'] )
    	{
    		if( $_REQUEST['enttipo'] == 'I' )
    		{
    			$join = " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = 'I' AND eas.entid = prg.entid ";
    		}
    		if( $_REQUEST['enttipo'] == 'U' )
    		{
    			$join = " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = 'U' AND eas.entid = prg.entid ";
    		}
    	}
    	
		$sql="select distinct prg.prgid as codigo, prg.prgnome as descricao 
		from assint.beneficiario ben
		inner join assint.programa prg on prg.prgid = ben.prgid
		".$join."
		where prg.prgstatus = 'A'
		order by prg.prgnome asc";
		
		$db->monta_combo("prgid_prog_projeto",$sql,'S',"Selecione um Programa/Projeto...",'','','','350','N','prgid_prog_projeto');
	 	?>
       </td>
    </tr>
 	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	    	<input type="button" name="btpesquisar" value="Pesquisar" onclick="pesquisa();" class="botao" />
	    	&nbsp;&nbsp;&nbsp;&nbsp;
	    	<input type="button" name="btexcel" value="Visualizar XLS" onclick="visualizarXls();" class="botao" />
	   </td>
	</tr>           
</table>
</form>
<script>

function pesquisa()
{
	$('tipoPesquisa').value = 'lista';
	$('pesqEstudanteDocente').submit();
}

function visualizarXls()
{
	$('tipoPesquisa').value = 'xls';
	$('pesqEstudanteDocente').submit();
}

function mostraComboPrograma(tipo)
{
	if( tipo == 'S' )
	{
		$('tr_programa').style.display = 'table-row';
	}
	else
	{
		$('prgid_prog_projeto').selectedIndex = 0;
		$('tr_programa').style.display = 'none';
	}
}

function tipoEntAlteraCombo(obj)
{
	//return
	new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=tipoEntAlteraComboOrigem&tipo=' + obj.value,
		onComplete: function(res){
			$('td_inst_origem').innerHTML = res.responseText;
		}
	});	

	new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=tipoEntAlteraComboPrograma&tipo=' + obj.value,
		onComplete: function(res){
			$('td_programa_projeto').innerHTML = res.responseText;
		}
	});	
}

</script>
<?

if( !$_POST['tipoPesquisa'] || $_POST['tipoPesquisa'] == 'lista' )
{
	$_POST['listaEstudanteDocente'] = true;
	$comp->listaBeneficiario($_POST, array('modulo' => 'principal/listaEstudanteDocente'));
}
else
{
	ob_clean();
	$_POST['listaEstudanteDocente'] = true;
	$sql = $comp->listaBeneficiario($_POST);
	
	$cab = array("Nome", "Tipo", "País de Origem/Destino", "Instituição", "Instituição de Origem");
	
	$db->sql_to_excel($sql,"SIMEC_Relat",$cab);
}

?>