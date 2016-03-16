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

$pjeid=$copvalorcampo;
$sql= "select ug.ungabrev,p.pjecod,p.pjedsc,to_char(p.pjedataini,'dd/mm/yyyy') as dataini, to_char(p.pjedatafim,'dd/mm/yyyy') as datafim, p.* , m.unmdsc, pr.prodsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod  inner join unidadegestora ug on ug.ungcod=p.ungcod where p.pjeid=".$pjeid;

$res =  $db->pegalinha($sql);

// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$visao = "<center>
<table valign='top' >
  <tr bgcolor='#E9E9E9'>
    <td valign='top' align='center' width='35%'><B>SIMEC - VISÃO GERENCIAL DE PROJETO</td>
  </tr>
</table>
    
<table valign='top' >
  <tr bgcolor='#E9E9E9'>
    <td valign='top' width='35%'>
	  <table   width='100%' bgcolor='#f5f5f5' cellSpacing='0' cellPadding='0' border='1' bordercolor='#000000' style='border-collapse: collapse' >
        <tr >
          <td colspan='2' align='center' ><b>Dados do Projeto</b></td>
        </tr>	
        <tr>
          <td align='left' width='40%' ><b>Denominação:</b></td>
          <td><i><font face='Verdana' size='2'>$ungabrev$pjecod-$pjedsc</font></i></td>
      </tr>    ";

   $sql = "select distinct pfl.pflcod,pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra,usu.usucpf as membro, usu.usuemail, usu.usunome || ' ('|| ee.entnome ||')' as usuario,usu.usufoneddd||'-'||usu.usufonenum as fone from seguranca.perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = ".$pjeid." and rpu.rpustatus='A' inner join seguranca.usuario usu on usu.usucpf=rpu.usucpf left join entidade.entidade ee on ee.entid = usu.entid where   pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E') order by pfl.pflcod";

 $rseq = @$db->carregar( $sql );
 if (  $rseq && count($rseq) > 0 )
	{
	 foreach ( $rseq as $linharesp )
		{
		 foreach($linharesp as $k=>$v) ${$k}=$v;
         if ($usuario ){
	        $linhar .= "<tr><td align='left' class='SubTituloDireita'><b>".$descricao.":</b></td><td><i><font face='Verdana' size='2'>".$usuario.' Telefone:'.$fone;
	        $linhar .= "</font></i></td></tr>";
		}
	}
}

    
$visao .= $linhar;

$visao .= "
   </table>
 </td>

 <td width='35%' valign='top'>
   <table valign='top' bordercolor='#000000' style='border-collapse: collapse' cellpadding='0' cellspacing='0' border='1'>
     <tr>
       <td colspan='2' align='center'>
        <b>Acompanhamento Físico e Financeiro</b></td>
     </tr>
";

         $sql = "select avgrealizado from monitora.avaliacaogenerico where pjeid=".$pjeid." order by avgdata desc limit 1";
         $realizado = $db->pegaUm($sql);
		 $percent='0';
		 if ($pjeprevistoano) $percent = number_format(($realizado/$pjeprevistoano)*100,0,',','.');
		 $sql = "select c.corsignificado as desc , trim(c.corimgav) as imagem, u.usunome,avgtexto,s.tpsdsc,s.tpscor from monitora.avaliacaogenerico ap inner join cor c on c.corcod=ap.corcod inner join seguranca.usuario u on u.usucpf=ap.usucpf inner join tiposituacao s on s.tpscod=ap.tpscod where avgliberada='t' and avgstatus='A' and pjeid = $pjeid order by avgdata desc limit 1";
		 //dbg($sql,1);
    	$respe=$db->pegalinha($sql);
		if(is_array($respe)) foreach($respe as $k=>$v) ${$k}=$v; 
		if ($tpsdsc) {
			$situacao = "$desc - $tpsdsc";	 
		}
		else $situacao='Sem avaliação!';
		if (! $avgtexto) $avgtexto='Sem comentários!';
$visao .= "
     <tr>
        <td align='left' width='50%' ><b>Período:</b></td>
        <td align='right'><i><font face='Verdana' size='2'>$dataini a $datafim</font></i></td>
     </tr>      
     <tr>
        <td align='left' width='50%' ><b>Produto:</b></td>
        <td align='right'><i><font face='Verdana' size='2'>$prodsc</font></i></td>
     </tr> 
   	 <tr>
        <td align='left' width='50%' ><b>Unidade de Medida:</b></td>
        <td align='right'><i><font face='Verdana' size='2'>$unmdsc</font></i></td>
     </tr>
   	 <tr>
        <td align='left' width='50%' ><b>Meta:</b></td>
        <td align='right'><i><font face='Verdana' size='2'>";
$visao .= number_format($pjeprevistoano,0,',','.');

$visao .= "</font></i></td>
     </tr>     
     <tr>
        <td align='left' width='50%'><b>Realizado até o momento / (Percentual):</b></td>
        <td align='right'><i><font face='Verdana' size='2'>$realizado / ($percent%)</font></i></td>
     </tr> ";

	  $sql="	select p.pjevlrano as vlrano,sum(e.expfinanceiro) as gasto 
          from monitora.projetoespecial p
           left join monitora.execucaoto e on p.pjeid=e.pjeid
         where p.pjeid=$pjeid group by p.pjevlrano";
       $res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
       $financeiro="Total do projeto: R$ ".number_format($vlrano,2,',','.').
                   "<br>Gasto: R$ ".number_format($gasto,2,',','.').
                   "<br>Saldo: R$ ".number_format($vlrano-$gasto,2,',','.'); 

$visao .= "
    <tr>
       <td align='left' width='50%'><b>Financeiro:</b>
       </td><td align='right'><i><font face='Verdana' size='2'>$financeiro</font></i></td>
    </tr>
    <tr>
       <td align='left' width='50%'><b>Situação atual:</b>
       </td><td align='right'><i><font face='Verdana' size='2'>$situacao</font></i></td>
    </tr>   
         <tr>
       <td align='left' width='50%'><b>Comentário:</b>
       </td><td align='right'><i><font face='Verdana' size='2'>$avgtexto</font></i></td>
    </tr> 
  </table>
  </td>


  </tr>

</table>
<DIV><SPAN><A href=\"http://simec.mec.gov.br/geral/ganttgraph.php?cp='".md5_encrypt($pjeid,'')."'\"><B>GRÁFICO DE GANTT</A></SPAN></DIV>";

