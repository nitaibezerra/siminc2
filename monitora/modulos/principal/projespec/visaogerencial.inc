<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   módulo:visaogerencial.inc
   finalidade: permitir a visão de alta-gestão do projeto
   */
$modulo=$_REQUEST['modulo'];
print '<br>';
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Visão Gerencial';
monta_titulo($titulo_modulo,'');
$sql= "select to_char(p.pjedataini,'dd/mm/yyyy') as dataini, to_char(p.pjedatafim,'dd/mm/yyyy') as datafim, p.* , m.unmdsc, pr.prodsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod  where p.pjeid=".$pjeid;
$res =  $db->pegalinha($sql);

// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

?>
<center>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" >
  <tr bgcolor="#E9E9E9">
    <td valign="top" width="35%">
	  <table  width="100%" class="" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" >
        <tr >
          <td colspan="2" align='center' ><b>Dados do Projeto</b></td>
        </tr>	
        <tr>
          <td align='left' width='40%' >Denominação:</td>
          <td><?=$pjecod.'.'.$pjedsc?></td>
      </tr>    
      <?
          $sql = "select distinct pfl.pflcod,pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra,usu.usucpf, usu.usuemail, usu.usunome || ' ('|| ee.entnome ||')' as usuario,usu.usufoneddd||'-'||usu.usufonenum as fone from seguranca.perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = $pjeid and rpu.rpustatus='A' left join seguranca.usuario usu on usu.usucpf=rpu.usucpf left join entidade.entidade ee on ee.entid = usu.entid where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E') order by pfl.pflcod";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if ($usuario ){
	        $linha = "<tr><td align='left' >".$descricao.":</td><td>".$usuario.' Telefone:'.$fone;
	        if ($usucpf <> $_SESSION['usucpf'])
	        {
	        	$linha .= '&nbsp;&nbsp;&nbsp;<img src="../imagens/email.gif" title="Envia e-mail" border="0" onclick="envia_email(\''.$usucpf.'\');"> ';
	        }
	        $linha .= "</td></tr>";
            print $linha;
            }
        }
     }
     ?>
     <tr>
   </table>
 </td>

 <td width="35%">
   <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
     <tr>
       <td colspan="2" align="center">
        <b>Acompanhamento Físico e Financeiro</b></td>
     </tr>
     <?
         $sql = "select avgrealizado from monitora.avaliacaogenerico where pjeid=".$pjeid." order by avgdata desc limit 1";
         $realizado = $db->pegaUm($sql);
		 $percent='0';
		 if ($pjeprevistoano) $percent = number_format(($realizado/$pjeprevistoano)*100,0,',','.');
		 $sql = "select c.corsignificado as desc , trim(c.corimgav) as imagem, u.usunome,avgtexto,s.tpsdsc,s.tpscor from monitora.avaliacaogenerico ap inner join cor c on c.corcod=ap.corcod inner join seguranca.usuario u on u.usucpf=ap.usucpf inner join tiposituacao s on s.tpscod=ap.tpscod where avgliberada='t' and avgstatus='A' and pjeid = $pjeid order by avgdata desc limit 1";
    	$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
		if ($tpsdsc) {
			$situacao = "<img src='../imagens/$imagem' title='$desc' >  <font color='$tpscor'>$tpsdsc</font>";	 
		}
		else $situacao='Sem avaliação!';
		if (! $avgtexto) $avgtexto='Sem comentários!';
     ?>
     <tr>
        <td align='left' width='50%' >Período:</td>
        <td align='right'><?=$dataini .' a '.$datafim?></b></td>
     </tr>      
     <tr>
        <td align='left' width='50%' >Produto:</td>
        <td align='right'><?=$prodsc?></b></td>
     </tr> 
   	 <tr>
        <td align='left' width='50%' >Unidade de Medida:</td>
        <td align='right'><?=$unmdsc?></b></td>
     </tr>
   	 <tr>
        <td align='left' width='50%' >Meta:</td>
        <td align='right'><?=number_format($pjeprevistoano,0,',','.');?></b></td>
     </tr>     
     <tr>
        <td align='left' width='50%'>Realizado até o momento / (Percentual):</td>
        <td align='right'><?=$realizado.' / ('.$percent.'%)'?></b></td>
     </tr> 
     <?      
	  $sql="	select p.pjevlrano as vlrano,sum(e.expfinanceiro) as gasto 
          from monitora.projetoespecial p
           left join monitora.execucaopto e on p.pjeid=e.pjeid
         where p.pjeid=$pjeid group by p.pjevlrano";
       $res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
       $financeiro="Total do projeto: R$ ".number_format($vlrano,2,',','.').
                   "<br>Gasto: R$ ".number_format($gasto,2,',','.').
                   "<br>Saldo: R$ ".number_format($vlrano-$gasto,2,',','.');             
     ?>
    <tr>
       <td align='left' width='50%'>Financeiro:
       </td><td align='right'> <?=$financeiro?></td>
    </tr>
    <tr>
       <td align='left' width='50%'>Situação atual:
       </td><td align='right'><?=$situacao?></td>
    </tr>   
         <tr>
       <td align='left' width='50%'>Comentário:
       </td><td align='right'> <?=$avgtexto?></td>
    </tr> 
  </table>
  </td>

  <td valign="top" width="28%">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
      <tr>
        <td valign="top" width="100%">
        <!--<p align="center"><b>Valor Agregado</b>--></td>
      </tr>
<?
// cálculo do valor agregado
// tempo total do projeto = dtfim - dtini
// tempo decorrido = dataatual-dtini
// total previsto
$ev=$gasto;
 $sql="	select sum(dp.dpevalor) as pv 
          from monitora.desembolso_projeto dp
          inner join monitora.planotrabalho p on p.ptoid=dp.ptoid and p.pjeid=$pjeid
         where dp.dpestatus='A' and dp.dpedata <= now()";

        $res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 

?>
      
      
      
    </table>
    </td>
  </tr>

</table>

