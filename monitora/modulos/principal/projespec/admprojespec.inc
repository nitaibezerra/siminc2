 <?
  /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:admprojespec.inc
   finalidade: permitir a administração do projeto especial
   */
   $modulo=$_REQUEST['modulo'] ;//
   unset($_SESSION['pjeid']);

if ($_POST['exclui'])
{
    $sql = "update projetoespecial set pjestatus='I' where pjeid=".md5_decrypt($_POST['exclui'],'');
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}   
 
if($_REQUEST['pjeid'] and $_REQUEST['act']=='editar') {
	   $_SESSION['pjeid']= md5_decrypt($_REQUEST['pjeid'],'');
        $sql= "select * from projetoespecial where pjeid=".md5_decrypt($_REQUEST['pjeid'],'');
        $saida = $db->recuperar($sql,$res);        
        if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
        }
 else {

	$pjecod = $_REQUEST['pjecod'];
	$pjedsc =$_REQUEST['pjedsc'];
	$pjeid=0;
 }

include APPRAIZ."includes/cabecalho.inc";

print '<br>';
$db->cria_aba($abacod_tela,$url,'');

$titulo_modulo='Administração de Projetos Especiais';
monta_titulo($titulo_modulo,'');


// variáveis gerais
?>
<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name='act'>
<input type='hidden' name='ver' value='0'>
<input type='hidden' name='exclui' value=0>
<input type='hidden' name="pjeid" value=<?=$pjeid?>>



<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>Ações</strong></td>
      <td valign="top" class="title"><strong>Código</strong></td>
      <td valign="top" class="title"><strong>Descrição</strong></td>
      <td valign="top" class="title"><strong>Coordenador(a)</strong></td>
      <td valign="top" class="title"><strong>Status</strong></td>      
      <td valign="top" class="title"><strong>Acompanhamento</strong></td>       
    </tr>
</thead>
<tbody>
<?

$sql = "select pe.pjeid as codigo, trim(ug.ungabrev) ||'-'|| trim(pe.pjecod) as codigopje, trim(pe.pjedsc) as descricao, ts.tpsdsc as status,pe.pjeprevistoano from monitora.projetoespecial pe inner join unidadegestora ug using (ungcod) inner join tiposituacao ts on ts.tpscod=pe.tpscod group by pe.pjecod, pe.pjeid, ug.ungabrev, pe.pjedsc, tpsdsc,pjeprevistoano order by pjecod ";
$RS2 = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS2);
if ($nlinhas >= 0)
{
   for ($i=0; $i<=$nlinhas;$i++)
   {
   	 $res = $db->carrega_registro($RS2,$i);
    // a linha abaixo transforma em variáveis todos os campos do array
     if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     // seleciona o coordenador (a)
     $sql="select case when u.usucpf !='".$_SESSION['usucpf']."' then u.usunome ||'<br><img src=\"../imagens/email.gif\" title=\"Envia e-mail\" border=\"0\" onclick=\"envia_email(\'' || u.usucpf || '\');\">&nbsp;&nbsp;'||u.usufoneddd || '-'||u.usufonenum else u.usunome ||'<br>'||u.usufoneddd || '-'||u.usufonenum end as coord from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.pjeid=$codigo and ur.pflcod=47";
     
   
     $coord=$db->pegaUm($sql);
     // seleciona o acompanhamento físico
     $avgrealizado=0;
     $texto = 'Sem avaliação!';
     $cor='';
  $sql = "select ag.avgtexto as texto,ag.avgrealizado, ts.tpsdsc as situacao,case when ag.corcod=1 then '<img border=\"0\" src=\"../imagens/p_verde.gif\" title=\"Estável!\">' when  ag.corcod=2 then '<img border=\"0\" src=\"../imagens/p_amarelo.gif\" title=\"Merece atenção!\">' when ag.corcod=3 then '<img border=\"0\" src=\"../imagens/p_vermelho.gif\" title=\"Crítico!\">' end as cor from monitora.avaliacaogenerico ag inner join tiposituacao ts on ts.tpscod=ag.tpscod where ag.avgliberada='t' and ag.pjeid=$codigo and ag.avgstatus='A' order by ag.avgdata desc limit 1";
 
     $linha=$db->pegalinha($sql);
     if(is_array($linha)) foreach($linha as $k=>$v) ${$k}=$v;
     if (! $texto) $texto = 'Sem avaliação!';
     if ($pjeprevistoano)
     {
     	// há previsão de meta     	
     	$realizado = number_format(($avgrealizado/$pjeprevistoano)*100,0).' %';
     	//dbg($realizado);
     }
     // 
	  if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
	  // aqui testo se pode administrar projetos especiais
	  if ($db->testa_responsavel_projespec())
	  {
	      // aqui testa se o usuário foi o autor do projeto
	      if ($usucpf==$_SESSION['usucpf'])
	        { // então pode alterar ou mesmo excluir o projeto
      			print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/alterar.gif" title="Alterar/Consultar o Projeto." onclick="altera_proj('.$codigo.')">&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir o Projeto." onclick="excluir_proj('.$codigo.",'".$codigopje.')"></td>';
	  		}
	  	else {	
	  	  // então não é o autor, só pode ver ou criar	
      			print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/alterar.gif" title="Ver o Projeto." onclick="ver_proj('.$codigo.')"></td>';
	  	}
	  }      
      else // então não é o autor, só pode ver
          print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/alterar.gif" title="Ver o Projeto." onclick="ver_proj('.$codigo.')"></td>';
         
      print "<td>$codigopje</td><td>$descricao</td><td>$coord</td><td>$status</td><td><span onmouseover=\"return escape('$texto')\">$cor<br>Realizado:$realizado</span></td></tr>";
      
      //'<span onmouseover=\"return escape(\'' || ag.avgtexto || '\')\">'
  }
}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de Projeto Especial</td></tr>';
}

//Verifica se é o gerente do projeto ou super usuário
       $sql="select p.pflcod,p.pfldsc from perfil p inner join perfilusuario pu on p.pflcod=pu.pflcod and  pu.usucpf='".$_SESSION['usucpf']."' where p.pflcod = 12 or p.pflcod = 62 and pu.usucpf='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
       //dbg($sql,1,'');     
       $res =  $db->pegalinha($sql);
       	//Caso seja gerente de projeto ou super usuário mostra botão inclui
       	if(is_array($res)){

print '<tr class="'.$marcado.'"><td></td><td><input type="button" value="Inclui novo Projeto" onclick="inclui_proj()"> </td></tr>';
       	}



?>
</tbody>
    </table>
    </center>
  </div>
</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
<script>
   function inclui_proj()
   {   	
       location.href ='monitora.php?modulo=principal/projespec/cadprojespec&acao=I';
   }


  function excluir_proj(cod,dsc) {
    if( window.confirm( "Confirma a exclusão do Projeto "+dsc+" ?") )
    {
	document.formulario.exclui.value = cod;
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;
  }
  function altera_proj(cod) {
	
  	location.href ='monitora.php?modulo=principal/projespec/cadprojespec&acao=A&ver=2&id='+cod;

  }
  function ver_proj(cod) {
	
  	//location.href ='monitora.php?modulo=principal/projespec/cadprojespec&acao=A&ver=1&id='+cod;
  //	location.href ='monitora.php?modulo=principal/projespec/plantrabpje&acao=I&id='+cod;
  	location.href ='monitora.php?modulo=principal/projespec/plantrabdet/plantrabdet&acao=A&ver=2&id='+cod;
  	//monitora.php?modulo=principal/projespec/cadprojespec&acao=A&ver=1&id=7

  }  
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }  

</script>
