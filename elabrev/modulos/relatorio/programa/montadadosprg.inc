<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:montadadosprg.inc
   Finalidade: permitir a construção de relatorio gerencial de programas em dados gerais
    */
    $RS = $db->record_set($sqlrel);
    $_SESSION['LINHA'] = $db->conta_linhas($RS);
    
    $_SESSION['title'] = $tit;
    $_SESSION['TITULO']= 'Relatório Gerencial de Programa';
    $_SESSION['TIT'][0][0] = 'Código ';
    $_SESSION['TIT'][0][1] = 'Título';
    $_SESSION['data']= date('(h:m) d/m/Y');
    $_SESSION['LIN'] = array();
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    if ($_SESSION['LINHA'] >=0) print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel(2)">&nbsp;&nbsp;<a href="ar505ptb.exe"><img border="0" width="30" heigth="30" src="imagens/pdf_adobe.jpg" title=" Você não tem o Adobe Reader? Clique aqui!"></a>';
    print '</td></tr></table>';
if ($_SESSION['LINHA'] >= 0)
{
     unset($_SESSION['rsdados']);
     unset($_SESSION['rsdadosresp']);
     unset($_SESSION['rsfinevo']);
     unset($_SESSION['rsavapar']);
     unset($_SESSION['rsrespro']);
     unset($_SESSION['rsparalt']);
     
   for ($i=0; $i<=$_SESSION['LINHA'];$i++)
   {
     $res = $db->carrega_registro($RS,$i);
     // a linha abaixo transforma em variáveis todos os campos do array
     if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     $programa=$prgid;
     $coprg = $codigo;
     $prgdsc = $descricao;

 	 $_SESSION['LIN'][$i][0] = $codigo;
	 $_SESSION['LIN'][$i][1] = $descricao;
	 // primeira parte do relatório, dados básicos
     if ($_REQUEST['dadobas'])
     {
       include "modulos/relatorio/programa/dadosbasprg.inc";
     }
     if ($_REQUEST['finevo']){
           // segunda parte do relatório : Financeiro Evolução
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>2. FINANCEIRO EVOLUÇÃO -- '.$coprg.'-'.$prgdsc.'</b></td></tr></table>';
    $cabecalho = array('Título da Ação','Dotação Autorizada','Valor Empenhado','Valor Liquidado','Valor Pago');
    $sql = "Select '<font color=#006600><b>'||s.co_acao||'</b></font> - '||s.no_projeto_atividade as acao, sum(s.dotacao_disponivel) as disponivel, sum(s.empenhos_emitidos) as emitidos, sum(s.empenhos_liquidados) as liquidados, sum(s.valor_pago) as pago from  siof_simec s where co_programa='".$coprg."' and to_char(s.dt_referencia,'YYYY') ='".$_SESSION['exercicio']."'  group by s.co_acao, s.no_projeto_atividade order by 1";

    $sqlp = "select prgcod, prgdsc from programa where prgcod='".$coprg."' and prgano='".$_SESSION['exercicio']."'";
     $RS22 = $db->record_set($sqlp);
     $res22 = $db->carrega_registro($RS22,0);
     // a linha abaixo transforma em variáveis todos os campos do array
     if(is_array($res22)) foreach($res22 as $k=>$v) ${$k}=$v;
     $_SESSION['prgcod'] = $prgcod;
     $_SESSION['prgdsc'] = $prgdsc;

    $RSfin = $db->record_set($sql);
    $_SESSION['rsfinevo'][] = $db->carrega_tudo($RSfin);
    
$db->monta_lista($sql,$cabecalho,300,20,'S','','');
     }
     if ($_REQUEST['respro'])
     {
       // restrições e providências
        $sql= "select p.prgcod,p.prgdsc, r.rspdscrestricao as restricao, r.rspdscprovidencia as providencia,  to_char(r.rspdatainclusao,'DD/MM/YYYY') as datainc, u.usunome, o.orgdsc as orgao, t.trsdsc from restricaoprograma r, programa p, usuario u, orgao o, tiporestricao t where r.prgid =".$programa." and r.prgid=p.prgid  and r.rspsnliberado='t' and r.usucpf=u.usucpf and o.orgcod = u.orgcod and t.trscod=r.trscod";
        if ($_REQUEST['cbrp'] == 'N') $sql = $sql." and r.rspsntempohabil='f' ";
        if ($_REQUEST['cbrp'] == 'S') $sql = $sql." and r.rspsntempohabil='t' ";
        $RSr = $db->record_set($sql);
        $_SESSION['rsrespro'][] = $db->carrega_tudo($RSr);
        $nlinhas = $db->conta_linhas($RSr);
        if ($nlinhas >=0)
        {
           print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>3. RESTRIÇÕES E PROVIDÊNCIAS-- '.$coprg.'-'.$prgdsc.'</b></td></tr></table>';
           print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
           print "<tr><td width='50%' valign='top'> <font color='#FF5B3C'><b><h2>Restrições </h2></font></td><td width='50%' valign='top'><font color='#FF5B3C'><b><h2> Providências </h2></b></font></td></tr>";
           for ($irp=0; $irp<=$nlinhas;$irp++)
            {
                $res = $db->carrega_registro($RSr,$irp);
                if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
                print "<tr><td> ".$restricao." </td><td> ".$providencia." </td></tr>";
                print "<tr><td> Tipo de restrição ".$trsdsc." </td><td></td></tr>";
                print "<tr><td>Data de inclusão: ".$datainc." </td><td> Relator: ".$usunome." / $orgao </td></tr>";
                print "<tr><td colspan=2><hr> </td></tr>";
            }
        }
     }
     if ($_REQUEST['paralt'])
     {
       // parecer da alta gestão
  $sql= "select p.prgcod,p.prgdsc, a.avptexto as parecer, to_char(a.avpdata,'DD/MM/YYYY') as datainc, u.usunome, o.orgdsc as orgao from avaliacaoparecer a, usuario u, orgao o, programa p where a.prgid =".$programa." and a.prgid=p.prgid and a.avporigem=9 and a.usucpf=u.usucpf and o.orgcod = u.orgcod";
    $RSr = $db->record_set($sql);
    $_SESSION['rsparalt'][] = $db->carrega_tudo($RSr);
  $nlinhas = $db->conta_linhas($RSr);
  if ($nlinhas >=0)
  {
 print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>3. PARECER DA ALTA GESTÃO -- '.$coprg.'-'.$prgdsc.'</b></td></tr></table>';
  print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
      for ($irp=0; $irp<=$nlinhas;$irp++)
      {
        $res = $db->carrega_registro($RSr,$irp);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        print "<tr><td><b>Parecer: </b> ".$parecer." </td></tr>";
        print "<tr><td><b>Data de inclusão: </b>".$datainc."</td></tr>";
        print "<tr><td><b>Relator: </b>".$usunome." / $orgao </td></tr>";
        print "<tr><td ><hr> </td></tr>";
      }
  }
    print '</table>';
   }
   
     if ($_REQUEST['avapar'])
   {
    $sql = "select ref.refdsc, av.avptexto as avaliacao, av.avpliberada as avlib,u.usunome as avusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as avdata, ca.corimgav as corav, ca.corsignificado as avcordsc, tsa.tpsdsc as avtpsdsc, tsp.tpsdsc as partpsdsc , pa.avpliberada as parlib, pa.avptexto as parecer,u2.usunome as parusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as pardata,  cp.corimgpar as corpar , cp.corsignificado as pacordsc, o.orgdsc as orgav, o2.orgdsc as orgpar,tsa.tpsdsc as patpsdsc, p.prgcod, p.prgdsc from referencia ref left join avaliacaoparecer av on av.refcod=ref.refcod and av.tpaid=1 and av.prgid =".$programa."  left join cor ca on av.corcod = ca.corcod left join tiposituacao tsa on tsa.tpscod = av.tpscod  left join avaliacaoparecer pa on pa.refcod=ref.refcod and pa.tpaid=2 and pa.prgid = ".$programa."   left join usuario u2 on pa.usucpf = u2.usucpf left join usuario u on u.usucpf = av.usucpf left join orgao o on o.orgcod = u.orgcod left join orgao o2 on o2.orgcod=u2.orgcod left join cor cp on pa.corcod = cp.corcod  left join tiposituacao tsp on tsp.tpscod = pa.tpscod left join programa p on p.prgid=av.prgid where ref.refdata_limite_parecer_aca is not null and ref.refsnmonitoramento='t' and ref.refano_ref=".$_SESSION['exercicio'];

    if ($_REQUEST['dataini'] and $_REQUEST['datafim'])
    {
        $sql.= " and ref.refano_ref = '".substr($_REQUEST['dataini'],6,4)."' and ref.refmes_ref >= '".substr($_REQUEST['dataini'],3,2)."' and ref.refmes_ref <= '".substr($_REQUEST['datafim'],3,2)."' ";
    }
    $RSavp = $db->record_set($sql);
    $_SESSION['rsavapar'][] = $db->carrega_tudo($RSavp);
    $nlinhasM = $db->conta_linhas($RSavp);
  if ($nlinhasM >=0)
  {
      for ($iap=0; $iap<=$nlinhasM;$iap++)
      {
        $res = $db->carrega_registro($RSavp,$iap);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) {
print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>3. Avaliação e Parecer -- '.$coprg.'-'.$prgdsc.'</b></td></tr></table>';
  print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr>';
      if ($avaliacao<>'' and $avlib=='t')
      print "<td width='50%' valign='top'> <font color='#FF5B3C'><b><h2>Avaliação </h2></font></td>";
       if ($parecer <> '' and $parlib=='t') print "<td width='50%' valign='top'><font color='#FF5B3C'><b><h2> Parecer </h2></b></font></td>";
       if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr>';
       
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '<tr>';
        if ($avaliacao<>'' and $avlib=='t')
        print "<td valign='top'> ".$avaliacao." </td>";
        if ($parecer <> '' and $parlib=='t')
        print "<td valign='top'> ".$parecer." </td>";
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr><tr>';

        if ($avaliacao<>'' and $avlib=='t')
        print "<td><b>Emitida por:</b> ".$avusu." / $orgav </td>";
        if ($parecer <> '' and $parlib=='t')
        print "<td><b>Emitido por:</b> ".$parusu." / $orgpar </td>";
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr><tr>';

        if ($avaliacao<>'' and $avlib=='t')
        print "<td><b>Situação:</b> ".$avtpsdsc."&nbsp;&nbsp;<img border='0' src='imagens/$corav' title='$avcordsc'></td>";
        if ($parecer <> '' and $parlib=='t')
        print "<td><b>Situação:</b> ".$partpsdsc."&nbsp;&nbsp;<img border='0' src='imagens/$corpar' title='$pacordsc'></td>";
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr><tr>';
        
        if ($avaliacao<>'' and $avlib=='t')
        print "<td><b>Data de avaliação:</b> ".$avdata." </td>";
        if ($parecer <> '' and $parlib=='t')
        print "<td><b>Data do Parecer:</b> ".$pardata."</td>";
        if (($parecer <> '' or $avaliacao<>'') and ($avlib=='t' or $parlib=='t') ) print '</tr><tr><td colspan=2><hr> </td></tr>';
    }
      }
  print '</table>';
  }



   }
   }
}
?>
