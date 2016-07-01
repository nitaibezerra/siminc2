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

### function, Copia diretório ###
function copyDir ($source, $dest) {
	$arq = (array) scandir($source);
	
	// Caso não exista o diretório de destino cria o mesmo
	if (!is_dir($dest))
		mkdir($dest); 
		
	// LOOP nos arquivos do diretório que será copiado	
	foreach ($arq as $arq):
	
		## Caso seja um diretório aplica recursividade ##
		if (strpos($arq, ".") === false && is_dir($source."/".$arq)):
			copyDir($source."/".$arq, $dest."/".$arq);
			continue;
		## Caso seja um arquivo ".php ou .inc" copia para o novo diretório
		elseif (strrpos($arq, ".php") || strrpos($arq, ".inc")):
			copy($source."/".$arq, $dest."/".$arq);
			continue;
		endif;
	endforeach;
	return true;
}

########### AÇÕES Cadastro/Atualização de sistema ###########
/* 
 * Será feita uma CÓPIA do sistema RAIZ => sisid = 999 
 * Criado com a finalidade de ser o sistema padrão 
*/
if (($_REQUEST['act'] == 'inserir') and (! is_array($msgerro))):
	
    $sql= "SELECT
  	  	   sidid as sidid, 
  	  	   siddescricao as siddescricao
  		  FROM
  		   demandas.sistemadetalhe 
  		  WHERE
  		   siddescricao = '".$_REQUEST['dsc']."' OR
  		   sidabrev = '".$_REQUEST['abrev']."';";
   $usu = $db->pegaUm($sql);
   
   
   unset($sql);
	
   if ($usu) {
	   // existe perfil identico, logo, tem que bloquear
	   ?>
	      <script>
	         alert ('O Sistema: <?=$_REQUEST['dsc']?> já se encontra cadastrado no sistema.');
	         history.back();
	      </script>
	   <?
	     exit();
	}

//////// Verifica ESQUEMA /////////
/*
 * Se há esquema == diretório novo, interrompe cadastro
 */	
	 $sql = sprintf("SELECT
				     sidid 
				    FROM
				     demandas.sistemadetalhe 
				    WHERE
				     siddescricao = '%s'",
			$_REQUEST['dsc']);
	if ($db->pegaUm($sql) == "t" ){
		die('<script>
	         	alert(\'Já existe o esquema "'.$_REQUEST['dsc'].'" no Banco de Dados.\nOperação não realizada!\');
	         	history.back();
	      	 </script>');
	}	

///////// Cadastro do sistema //////////	   
    $sql = "INSERT INTO demandas.sistemadetalhe (
    			siddescricao,
    			sidurl,
    			sidabrev,
    			sidfinalidade,
    			sidrelacionado,
    			sidpublico,
    			sidstatus,
    			sidemail,
			    sipid, 
			    sitid, 
			    sidoutrastecno,
			    ssiid, 
			    sidmotivo, 
			    sidimplantacao,
			    sidqtdeusuarios, 
			    sidpublicar,
			    unaid 			
    			) 
    			VALUES (".
		   "'".$_REQUEST['dsc']."',".
		   "'".$_REQUEST['url']."',".
		   "'".$_REQUEST['abrev']."',".   
		   "'".$_REQUEST['finalidade']."',".
		   "'".$_REQUEST['relacionado']."',".
		   "'".$_REQUEST['publico']."',".
		   "'".$_REQUEST['status']."',".
		   "'".$_REQUEST['email']."',".
		   "'".$_REQUEST['plataforma']."',".
		   "'".$_REQUEST['tecnologia']."',".
       	   "'".$_REQUEST['outras']."',".
    	   "'".$_REQUEST['situacao']."',".
    	   "'".$_REQUEST['motivo']."',".
    	   "'".$_REQUEST['implantacao']."',".
    	   "'".$_REQUEST['qtdeusuarios']."',".    
    	   "'".$_REQUEST['publicar']."',".
    	   "'".$_REQUEST['orgao']."') RETURNING sidid";
   $sisidNew = $db->pegaUm($sql);
   $db->commit();
   unset($sql);
   
    ?>
    	<script>
            alert('Operação realizada com sucesso!');
		  	location.href='demandas.php?modulo=sistema/apoio/sistemas&acao=A';
        </script>
    <?
	exit();
endif;

if ( ($_REQUEST['act']=='alterar') and (! is_array($msgerro)) ):
   // fazer alteração de perfil na base de dados.
  	$sql = "UPDATE 
   			 demandas.sistemadetalhe 
   			SET 
		   	 siddescricao 		= '".$_REQUEST['dsc']."',
		  	 sidurl 			= '".$_REQUEST['url']."',
		  	 sidabrev 			= '".$_REQUEST['abrev']."',
		  	 sidfinalidade 		= '".$_REQUEST['finalidade']."',
		  	 sidrelacionado 	= '".$_REQUEST['relacionado']."',
		  	 sidpublico 		= '".$_REQUEST['publico']."',
		  	 sidstatus 			= '".$_REQUEST['status']."',
		  	 sidemail 			= '".$_REQUEST['email']."',
 		  	 sipid 				= '".$_REQUEST['plataforma']."',
		  	 sitid 				= '".$_REQUEST['tecnologia']."',
		  	 sidoutrastecno 	= '".$_REQUEST['outras']."',
		  	 ssiid 				= '".$_REQUEST['situacao']."',
		  	 sidmotivo 			= '".$_REQUEST['motivo']."',
		  	 sidimplantacao		= '".$_REQUEST['implantacao']."',
		  	 sidqtdeusuarios 	= '".$_REQUEST['qtdeusuarios']."',		  	 
		  	 sidpublicar 		= '".$_REQUEST['publicar']."',		  	 		  	 		  	 		  	  		  	 
		  	 unaid 		    	= '".$_REQUEST['orgao']."'		  	 
		  	WHERE
		  	 sidid = ".$_POST['sisidEdit'];
  	
    $saida = $db->executar($sql);
	$db->commit();
	?>
    	<script>
            alert('Operação realizada com sucesso!');
		   	location.href='demandas.php?modulo=sistema/apoio/sistemas&acao=A';
        </script>
    <?
	exit();
endif;

if ( ($_REQUEST['act']=='excluir') and (! is_array($msgerro)) ):
   // fazer exclusão de perfil na base de dados.
   	
	$sql1 = "SELECT u.sidid 
			 FROM demandas.usuarioresponsabilidade AS u
			 LEFT JOIN demandas.demanda AS d ON u.sidid = d.sidid 
			 WHERE u.sidid = ". $_REQUEST['sidid'];
	 
	$carrega = $db->carregar($sql1);
 	 
	if(count( $carrega ) > 1 )
	{
		?>
		<script >
			alert('Operação não pode ser realizada devido a dados vinculados com usuario');
			location.href = 'demandas.php?modulo=sistema/apoio/sistemas&acao=A';
		</script>
		<? 
	}else{
     
    $sql  = "DELETE FROM demandas.sistemadetalhe WHERE sidid = ".$_REQUEST['sidid'];
    $saida = $db->executar( $sql );
    $db->commit();
	?>
    	<script>
            alert('Operação realizada com sucesso!');
		   	location.href='demandas.php?modulo=sistema/apoio/sistemas&acao=A';
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
$titulo_modulo='Cadastro de Sistema';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<div align="center">
<center>
<?
//atribui valores as variaveis
$modulo=$_REQUEST['modulo'] ;//
//$sidid = md5_decrypt($_REQUEST['sisidEdit'],'');
$sidid = $_REQUEST['sidid'];

if ($sidid):
	$sql= "SELECT
			 sidid, 
			 siddescricao AS dsc, 
			 sidurl AS url, 
			 sidabrev AS abrev, 
			 sidfinalidade AS finalidade, 
		     sidrelacionado AS relacionado, 
		     sidpublico AS publico, 
		     sidstatus AS status,
		     sidemail AS email,
		     sipid AS plataforma, 
 		     sitid AS tecnologia,
 		     sidoutrastecno AS outras, 
		     ssiid AS situacao, 
		     sidmotivo AS motivo,
   		     sidqtdeusuarios AS qtdeusuarios, 
		     sidimplantacao AS implantacao, 
		     unaid AS orgao,  
             sidpublicar AS publicar 	     
		   FROM
		     demandas.sistemadetalhe 
		   WHERE 
		     sidid = ".$sidid."
		   ORDER BY
		     siddescricao";
	$dados = $db->carregar($sql);
	
	
	#### Transforma $dados[0] em Variáveis ####
	extract($dados[0]);
	
else:
		
	$sidid 		   = "";
	$url		   = "";
	$abev		   = "";
	$finalidade    = "";
	$relacionado   = "";
	$publico       = "";
	$status        = "A";
	$email         = "";
	$plataforma    = "";
	$tecnologia    = "";
	$outras		   = "";
	$situacao      = "";
	$motivo        = "";
	$qtdeusuarios  = "";	
	$implantacao   = "";
	$orgao         = "";
	$publicar      = "";
	
endif;
	$act = '';
	
?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="sisidEdit" value="<?=$sidid?>">
<input type='hidden' name="act" value=<?=$act?>>
    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <!-- 
      <tr>
        <td align='right' class="SubTituloDireita">Código:</td>
	    <td><?//=campo_texto('sisid','S',($sisid != ""?'N':'S'),'',5,5,'','');?></td>
      </tr>
      -->

      <tr>
        <td align='right' class="SubTituloDireita">Plataforma:</td>
	    <td><?php
  	
	    	
	    	$sql = "select 	sipid as codigo, 
               				sipdsc as descricao
						from demandas.sistemaplataforma
						order by descricao";	
	    	                             	    
			$db->monta_combo('plataforma', $sql, 'S', "Selecione...", 'filtraPlataforma', '', '', '150', 'S', 'plataforma',false,null,'Plataforma');?></td>
      </tr>
      <!-- tr>
			<td align='right' class="SubTituloDireita">Plataforma:</td>
			<td>
				<table width="20%">
				<?/* $sql = "SELECT
							'<input type=\'radio\' name=\'sipid\' value=\'' || sipid || '\' onclick=\'filtraPlataforma('|| sipid ||');\'/>' || sipdsc || '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'AS campo 
						   FROM
						    demandas.sistemaplataforma
						   WHERE
						    sipstatus = 'A'
							ORDER BY 
							 sipdsc";
					$radio = (array) $db->carregar($sql);
					
					foreach ($radio as $radio){
						//$a++;
						//echo $a == 1 || $a == 7 || $a == 13 ? '<TR>' : '';
						//echo  '<td>'.$radio['campo'].'</td>';
						//echo $a == 3 || $a == 9 || $a == 15 ? '</TR>' : '';
						echo ($a%3) == 0 ? '<TR>' : '';
						echo  '<td>'.$radio['campo'].'</td>';
						echo ($a+1%3) == 0 ? '</TR>' : '';
						$a++;
					}*/	
			    ?>
			    </table>
			</td>
	  </tr-->	
		<?php $mostraUrl='none';
		 if ($plataforma == '1') $mostraUrl='';
		 ?>
      <tr id=tr_url style="display:<?=$mostraUrl?>;">
        <td align='right' class="SubTituloDireita">Url:</td>
	    <td id=td_url>
	     <?php
	     if (!$url) $url =  '';
	     ?>
	    <?=campo_texto('url','S',$sidid ? 'S' : 'S','',50,100,'','','','','','','',$url);?>
	    </td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Tecnologia:</td>
	    <td><?php

	    	$sql = "select 	sitid as codigo, 
               				sitdsc as descricao
						from demandas.sistematecnologia
						where sitid  not in (6)
						order by 2";	
			$arraysql = $db->carregar( $sql );	    	                             	    

	    	$sql = "select 	sitid as codigo, 
               				sitdsc as descricao
						from demandas.sistematecnologia
						where sitid  in (6)
						";	
			$arraysql2 = $db->pegalinha( $sql );	    	                             	    
			
			array_push($arraysql,$arraysql2);
			
			$db->monta_combo('tecnologia', $arraysql, 'S', "Selecione...", 'filtraTecnologia', '', '', '100', 'S', 'tecnologia',false,null,'Tecnologia');?></td>
      </tr>

		<?php $mostraOutras='none';
		 if ($tecnologia == '6') $mostraOutras='';
		 ?>
      <tr id=tr_outras style="display:<?=$mostraOutras?>;">
        <td align='right' class="SubTituloDireita">Especificar (Outras):</td>
	    <td id=td_outras>
	     <?php
	     if (!$outras) $outras =  '';
	     ?>
	    <?=campo_texto('outras','S',$sidid ? 'S' : 'S','',50,100,'','','','','','','',$outras);?>
	    </td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Situação:</td>
	    <td><?php

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
	    <td id=td_motivo>     
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
	    <td id=td_qtde><?=campo_texto('qtdeusuarios','S','S','',15,15,'#########','');?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Data Implantação:</td>
	    <td><?=campo_texto('implantacao','S','S','',8,15,'##/####','');?> (mm/aaaa)</td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Órgão Responsável:</td>
	    <td><?php

	    	$sql = "select 	unaid as codigo, 	
                			unasigla || ' - ' || unadescricao as descricao
						from demandas.unidadeatendimento
						order by unasigla";
	    	                             	    
			$db->monta_combo('orgao', $sql, 'S', "Selecione...", '', '', '', '500', 'S', 'orgao',false,null,'orgao');?></td>
      </tr>
      

            
      <tr>
        <td align='right' class="SubTituloDireita">E-mail:</td>
	     <td><?=campo_texto('email', 'N', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="email"'); ?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Publicar em Relatório:</td>
	       <td>
            <input type="radio" name="publicar" value="S" <?=($publicar=='S'?"CHECKED":"")?>> Sim
            <input type="radio" name="publicar" value="N" <?=($publicar=='N'?"CHECKED":"")?>> Não
        </td>
      </tr>
      
      <tr>
        <td align='right' class="SubTituloDireita">Status:</td>
	       <td>
            <input type="radio" name="status" value="A" <?=($status=='A'?"CHECKED":"")?>> Ativo
            <input type="radio" name="status" value="I" <?=($status=='I'?"CHECKED":"")?>> Inativo
        </td>
      </tr>
      <!--  <tr>
        <td align='right' class="SubTituloDireita">Tabelas de segurança:</td>
	       <td>
            <input type="radio" name="tblseg" <?=$sisid ? 'disabled="disabled"' : '';?> value="A" <?=($tblseg=='A'?"CHECKED":"")?>> Ativo
            <input type="radio" name="tblseg" <?=$sisid ? 'disabled="disabled"' : '';?> value="I" <?=($tblseg=='I'?"CHECKED":"")?>> Inativo
        </td>
      </tr>-->
<? if ($sidid) { ?>
	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
   		<input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao"></td>
 	</tr>
<? } else { ?>
	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao">
   		<input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao"></td>
	</tr>
<? } ?>
      
    </table>

  
</form>


<? 

//exibe listagem dos sistemas cadastrados
$sql= "select 
		'<div align=center><a href=\"demandas.php?modulo=sistema/apoio/sistemas&acao=A&sidid=' || sidid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/sistemas&acao=A&sidid=' || sidid || '&act=excluir\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a>' AS op,
		sidid as codigo2, 
		siddescricao as descricao2, 
		sidabrev as abreviacao2, 
		CASE
			WHEN sidstatus = 'I' THEN '<font color=red>Inativo</font>' 
			WHEN sidstatus = 'A' THEN '<font color=blue>Ativo</font>'
		END as status2
 		from demandas.sistemadetalhe 
 		order by siddescricao ";
$cabecalho = array("Ação","Cód","Sistema","Abreviação","Status");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');

?>


<script>

var d=document;

function strSpecial(string){
	var search = 'áâàãÁÂÀÃéêèÉÊÈíîìÍÎÌóõòõÓÔÌÕúûùÚÛÙ!@#$%¨&*()-+][}{~^´`?:;,"';

	for (i = 0; i < search.length; i++){
		if ( string.indexOf(search.substr(i,1)) >= 0 ){
			alert('Esse caractere é inválido para o campo: " '+search.substr(i,1)+' "');
			return false;
		}		
	}
	return true;
}



function altera_cadastro(cod) {
    document.formulario.sisidEdit.value = cod;
    document.formulario.act.value = 'alterar';
    document.formulario.submit();
}



function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este item?")
	if (questao){
		window.location = url;
	}
}

function inicio() {
 	document.formulario.act.value = '';
 	location.href='demandas.php?modulo=sistema/apoio/sistemas&acao=A';
}
  
function validar_cadastro(cod) {

	/*
	if (document.formulario.sisid.value == "") 
	{
		alert("Sistema sem código.");
		document.formulario.sisid.focus();
		return;
	}
	*/
	
	if (document.formulario.dsc.value == "") 
	{
		alert("Favor informar descrição do Sistema.");
		document.formulario.dsc.focus();
		return;
	}
	
	if (document.formulario.abrev.value == "") 
	{
		alert("Favor informar abreviação do Sistema.");
		document.formulario.abrev.focus();
		return;
	}
	
	if (document.formulario.finalidade.value == "") 
	{
		alert("Favor informar finalidade do Sistema.");
		document.formulario.finalidade.focus();
		return;
	}
	
	if (document.formulario.relacionado.value == "") 
	{
		alert("Favor informar relacionamento do Sistema.");
		document.formulario.relacionado.focus();
		return;
	}
	
	if (document.formulario.publico.value == "") 
	{
		alert("Favor informar público alvo do Sistema.");
		document.formulario.publico.focus();
		return;
	}
	
//	if (document.formulario.diret.value == "") 
//	{
//		alert("Sistema sem diretorio.");
//		document.formulario.diret.focus();
//		return;
//	}
//	
//	if (!strSpecial(document.formulario.diret.value)){
//		document.formulario.diret.focus();
//		return;
//	}

	if (document.formulario.plataforma.value == "")
	{
		alert("Favor informar plataforma do Sistema.");
		document.formulario.plataforma.focus();
		return;		
	}
//	alert(document.formulario.url.value.substr(0,7));
//    return;

	
	
	if (document.formulario.plataforma.value == 1){	 
		if (document.formulario.url.value == "http:\\\\" || document.formulario.url.value == "")  {
			alert("Plataforma WEB é obrigatório informar URL.");
			document.formulario.url.focus();
			return;
		}
	}

	if (document.formulario.tecnologia.value == "")
	{
		alert("Favor informar tecnologia do Sistema.");
		document.formulario.tecnologia.focus();
		return;		
	}

	if (document.formulario.tecnologia.value == 6){	 
		if (document.formulario.outras.value == "")  {
			alert("Tecnologia 'Outras' é obrigatório informar especificação.");
			document.formulario.outras.focus();
			return;
		}
	}

	
	if (document.formulario.situacao.value == "")
	{
		alert("Favor informar situação do Sistema.");
		document.formulario.situacao.focus();
		return;		
	}

	if (document.formulario.situacao.value == 4) {
		if (document.formulario.motivo.value == "") 
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
	
	if (document.formulario.implantacao.value == "")
	{
		alert("Favor informar data implantação do Sistema.");
		document.formulario.implantacao.focus();
		return;		
	}
	var mes = document.formulario.implantacao.value.substring(0,2);
	var ano = document.formulario.implantacao.value.substring(3,7);
	
	if (mes < 1 || mes > 12){
		alert("Favor informar data de implantação com 'mês' válido.");
		document.formulario.implantacao.focus();
		return;		
	}		

	if (ano > 2030 || ano < 1980){
		alert("Favor informar data de implantação com 'ano' válido.");
		document.formulario.implantacao.focus();
		return;		
	}		
	
	if (document.formulario.orgao.value == "")
	{
		alert("Favor informar Órgão responsável pelo Sistema.");
		document.formulario.orgao.focus();
		return;		
	}
	
	if (document.formulario.email.value == "")
	{
		alert("Favor informar e-mail.");
		document.formulario.email.focus();
		return;
	}

	if ((document.formulario.publicar[0].checked == 0) && (document.formulario.publicar[1].checked == 0)) 
	{
		alert("Favor informar opção de publicar em relatório.");
		document.formulario.publicar.focus();
		return;
	}

	
//	if (document.formulario.paginainicial.value == "") 
//	{
//		alert("Sistema sem página inicial.");
//		document.formulario.paginainicial.focus();
//		return;
//	}
	/*
	if(document.formulario.tel.value == "")
	{
		alert("Sistema sem telefone.");
		document.formulario.tel.focus();
		return;
	}
	
	if(document.formulario.fax.value == "")
	{
		alert("Sistema sem fax.");
		document.formulario.fax.focus();
		return;
	}
	*/
//	if ((document.formulario.exerc[0].checked == 0) && (document.formulario.exerc[1].checked == 0)) 
//	{
//		alert("Sistema sem exercício.");
//		document.formulario.exerc.focus();
//		return;
//	}
	
	if ((document.formulario.status[0].checked == 0) && (document.formulario.status[1].checked == 0)) 
	{
		alert("Favor informar status do cadastro.");
		document.formulario.status.focus();
		return;
	}
	
	
	if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
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
