<? 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   Módulo:sistemas.inc
   Finalidade: permitir o cadastro dos sistemas
   */



if ( ($_REQUEST['act']=='alterar') && (! is_array($msgerro))  ):

	
	
   // fazer alteração de perfil na base de dados.
  	$sql = "UPDATE 
   			 demandas.sistemadetalhe 
   			SET 
		  	 sidurl 			= '".$_REQUEST['sidurl']."',
 		  	 sipid 				= '".$_REQUEST['plataforma']."',
		  	 sitid 				= '".$_REQUEST['tecnologia']."',
		  	 sidoutrastecno 	= '".$_REQUEST['outras']."',
		  	 sbdid 				= '".$_REQUEST['sbdid']."',
		  	 sidurldoc 			= '".$_REQUEST['urldoc']."'
		  	 
		  	WHERE
		  	 sidid = ".$_SESSION['sidid'];

    $saida = $db->executar($sql);
	$db->commit();
	?>
    	<script>
            alert('Operação realizada com sucesso!');
		   	location.href='demandas.php?modulo=sistema/apoio/dadosSistemasTec&acao=A';
        </script>
    <?
	exit();
endif;



include APPRAIZ."includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Dados Técnicos do Sistema';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<div align="center">
<center>
<?
//atribui valores as variaveis
$modulo=$_REQUEST['modulo'] ;//


if ($_SESSION['sidid']):
	$sql= "SELECT
			 sidid, 
			 sidurl AS sidurl, 
		     sipid AS plataforma, 
 		     sitid AS tecnologia,
 		     sidoutrastecno AS outras,
 		     sidurldoc as urldoc,
 		     sbdid as sbdid 
		   FROM
		     demandas.sistemadetalhe 
		   WHERE 
		     sidid = ".$_SESSION['sidid']."
		   ORDER BY
		     siddescricao";
	$dados = $db->carregar($sql);
	
	
	#### Transforma $dados[0] em Variáveis ####
	extract($dados[0]);
	
else:
		
	$sidid 		   = "";
	$sidurl		   = "";
	$plataforma    = "";
	$tecnologia    = "";
	$outras		   = "";
	
endif;
	$act = '';
	
?>

<form method="POST"  name="formulario">

	<input type='hidden' name="modulo" value="<?=$modulo?>">
	<input type='hidden' name="act" value="">

    <center>
    <?=cabecalhoSistemas(); ?>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td width="20%" align='right' class="SubTituloDireita">Plataforma:</td>
	    <td><?php
  	
	    	
	    	$sql = "select 	sipid as codigo, 
               				sipdsc as descricao
						from demandas.sistemaplataforma
						order by descricao";	
	    	                             	    
			$db->monta_combo('plataforma', $sql, 'S', "Selecione...", 'filtraPlataforma', '', '', '150', 'S', 'plataforma',false,null,'Plataforma');?></td>
      </tr>
		<?php $mostraUrl='none';
		 if ($plataforma == '1') $mostraUrl='';
		 ?>
      <tr id=tr_url style="display:<?=$mostraUrl?>;">
        <td align='right' class="SubTituloDireita">Url (web):</td>
	    <td id=td_url>
	     <?php
	     if (!$sidurl) $sidurl =  '';
	     ?>
	    <?=campo_texto('sidurl','S','S','',50,120,'','','','','','','',$sidurl);?>
	    </td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Linguagem:</td>
	    <td><?php
			/*
	    	$sql = "select 	sitid as codigo, 
               				sitdsc as descricao
						from demandas.sistematecnologia
						where sitid  not in (6)
						order by 2";	
			$arraysql = $db->carregar( $sql );	    	                             	    

	
			$db->monta_combo('tecnologia', $arraysql, 'S', "Selecione...", 'filtraTecnologia', '', '', '100', 'S', 'tecnologia',false,null,'Tecnologia');
			*/
	    
			?>
			
			<select style="width: 200px;" class="CampoEstilo" name="tecnologia">
				<option value=''>Selecione...</option>
				<optgroup label="Utilizadas pelo MEC">
					<?php
						$sql = sprintf( "select 	sitid as codigo, sitdsc as descricao from demandas.sistematecnologia where sitid  not in (6) order by 2" );
					?>
					<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
					<option value="<?= $tipo['codigo'] ?>" <?php if($tecnologia == $tipo['codigo']) echo "selected";?>><?= $tipo['descricao'] ?></option>
					<?php endforeach; ?>
				</optgroup>
				<optgroup label="Outras">
					<?php
						$sql = sprintf( "select 	sitid as codigo, sitdsc as descricao from demandas.sistematecnologia where sitid  in (6) order by 2" );
					?>
					<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
					<option value="<?= $tipo['codigo'] ?>" <?php if($tecnologia == $tipo['codigo']) echo "selected";?>><?= $tipo['descricao'] ?></option>
					<?php endforeach; ?>
				</optgroup>
			</select>		
			<?php echo obrigatorio();?>	
		</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Banco de Dados:</td>
	    <td><?php

	    	$sql = "select 	sbdid as codigo, 
               				sbddsc as descricao
						from demandas.sistemasgbd
						where sbdstatus = 'A'
						order by 1";	
	    	                             	    
			$db->monta_combo('sbdid', $sql, 'S', "Selecione...", '', '', '', '150', 'S', 'sbdid',false,null,'Banco de Dados');?></td>
      </tr>  
           
      <tr >
        <td align='right' class="SubTituloDireita">Detalhar Outras Tecnologias:</td>
	    <td >
		    <?=campo_textarea('outras','N','S','',75,3,'');?>
	    </td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Url da documentação:</td>
	    <td>
	    	<?=campo_texto('urldoc','S','S','',50,120,'','','','','','','',$urldoc);?>
	    </td>
      </tr>      

	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td>
   			<input type="button" name="btinserir" value="Salvar" onclick="validar_cadastro('I')" class="botao">
   			&nbsp;
   			<input type="button" name="btcancela" value="Limpar" onclick="inicio();" class="botao">

   		</td>
	</tr>
      
    </table>

  
</form>



<script>

var d=document;



function inicio() {
 	location.href='demandas.php?modulo=sistema/apoio/dadosSistemasTec&acao=A';
}
  
function validar_cadastro() {


	if (document.formulario.plataforma.value == "")
	{
		alert("Favor informar plataforma do Sistema.");
		document.formulario.plataforma.focus();
		return;		
	}
	
	if (document.formulario.plataforma.value == 1){	 
		if (document.formulario.sidurl.value == "http:\\\\" || document.formulario.sidurl.value == "")  {
			alert("Plataforma WEB é obrigatório informar URL.");
			document.formulario.sidurl.focus();
			return;
		}
	}

	if (document.formulario.tecnologia.value == "")
	{
		alert("Favor informar Linguagem do Sistema.");
		document.formulario.tecnologia.focus();
		return;		
	}

	if (document.formulario.sbdid.value == "")
	{
		alert("Favor informar o Banco de Dados do Sistema.");
		document.formulario.sbdid.focus();
		return;		
	}
/*
	if (document.formulario.outras.value == "")  {
		alert("Favor informar Detalhar Outras Tecnologias.");
		document.formulario.outras.focus();
		return;
	}
*/
	if (document.formulario.urldoc.value == "http:\\\\" || document.formulario.urldoc.value == "")  {
		alert("Favor informar a URL da documentação do Sistema.");
		document.formulario.urldoc.focus();
		return;
	}	
	
	
	document.formulario.act.value = 'alterar';
    document.formulario.submit();

}

/*
* Faz requisição via ajax
* Filtra o tipo de damanda, atravéz do parametro passado 'ordid'
*/
function filtraPlataforma(sipid) {
	
	trUrl  = d.getElementById('tr_url');
	tdUrl  = d.getElementById('td_url');

	trUrl.style.display = 'none';
	tdUrl.style.display = 'none';

	if(sipid == 1){ //Web
		trUrl.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdUrl.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
}

function filtraTecnologia(sitid) {
	
	trOutras  = d.getElementById('tr_outras');
	tdOutras  = d.getElementById('td_outras');

	trOutras.style.display = 'none';
	tdOutras.style.display = 'none';

	if(sitid == 6){ //Outras
		trOutras.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdOutras.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
}


</script>
