<? 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Módulo:sistemas.inc
   Finalidade: permitir o cadastro dos sistemas
   */

// cria sessao sistemas
$_SESSION['sidid'] = $_REQUEST['sidid'] ? $_REQUEST['sidid'] : $_SESSION['sidid'];



########### AÇÕES Cadastro ###########
if ( ($_REQUEST['act']=='alterar') && (! is_array($msgerro)) ):

	
	if(strtoupper($_REQUEST['abrev']) != strtoupper($_REQUEST['abrev_old']))
	{
	    $sql = sprintf("SELECT
					     COUNT(sidid) as total 
					    FROM
					     demandas.sistemadetalhe 
					    WHERE
					    sidstatus = 'A'
					    AND UPPER(sidabrev) = '%s'",
				strtoupper($_REQUEST['abrev']));
	
				
		$total = $db->pegaUm($sql);
				
		if ($total > 0 ){
			die('<script>
		         	alert(\'Já existe a Sigla "'.strtoupper($_REQUEST['abrev']).'" no Banco de Dados.\nOperação não realizada!\');
		         	history.back();
		      	 </script>');
		}
	}
	
	if(!$_REQUEST['qtdeusuarios']) $_REQUEST['qtdeusuarios'] = "0";
	
   // fazer alteração de perfil na base de dados.
  	$sql = "UPDATE 
   			 demandas.sistemadetalhe 
   			SET 
		   	 siddescricao 		= '".$_REQUEST['dsc']."',
		  	 sidabrev 			= '".strtoupper($_REQUEST['abrev'])."',
		  	 sidfinalidade 		= '".$_REQUEST['finalidade']."',
		  	 sidrelacionado 	= '".$_REQUEST['relacionado']."',
		  	 sidpublico 		= '".$_REQUEST['publico']."',
		  	 sidemail 			= '".$_REQUEST['email']."',
		  	 ssiid 				= '".$_REQUEST['situacao']."',
		  	 sidmotivo 			= '".$_REQUEST['motivo']."',
		  	 sidimplantacao		= '".$_REQUEST['implantacao']."',
		  	 sidqtdeusuarios 	= '".$_REQUEST['qtdeusuarios']."',	
		  	 siddestaquebanner 	= '".$_REQUEST['dbanner']."',
		  	 siddestaqueaba 	= '".$_REQUEST['daba']."',
		  	 sidgestor		 	= '".$_REQUEST['sidgestor']."',
		  	 sidpublicar	 	= '".$_REQUEST['publicar']."',
		  	 unaid 		    	= '".$_REQUEST['orgao']."'		  	 
		  	WHERE
		  	 sidid = ".$_SESSION['sidid'];
    $db->executar($sql);
    
  	//insere eixos pde
	if ( $_REQUEST['exoid'] && $_REQUEST['exoid'][0] ){
		
		$sql = "DELETE FROM demandas.sistemaeixos WHERE sidid=".$_SESSION['sidid'];
	    $db->executar($sql);

		for($i=0; $i<count($_REQUEST['exoid']); $i++){
			$sql = "INSERT INTO demandas.sistemaeixos (sidid,exoid)
		    		VALUES (".$_SESSION['sidid'].",".$_REQUEST['exoid'][$i].")";
		    $db->executar($sql);
		}
		
	}    
    
	$db->commit();
	
	?>
    	<script>
            alert('Operação realizada com sucesso!');
		   	location.href='demandas.php?modulo=sistema/apoio/dadosSistemas&acao=A';
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
$titulo_modulo='Dados do Sistema';
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
			 siddescricao AS dsc, 
			 sidabrev AS abrev, 
			 sidfinalidade AS finalidade, 
		     sidrelacionado AS relacionado, 
		     sidpublico AS publico, 
		     sidstatus AS status,
		     sidemail AS email,
		     ssiid AS situacao, 
		     sidmotivo AS motivo,
   		     sidqtdeusuarios AS qtdeusuarios, 
		     sidimplantacao AS implantacao, 
		     unaid AS orgao,
		     siddestaquebanner as dbanner,
		     siddestaqueaba as daba,
		     sidgestor as sidgestor,
		     sidpublicar as publicar	     
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
	$abev		   = "";
	$finalidade    = "";
	$relacionado   = "";
	$publico       = "";
	$status        = "";
	$email         = "";
	$situacao      = "";
	$motivo        = "";
	$qtdeusuarios  = "";	
	$implantacao   = "";
	$orgao         = "";
	$sidgestor     = "";
	$publicar      = "";
	
	
endif;
	$act = '';
	
?>

<form method="POST"  name="formulario">

	<input type='hidden' name="modulo" value="<?=$modulo?>">
	<input type='hidden' name="act" value="">
	<input type="hidden" name="abrev_old" value="<?=$abrev?>">
	

    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     
      <tr>
        <td width="30%" align='right' class="SubTituloDireita">Código:</td>
	    <td colspan=2><?=campo_texto('sidid','S','N','',5,5,'','');?></td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Sistema:</td>
	     <td><?=campo_texto('dsc','S','S','',50,100,'','');?></td>
	     <td rowspan=3 align=right>
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
	    <td colspan=2><?=campo_texto('abrev','S','S','',15,15,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
	    <td colspan=2><?=campo_textarea('finalidade','S','S','',75,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Relacionado:</td>
	    <td colspan=2><?=campo_textarea('relacionado','N','S','',75,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Público Alvo:</td>
	    <td colspan=2><?=campo_textarea('publico','S','S','',75,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Situação:</td>
	    <td colspan=2><?php

	    	$sql = "select 	ssiid as codigo, 
               				ssidsc as descricao
						from demandas.sistemasituacao
						order by 1";	
	    	                             	    
			$db->monta_combo('situacao', $sql, 'S', "Selecione...", 'filtraSituacao', '', '', '150', 'S', 'situacao',false,null,'Situacao');?></td>
      </tr>
      	<?php $mostraMotivo='none';
		 if ($situacao == '4') $mostraMotivo='';
		 ?>
      
      <tr id=tr_motivo style="display:<?=$mostraMotivo?>;">
        <td align='right' class="SubTituloDireita">Justificativa Situação:</td>
	    <td colspan=2 id=td_motivo>     
	    <?php 
	    if (!$motivo) $motivo =  '';
	    ?>
			<?=campo_textarea('motivo','S','S','',75,3,'');?>	    
	    </td>
      </tr>
      	<?php $mostraQtde='none';
   	
		 if ($situacao == '2') {
		 	$mostraQtde='';
         		 }
         else  	$qtdeusuarios='0';		 
	 		 
		 ?>

      <tr id=tr_qtde style="display:<?=$mostraQtde?>;">
        <td align='right' class="SubTituloDireita">Quantidade de Usuários:</td>
	    <td colspan=2 id=td_qtde><?=campo_texto('qtdeusuarios','S','S','',15,15,'#########','');?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Data Implantação:</td>
	    <td colspan=2><?=campo_texto('implantacao','S','S','',8,15,'##/####','');?> (mm/aaaa)</td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Órgão Responsável:</td>
	    <td colspan=2><?php

	    	$sql = "select 	unaid as codigo, 	
                			unasigla || ' - ' || unadescricao as descricao
						from demandas.unidadeatendimento
						order by unasigla";
	    	                             	    
			$db->monta_combo('orgao', $sql, 'S', "Selecione...", '', '', '', '500', 'S', 'orgao',false,null,'orgao');?></td>
      </tr>
      

            
      <tr>
        <td align='right' class="SubTituloDireita">E-mail:</td>
	     <td colspan=2><?=campo_texto('email', 'N', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="email"'); ?></td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Eixo PDE:</td>
	    <td colspan=2><?php

			if($_SESSION['sidid']) {
				$exoid = $db->carregar("select 	e.exoid as codigo, 	
                			e.exodsc as descricao
						from demandas.sistemaeixos se
						inner join painel.eixo e ON e.exoid = se.exoid
						where sidid = ".$_SESSION['sidid']."
						order by 2");
			}
			$stSql = "select 	exoid as codigo, 	
                			exodsc as descricao
						from painel.eixo
						order by 2";
			combo_popup( "exoid", $stSql, "Selecione o(s) Eixo(s)", "600x600", 0, array(), "", "S", false, false, 5, 400 );
			
			echo obrigatorio();
			
			//combo_popup( 'exoid', $stSql, 'Selecione o(s) Eixo(s)', '360x460','','','','S','','',6,400,'','' );			
			?>
			
		</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Gestor do sistema:</td>
	    <td colspan=2>
	    	<?=campo_texto('sidgestor', 'S', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="sidgestor"'); ?>
	    </td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Publicar em Relatório:</td>
	       <td colspan=2>
            <input type="radio" name="publicar" value="S" <?=($publicar=='S'?"CHECKED":"")?>> SIM
            <input type="radio" name="publicar" value="N" <?=(($publicar=='N' || $publicar=='')?"CHECKED":"")?>> NÃO
        </td>
      </tr>
      

      
      <tr>
        <td align='right' class="SubTituloDireita">Destaque Banner:</td>
	       <td colspan=2>
            <input type="radio" name="dbanner" value="A" <?=($dbanner=='A'?"CHECKED":"")?>> SIM
            <input type="radio" name="dbanner" value="I" <?=(($dbanner=='I' || $dbanner=='')?"CHECKED":"")?>> NÃO
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Destaque Aba:</td>
	       <td colspan=2>
            <input type="radio" name="daba" value="A" <?=($daba=='A'?"CHECKED":"")?>> SIM
            <input type="radio" name="daba" value="I" <?=(($daba=='I' || $daba=='')?"CHECKED":"")?>> NÃO
        </td>
      </tr>


	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td colspan=2>
   			<input type="button" name="btinserir" value="Salvar" onclick="validar_cadastro()" class="botao">
   			&nbsp;
   			<input type="button" name="btcancela" value="Limpar" onclick="inicio();" class="botao">
   		</td>
	</tr>

      
    </table>

  
</form>




<script>

var d=document;


function inicio() {
 	location.href='demandas.php?modulo=sistema/apoio/dadosSistemas&acao=A';
}
  
function validar_cadastro() {

	
	if (trim(document.formulario.dsc.value) == "") 
	{
		alert("Favor informar Nome do Sistema.");
		document.formulario.dsc.focus();
		return;
	}
	
	if (trim(document.formulario.abrev.value) == "") 
	{
		alert("Favor informar Sigla do Sistema.");
		document.formulario.abrev.focus();
		return;
	}
	
	if (trim(document.formulario.finalidade.value) == "") 
	{
		alert("Favor informar Finalidade do Sistema.");
		document.formulario.finalidade.focus();
		return;
	}
/*	
	if (document.formulario.relacionado.value == "") 
	{
		alert("Favor informar relacionamento do Sistema.");
		document.formulario.relacionado.focus();
		return;
	}
*/	
	if (trim(document.formulario.publico.value) == "") 
	{
		alert("Favor informar Público Alvo do Sistema.");
		document.formulario.publico.focus();
		return;
	}
	
	if (trim(document.formulario.situacao.value) == "")
	{
		alert("Favor informar Situação do Sistema.");
		document.formulario.situacao.focus();
		return;		
	}

	if (document.formulario.situacao.value == 4) {
		if (trim(document.formulario.motivo.value) == "") 
		{
			alert("Situação 'Suspenso' é obrigatório informar Motivo.");
			document.formulario.motivo.focus();
			return;
		}
	}

	if (document.formulario.situacao.value == 2) {
		if (document.formulario.qtdeusuarios.value == 0)
		{
			alert("Situação 'Em Produção' é obrigatório informar a quantidade de usuários.");
			document.formulario.qtdeusuarios.focus();
			return;		
		}
	}
	
	if (trim(document.formulario.implantacao.value) == "")
	{
		alert("Favor informar Data de Implantação do Sistema.");
		document.formulario.implantacao.focus();
		return;		
	}
	var mes = document.formulario.implantacao.value.substring(0,2);
	var ano = document.formulario.implantacao.value.substring(3,7);
	
	if (mes < 1 || mes > 12){
		alert("Data de Implantação inválida.");
		document.formulario.implantacao.focus();
		return;		
	}		

	if (ano > 2030 || ano < 1980){
		alert("Data de implantação inválida");
		document.formulario.implantacao.focus();
		return;		
	}		
	
	if (trim(document.formulario.orgao.value) == "")
	{
		alert("Favor informar Órgão responsável pelo Sistema.");
		document.formulario.orgao.focus();
		return;		
	}

	var exoid = document.getElementById( 'exoid' );
	if ( !exoid[0].value ){
		alert( 'Favor selecionar ao menos um Eixo PDE.' );
		return;
	}
	
	
	if (trim(document.formulario.sidgestor.value) == "")
	{
		alert("Favor informar o Gestor do Sistema.");
		document.formulario.sidgestor.focus();
		return;
	}
	
		


	if(document.formulario.dbanner[0].checked == true){

		<?php if($dbanner=='A'){?>
		var total = <?=$totalBanner?>;
		<?php }else{?>
		var total = <?=$totalBanner+1?>;
		<?php }?>
		
		if( total > <?=DESTAQUE_BANNER_MAX?> ){
			alert("Limite máximo ultrapassado do DESTAQUE BANNER.");
			document.formulario.dbanner[1].checked = true;			
			return;
		}

		if( total < <?=DESTAQUE_BANNER_MIN?> ){
			alert("Quantidade total de DESTAQUE BANNER não pode ser menor que <?=DESTAQUE_BANNER_MIN?>.");
			document.formulario.dbanner[0].checked = true;			
			return;
		}
		
	}
	
	if(document.formulario.daba[0].checked == true){

		<?php if($daba=='A'){?>
		var total = <?=$totalAba?>;
		<?php }else{?>
		var total = <?=$totalAba+1?>;
		<?php }?>
				

		if( total > <?=DESTAQUE_ABA_MAX?> ){
			alert("Limite máximo ultrapassado do DESTAQUE ABA.");
			document.formulario.daba[1].checked = true;			
			return;
		}

		if( total < <?=DESTAQUE_ABA_MIN?> ){
			alert("Quantidade total de DESTAQUE ABA não pode ser menor que <?=DESTAQUE_ABA_MIN?>.");
			document.formulario.daba[0].checked = true;			
			return;
		}
		
	}	

	selectAllOptions( exoid );
	
	document.formulario.act.value="alterar";
    document.formulario.submit();

}



function filtraSituacao(ssiid) {
	
	trMotivo  = d.getElementById('tr_motivo');
	tdMotivo  = d.getElementById('td_motivo');
	trQtde	  = d.getElementById('tr_qtde');
	tdQtde 	  = d.getElementById('td_qtde');
	

	trMotivo.style.display = 'none';
	tdMotivo.style.display = 'none';
	trQtde.style.display = 'none';
	tdQtde.style.display = 'none';
	

	if(ssiid == 2){ //Em Produção
		trQtde.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdQtde.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}

	if(ssiid == 4){ //Suspenso
		trMotivo.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdMotivo.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
	
}



</script>
