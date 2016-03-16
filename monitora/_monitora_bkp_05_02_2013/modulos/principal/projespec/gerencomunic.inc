<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:monitorapje.inc
   finalidade: permitir a avaliação do projeto especial
   */
$modulo=$_REQUEST['modulo'];
if (! $_SESSION['pjeid']) 
{
	?>
	<script>alert('Escolha primeiro o Projeto Especial!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

if ($_REQUEST['act']=='excluiag')
{
	// exclusão de item na agenda
	  
       $sql = "update agenda_reuniao set agrstatus='I' where agrid=".$_REQUEST['doccod'];
       $saida = $db->executar($sql);
       $db->commit();
       	 $db->sucesso($modulo);	
}

if ($_REQUEST['act']=='excluicom')
{
	// exclusão de item na comunicação programada
	  
       $sql = "update comunicacao set comstatus='I' where comid=".$_REQUEST['doccod'];
       $saida = $db->executar($sql);
       $db->commit();
       	 $db->sucesso($modulo);	
}
if ($_REQUEST['act']=='excluiav')
{
	// exclusão de aviso
	  
       $sql = "update aviso set avsstatus='I' where avsid=".$_REQUEST['doccod'];
       $saida = $db->executar($sql);
       $db->commit();
       $db->sucesso($modulo);	
     
}

  $coordpje=false;
  $digit=false;

  if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) {
  	$coordpje = true;
  }
   // verifica se é digitador
  if ($db->testa_digitador($_SESSION['pjeid'],'E')) {
  	$digit = true;
  }
// verific se é super-usuário
  if ($db->testa_superuser())  {
  	$coordpje = true; 
  }
  
include  APPRAIZ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba($abacod_tela,$url,'');
//$titulo_modulo='Avaliação de Projeto Especial';
monta_titulo($titulo_modulo,'');
$sql= "select p.* , m.unmdsc, pr.prodsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod  where p.pjeid=".$_SESSION['pjeid'];
$res =  $db->pegalinha($sql);

// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

?>
<form name="formulario" method="post">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<input type=hidden name="doccod" >


<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$pjecod.'.'.$pjedsc?></td>
      </tr>
        <tr>
  <td></td>
  <td>
  <input type="button" name="btequipe" value="Equipe do Projeto" onclick="mostra_equipe()">
  </td>
  </tr>
 
</table>
<br>
<? // avisos ?>
<table  border="1" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr align="center">
    <td colspan="3"><b>AVISOS IMPORTANTES</td>
  </tr>
  <tr align="center">
    <td>Ações</td><td ><b>Avisos</td><td><b>Registrado por</td>
  </tr>  
  <?
    $sql = "SELECT DISTINCT 
    			'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar aviso\" onclick=\"altera_aviso('||av.avsid||')\">&nbsp;&nbsp;&nbsp;
    			 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Cancelar/Desativar aviso\" onclick=\"exclui_aviso('||av.avsid||')\">' AS acoes,
    			 av.avsdescricao AS descricao, 
    			 av.usucpf, 
    			 av.avsid,
    			 usu.usucpf, 
    			 usu.usuemail, 
    			 usu.usunome || ' ('|| ee.entnome ||')' AS usuario,
    			 av.avsdataini 
    		FROM 
    			aviso av 
    		INNER JOIN 
    			seguranca.usuario usu ON usu.usucpf = av.usucpf 
    		INNER JOIN 
    			entidade.entidade ee ON ee.entid = usu.entid 
    		INNER JOIN 
    			aviso_processo ap ON ap.avsid = av.avsid AND 
    								 ap.pjeid = $pjeid 
			WHERE 
				av.avsstatus='A' AND 
				av.avsdataini <= now() AND 
				av.avsdataini + av.avsvalidade >= now() 
			ORDER BY 
				av.avsdataini, 
				av.usucpf";

    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if ( $usuario )
            {
            	$linha = "<tr>";
            	if ( $coordpje or $criador )
            	{
            		$linha .= "<td>" . $acoes . "</td>";
            	}
            	else
            	{
            		$linha .= "<td></td>";
            	}
	        	$linha .= "<td><h2>".$descricao."</h2></td><td>".$usuario.' Telefone:'.$fone;
	        	if ( $usucpf <> $_SESSION['usucpf'] )
	        	{
		        	$linha .= '&nbsp;&nbsp;&nbsp;<img src="../imagens/email.gif" title="Envia e-mail" border="0" onclick="envia_email(\'' . $usucpf . '\');"> ';
	        	}
	        	$linha .= "</td></tr>";
            	print $linha;
            }
        }
     }
     else 
     print "<tr align='center'><td colspan='3'><font color='red'>Não há avisos válidos!</font></td><td></tr>";
// entrar com o botão de novo aviso
?>
<tr align="center" bgcolor="#CCCCCC">
	<td colspan="3"><?if ($coordpje or $digit){   ?>
              <input type="button" name="btincluiraviso" value="Incluir Aviso" onclick="Incluir_aviso(<?=$pjeid?>)" class="botao"><input type="button" name="btvoltar" value="Voltar" onclick="history.back()" class="botao"></td><?}?>
      </tr>    

</table>
<br>
<? // Agenda de reunião ?>
<table  border="1" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr align="center">
    <td colspan="8"><b>REUNIÕES AGENDADAS</td>
  </tr>
  <tr align="center">
    <td>Ações</td><td ><b>Assunto</td><td><b>LOCAL</td><td><b>Dia</td><td><b>Início</td><td><b>Duração(min)<BR>Prevista</td><td><b>Pauta</td><td><b>Registrado por</td>
  </tr>  
  <?
    $sql = "SELECT DISTINCT 
    			'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar item de agenda\" onclick=\"altera_agenda('||ar.agrid||')\">&nbsp;&nbsp;&nbsp;
    			 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Cancelar/Desativar item de agenda\" onclick=\"exclui_agenda('||ar.agrid||')\">' AS acoes, 
    			 ar.agrassunto AS assunto, 
    			 ar.usucpf, 
    			 ar.agrid, 
    			 u2.usucpf, 
    			 u2.usuemail, 
    			 u2.usunome || ' ('|| ee.entnome ||')' AS autor,
    			 to_char(ar.agrdata,'dd/mm/yyyy') AS data, 
    			 ar.agrdata,
    			 ar.agrhora AS hora, 
    			 ar.agrduracao AS duracao, 
    			 ar.agrpauta AS pauta , 
    			 ar.agrlocal AS local
    		FROM 
    			agenda_reuniao ar 
    		INNER JOIN 
    			seguranca.usuario u2 ON u2.usucpf = ar.usucpf 
    		INNER JOIN 
    			entidade.entidade ee ON ee.entid = u2.entid 
    		INNER JOIN 
    			agenda_reuniao_processo arp ON arp.agrid = arp.agrid AND 
    										   arp.pjeid = $pjeid 
			WHERE 
				ar.agrstatus='A' 
			ORDER BY 
				ar.agrdata DESC";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            
            //$sql = "select distinct u.usucpf, u.usuemail, u.usunome || ' ('|| o.orgdsc ||') <br>' as participante from agenda_reuniao_participante arp inner join seguranca.usuario u on u.usucpf = arp.usucpf inner join orgao o on o.orgcod=u.orgcod  where arp.agrid=$agrid order by participante";
            //$rs = $db->carregar($sql);
            
            ?>
			<tr>
				<td>
					<?php if ( $coordpje or $criador ) : ?>
						<?php print $acoes; ?>
						&nbsp;&nbsp;
						<img border="0" src="../imagens/grupo.gif" title="Ver participantes" onclick="mostra_participantes( '<?php echo $agrid; ?>' )"/>
						<?php $sql_arquivo = sql_vincula_arquivo( 'agenda_reuniao', $agrid ,'agrid'); ?>
						<?php popup_arquivo( 'arquivo_reuniao_' . $agrid . $agrid, $sql_arquivo, 'agrid', $agrid, 0, 400, 400 ); ?>
					<?php endif; ?>
				</td>
				<td><?= $assunto ?></td>
				<td><?= $local ?></td>
				<td><?= $data ?></td>
				<td><?= $hora ?></td>
				<td><?= $duracao ?></td>
				<td><?= $pauta ?></td>
				<td>
					<?= $autor ?>
					<?php if ( $usucpf <> $_SESSION['usucpf'] ) : ?>
						&nbsp;&nbsp;<img src="../imagens/email.gif" title="Envia e-mail" border="0" onclick="envia_email( '<?= $usucpf ?>');"> 
					<?php endif; ?>  
				</td>
			</tr>
			<?php
			/*
            <tr>
            	<td ><b>Participantes</td>
            	<td colspan="7" align="left">
            	<?
            	if ( $rs && count($rs) > 0 )
				{
					foreach ( $rs as $linha )
					{
						foreach($linha as $k=>$v) ${$k}=$v;
	              		print $participante;
              		}
				}           	
            	?></td>            
            </tr>
                        <tr><td colspan="9" ><hr></tr>
			*/
			?>
			</tr>
			<?php
		}
	}
	else
	{
		?>
		<tr align='center'>
			<td colspan='9'>
				<font color='red'>Não há reuniões agendadas!</font>
			</td>
			<td>
		</tr>
		<?php
	}
	?>
	<tr align="center" bgcolor="#CCCCCC">
		<td colspan="8">
			<?php if ( $coordpje or $digit ) : ?>
				<input type="button" name="btincluirreuniao" value="Incluir Reunião" onclick="Incluir_reuniao( <?= $pjeid ?> )" class="botao"><input type="button" name="btvoltar" value="Voltar" onclick="history.back()" class="botao">
			<?php endif; ?>
		</td>
	</tr>  
</table>

<br>
<? // comunicação gerencial ?>
<table  border="1" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr align="center">
    <td colspan="3"><b>COMUNICAÇÕES PROGRAMADAS</td>
  </tr>
  <tr align="center">
    <td>Ações</td><td ><b>Assunto</td><td><b>Registrado por</td>
  </tr>  
  <?
    $sql = "SELECT DISTINCT 
    			'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar item de comunicação.\" onclick=\"altera_comunicacao('||c.comid||')\">&nbsp;&nbsp;&nbsp;
    			 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Cancelar/Desativar item de comunicação.\" onclick=\"exclui_comunicacao('||c.comid||')\">' AS acoes, 
    			 a.assdsc AS assunto, 
    			 c.usucpf, 
    			 u.usucpf, 
    			 u.usuemail, 
    			 u.usunome || ' ('|| ee.entnome ||')' AS autor,
    			 to_char(c.comdataini,'dd/mm/yyyy') AS data 
    		FROM 
    			comunicacao c 
    		INNER JOIN 
    			seguranca.usuario u ON u.usucpf = c.usucpf 
    		INNER JOIN 
    			entidade.entidade ee ON ee.entid = u.entid 
    		INNER JOIN 
    			comunicacao_processo cp ON cp.comid = c.comid AND 
    									   cp.copcampo = 'pjeid' AND 
    									   cp.copvalorcampo = $pjeid 
			INNER JOIN 
				assuntocomunicacao a ON c.asscod = a.asscod 
			WHERE 
				c.comstatus='A' 
			ORDER BY 
				data";

    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;           
            ?>
            <tr>
                <td><?if ($coordpje or $criador){print $acoes;}   ?> </td>
            	<td><?=$assunto?></td>
           		<td><?
	        	$linha= $autor;
	        	if ($usucpf <> $_SESSION['usucpf'])
	        {
	        	$linha .= '&nbsp;&nbsp;&nbsp;<img src="../imagens/email.gif" title="Envia e-mail" border="0" onclick="envia_email(\''.$usucpf.'\');"> ';
	        }
	        	print $linha;
	        	

           		
           		?></td>
            </tr>
                       <tr><td colspan="3" ><hr></tr>
            <?
        }
     }
     else 
     print "<tr align='center'><td colspan='3'><font color='red'>Não há comunicações programadas!</font></td><td></tr>";
     ?>

<tr align="center" bgcolor="#CCCCCC">
	<td colspan="3">
		<?if ($coordpje or $digit){   ?>
              <input type="button" name="btincluirreuniao" value="Incluir Comunicação" onclick="Incluir_comunicacao(<?=$pjeid?>)" class="botao"><input type="button" name="btvoltar" value="Voltar" onclick="history.back()" class="botao">
             <?}?>
     </td>
</tr>  

</table>

</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
<script>
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    function Incluir_reuniao(cod)
    
    {
       e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/incluireuniao&acao=I&id="+cod+"&campo=pe";
          window.open(e, "Inclusaodereuniao","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=530");
    } 
    function altera_agenda(cod)    
    {
       e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/incluireuniao&acao=I&campo=pe&agrid="+cod;
          window.open(e, "Inclusaodereuniao","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=530");
    }     
    function Incluir_aviso(cod)    
    {
       e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/incluiraviso&acao=I&id="+cod+"&campo=pe";
          window.open(e, "Inclusaodereuniao","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=530");
    }
	
    function Incluir_comunicacao(cod)    
    {
       e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/incluircomunicacao&acao=I&id="+cod+"&campo=pe";
          window.open(e, "Inclusaodereuniao","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=530");
    }   
    function altera_comunicacao(cod)    
    {
       e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/incluircomunicacao&acao=I&campo=pe&comid="+cod;
          window.open(e, "Inclusaodereuniao","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=530");
    }      
    
  function exclui_agenda(cod) 
  {
  	if( window.confirm( "Você optou por excluir/desativar um item da agenda.\n Confirma sua escolha?") )
         {
            document.formulario.doccod.value=cod;
            document.formulario.act.value = 'excluiag';
            document.formulario.submit();
         }
  } 
  function exclui_aviso(cod) 
  {
  	if( window.confirm( "Você optou por excluir/desativar um aviso.\n Confirma sua escolha?") )
         {
            document.formulario.doccod.value=cod;
            document.formulario.act.value = 'excluiav';
            document.formulario.submit();
         }
  }
  
    function altera_aviso(cod)    
    {
       e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/incluiraviso&acao=I&campo=pe&avsid="+cod;
          window.open(e, "Inclusaoaviso","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=530");
    } 
  function exclui_comunicacao(cod) 
  {
  	if( window.confirm( "Você optou por excluir/desativar uma comunicação programada.\n Confirma sua escolha?") )
         {
            document.formulario.doccod.value=cod;
            document.formulario.act.value = 'excluicom';
            document.formulario.submit();
         }
  }    

  function mostra_equipe()
  {
  	 e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
  }
  
	function mostra_participantes( agrid )
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verparticipantes&acao=A&agrid=" + agrid;
		window.open( e, "participantes", "menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=200'");
	}
  
</script>



