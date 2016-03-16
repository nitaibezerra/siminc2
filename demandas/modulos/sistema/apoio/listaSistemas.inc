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

$_SESSION['sidid'] = "";

if ( ($_REQUEST['act']=='excluir') && (! is_array($msgerro)) && !$_POST['pesquisa']):
   // fazer exclusão de perfil na base de dados.
   	
	$sql1 = "SELECT sc.sidid 
			 FROM demandas.celula AS c
			 INNER JOIN demandas.sistemacelula AS sc ON sc.celid = c.celid 
			 WHERE c.celstatus = 'A' 
			 AND sc.sidid = ". $_REQUEST['sidid'] . "
			 LIMIT 1";
	 
	$verifica = $db->pegaUm($sql1);
	
	if($verifica)
	{
		?>
		<script >
			alert('Operação não pode ser realizada. \nDevido a dados vinculados com alguma célula de projeto');
			location.href = 'demandas.php?modulo=sistema/apoio/listaSistemas&acao=A';
		</script>
		<? 
	}else{
	     /*
	    $sql  = "DELETE FROM demandas.sistemadetalhe WHERE sidid = ".$_REQUEST['sidid'];
	    */
		$sql  = "UPDATE demandas.sistemadetalhe	SET sidstatus = 'I' WHERE sidid = ".$_REQUEST['sidid'];
	    $saida = $db->executar( $sql );
	    $db->commit();
		?>
	    	<script>
	            alert('Operação realizada com sucesso!');
			   	location.href='demandas.php?modulo=sistema/apoio/listaSistemas&acao=A';
	        </script>
	    <?
	}
	exit();
endif;

include APPRAIZ."includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Lista de Sistemas';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<div align="center">
<center>
<?
//atribui valores as variaveis
$modulo=$_REQUEST['modulo'] ;//

	
?>

<form method="POST"  name="formulario">

	<input type='hidden' name="modulo" value="<?=$modulo?>">
	<input type='hidden' name="act" value=<?=$act?>>
	<input type="hidden" name="pesquisa" value="">
	
	
  

    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     
      <tr>
        <td align='right' class="SubTituloDireita">Código:</td>
	    <td colspan='2'><?=campo_texto('sidid','N','S','',5,5,'##########','');?></td>
      </tr>
     
      <tr>
        <td align='right' class="SubTituloDireita">Sistema:</td>
	     <td><?=campo_texto('dsc','N','S','',50,100,'','');?></td>
	     <td rowspan=3>
		   <div style="width:300px;top:250px;">
				<!-- <fieldset style="background-color:#ffffff;"> -->
				<fieldset>
				<legend> <b>Mensagens</a></b></legend>
					<table width="100%" >
						<tr>
							<td valign="bottom" height="50%" style="padding-right:15px;">
								<?php 
								$sql = "select count(sidid) as total from demandas.sistemadetalhe where siddestaquebanner = 'A'";
								$totalBanner = $db->pegaUm($sql);
								echo "TOTAL DESTAQUE BANNER: <b>".$totalBanner."</b> (LIMITE: <B>5</B>)";
								?>
							</td>
						</tr>
						<tr>
							<td valign="bottom" height="50%" style="padding-right:15px;">
								<?php 
								$sql = "select count(sidid) as total from demandas.sistemadetalhe where siddestaqueaba = 'A'";
								$totalAba = $db->pegaUm($sql);
								echo "TOTAL DESTAQUE ABA: <b>".$totalAba."</b> (LIMITE: <B>8</B>)";
								?>
							</td>
						</tr>
					</table>
				</fieldset>	
			</div>	
	     
	     </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Sigla:</td>
	    <td colspan='2'><?=campo_texto('abrev','N','S','',15,15,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Plataforma:</td>
	    <td colspan='2'><?php
  	
	    	
	    	$sql = "select 	sipid as codigo, 
               				sipdsc as descricao
						from demandas.sistemaplataforma
						order by descricao";	
	    	                             	    
			$db->monta_combo('plataforma', $sql, 'S', "Selecione...", 'filtraPlataforma', '', '', '150', 'N', 'plataforma',false,null,'Plataforma');?></td>
      </tr>
      
		

      <tr>
        <td align='right' class="SubTituloDireita">Linguagem:</td>
	    <td colspan='2'><?php
			/*
	    	$sql = "select 	sitid as codigo, 
               				sitdsc as descricao
						from demandas.sistematecnologia
						where sitid  not in (6)
						order by 2";	
			$arraysql = $db->carregar( $sql );	    	                             	    

			$db->monta_combo('tecnologia', $arraysql, 'S', "Selecione...", '', '', '', '100', 'N', 'tecnologia',false,null,'Tecnologia');
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
						
		</td>
      </tr>


      <tr>
        <td align='right' class="SubTituloDireita">Banco de Dados:</td>
	    <td colspan='2'><?php

	    	$sql = "select 	sbdid as codigo, 
               				sbddsc as descricao
						from demandas.sistemasgbd
						where sbdstatus = 'A'
						order by 1";	
	    	                             	    
			$db->monta_combo('sbdid', $sql, 'S', "Selecione...", '', '', '', '150', 'N', 'sbdid',false,null,'Banco de Dados');?></td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Situação:</td>
	    <td colspan='2'><?php

	    	$sql = "select 	ssiid as codigo, 
               				ssidsc as descricao
						from demandas.sistemasituacao
						order by 1";	
	    	                             	    
			$db->monta_combo('situacao', $sql, 'S', "Selecione...", '', '', '', '150', 'N', 'situacao',false,null,'Situacao');?></td>
      </tr>
    	
      <tr>
        <td align='right' class="SubTituloDireita">Órgão Responsável:</td>
	    <td colspan='2'><?php

	    	$sql = "select 	unaid as codigo, 	
                			unasigla || ' - ' || unadescricao as descricao
						from demandas.unidadeatendimento
						order by unasigla";
	    	                             	    
			$db->monta_combo('orgao', $sql, 'S', "Selecione...", '', '', '', '500', 'N', 'orgao',false,null,'orgao');?></td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Eixo PDE:</td>
	    <td colspan='2'><?php

	    	$sql = "select 	exoid as codigo, 	
                			exodsc as descricao
						from painel.eixo
						order by 2";
	    	                             	    
			$db->monta_combo('exoid', $sql, 'S', "Selecione...", '', '', '', '500', 'N', 'exoid',false,null,'Eixo PDE');?></td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Destaque Banner:</td>
	       <td colspan='2'>
            <input type="radio" name="dbanner" value="A" <?=($dbanner=='A'?"CHECKED":"")?>> SIM
            <input type="radio" name="dbanner" value="I" <?=($dbanner=='I'?"CHECKED":"")?>> NÃO
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Destaque Aba:</td>
	       <td colspan='2'>
            <input type="radio" name="daba" value="A" <?=($daba=='A'?"CHECKED":"")?>> SIM
            <input type="radio" name="daba" value="I" <?=($daba=='I'?"CHECKED":"")?>> NÃO
        </td>
      </tr>
      

	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td colspan='2'>
	    	<input type="button" name="Pesquisar" value="Pesquisar" onclick="pesq();">
	    	&nbsp;
	    	<input type="button" name="btinserir" value="Novo" onclick="novo()" class="botao">
   		</td>
	</tr>

      
    </table>

  
</form>


<? 


//filtro
	if($_REQUEST['pesquisa'] ){
		
		if($_POST['sidid']) {$and .= " AND sd.sidid = ".$_POST['sidid'];}
	
		$_POST['dsc'] = str_replace("'", "", $_POST['dsc']);
		if($_POST['dsc']) {$and .= " AND upper(sd.siddescricao) ilike '%".strtoupper($_POST['dsc'])."%' ";}
		

		$_POST['abrev'] = str_replace("'", "", $_POST['abrev']);
		if($_POST['abrev']) {$and .= " AND upper(sd.sidabrev) ilike '%".strtoupper($_POST['abrev'])."%' ";}
		
		
		if($_POST['plataforma']) {$and .= " AND sd.sipid = ".$_POST['plataforma'];}
		if($_POST['tecnologia']) {$and .= " AND sd.sitid = ".$_POST['tecnologia'];}
		if($_POST['sbdid']) {$and .= " AND sd.sbdid = ".$_POST['sbdid'];}
		if($_POST['situacao']) {$and .= " AND sd.ssiid = ".$_POST['situacao'];}
		if($_POST['orgao']) {$and .= " AND sd.unaid = ".$_POST['orgao'];}
		if($_POST['exoid']) {$and .= " AND sd.exoid = ".$_POST['exoid'];}
		if($_POST['dbanner']) {$and .= " AND (sd.siddestaquebanner = '".$_POST['dbanner']."')";}
		if($_POST['daba']) {$and .= " AND (sd.siddestaqueaba = '".$_POST['daba']."')";}
		
	}
	
	
//exibe listagem dos sistemas cadastrados
$sql= "select 
		'<a href=\"demandas.php?modulo=sistema/apoio/dadosSistemas&acao=A&sidid=' || sd.sidid || '\"><img border=0 src=\"../imagens/alterar.gif\" /></a>&nbsp;<a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/listaSistemas&acao=A&sidid=' || sd.sidid || '&act=excluir\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a>' AS op,
		sd.sidid as codigo2, 
		sd.siddescricao as descricao2, 
		sd.sidabrev as abreviacao2, 
		ua.unasigla || ' - ' || ua.unadescricao as orgao,
		'' as eixopde,
		st.sitdsc as linguagem,
		bd.sbddsc as bancodedados,
		CASE WHEN siddestaquebanner = '' OR siddestaquebanner is null OR siddestaquebanner = 'I' THEN
				'NÃO'
			 ELSE
			 	'SIM'
		END AS banner,
		CASE WHEN siddestaqueaba = '' OR siddestaqueaba is null OR siddestaqueaba = 'I' THEN
				'NÃO'
			 ELSE
			 	'SIM'
		END as aba,
		sit.ssidsc as situacao
		
 		from demandas.sistemadetalhe sd
 		inner join demandas.unidadeatendimento ua on ua.unaid = sd.unaid
 		left join demandas.sistematecnologia st on st.sitid = sd.sitid
 		left join demandas.sistemasgbd bd on bd.sbdid = sd.sbdid
 		left join demandas.sistemasituacao sit on sit.ssiid = sd.ssiid
 		WHERE sidstatus = 'A'
 		$and
 		order by siddescricao ";
//dbg($sql,1);
$cabecalho = array("Ação","Cód","Sistema","Sigla","Órgão","Eixo PDE","Linguagem","Bando de Dados","Destaque Banner","Destaque Aba","Situação");
//$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
$dados = $db->carregar($sql);

if($dados){
	
	for($i = 0; $i<=count($dados)-1; $i++ ){
		$sql = "select 	e.exodsc as descricao
						from demandas.sistemaeixos se
						inner join painel.eixo e ON e.exoid = se.exoid
						where sidid = ".$dados[$i]['codigo2']."
						order by 1";
		$eixos = $db->carregar($sql);
		if($eixos){
			$strEixos = ''; 
			foreach($eixos as $ex){
				$strEixos .= '<br> - '.$ex['descricao'];
			}
			$dados[$i]['eixopde'] = $strEixos;
		}else{
			$dados[$i]['eixopde'] = '';
		}
	} 

	$db->monta_lista_array($dados,$cabecalho,50,10,'','');
}
else{
	echo "<b><font color=red>Não existe registros</font></b>";
}

?>


<script>

var d=document;


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este item?")
	if (questao){
		window.location = url;
	}
}


function novo() {
 	location.href='demandas.php?modulo=sistema/apoio/cadSistemas&acao=A';
}
  

function pesq(){
	/*
	if(document.formulario.dsc.value == '' && document.formulario.abrev.value == ''){
		alert ('Informe o Sistema ou a Sigla.');
		document.formulario.dsc.focus();
		return;
	}
	*/
	document.formulario.pesquisa.value="1";
    document.formulario.submit();
}


</script>
