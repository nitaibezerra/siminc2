<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:geraproposta.inc
   Finalidade: permitir a construção de relatorio de propostas de alteração em ações
    */
    // primeira parte do relatório : dados básicos
    $_SESSION['rsdados'][] = $acacodid;
    
 	

if ($exclusao) {
	$_SESSION['prnexc']=1;
?>
<br>
<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>Propostas de Exclusão da Ação <? print $prgcod.' - '.$acacod.' . '.$acadsc?></b></td></tr></table>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
<tr>
    <td><b>Proponente</td>
    <td><b>Justificativa</td>
    <td><b>Status<br>Decisão</td>
    <td><b>Decisor</td>
</tr>
</thead><tbody>
<?
$proponente=0;
$peastatus=0;
$peastatusspo=0;
$usunome=0;
$usunomeum=0;
$usunomedois=0;
$nlinhas=0;
    if ($acacodid)
    {
	$sql= "select u.usunome as proponente, p.peastatus as statusUm, p.peastatusspo as statusDois, p.peajustificativa as justificativa, u2.usunome as usunomeum, u3.usunome as usunomedois from elabrev.proposta_exclusao_acao p inner join seguranca.usuario u on u.usucpf=p.usucpf left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.peastatusspo in ('$decspo') and  p.peastatus in ('$decunid') and p.acaid=$acacodid order by p.peastatusspo desc, p.peastatus desc, u.usunome";

     $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
    }
     if ($nlinhas >= 0)
     {
        for ($ie=0;$ie<=$nlinhas;$ie++)
        {
            $resexc = $db->carrega_registro($RS,$ie);
            if(is_array($resexc)) foreach($resexc as $k=>$v) ${$k}=$v;
            
           if (fmod($ie,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
           print "<tr bgcolor='$marcado' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$marcado';\"><td>$proponente</td><td>$justificativa</td><td>";
            if ($statusdois=='Z')
            {
               print 'MEC</td><td>';
               if ($usunomedois) print $usunomedois;
            }
           else if ($statusdois=='S')
           {
               print 'SPO</td><td>';
               if ($usunomedois) print $usunomedois;
           }
            else if ($statusum=='D') 
            {
            	print 'Unidade</td><td>';
            	if ($usunomeum) print $usunomeum;
            }
            else 
            {
            	print 'Sem decisão</td><td>';
            }
            print "</td></tr>";
           
        }
     }
     else 
     print  "<tr align='center'><td colspan=4>Não há propostas de exclusão nesta ação</td></tr>";
}
if ($migracao)
{  	
	$_SESSION['prnmig']=1;
?>
</tbody></table>

<br>
<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>Propostas de Migração da Ação <? print $prgcod.' - '.$acacod.' . '.$acadsc?></b></td></tr></table>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
<tr>
    <td><b>Proponente</td>
    <td><b>Migra para</td>    
    <td><b>Justificativa</td>
    <td><b>Status<br>Decisão</td>
    <td><b>Decisor</td>
</tr>
</thead><tbody>
<?
$proponente=0;
$pmastatus=0;
$pmastatusspo=0;
$usunome=0;
$usunomeum=0;
$usunomedois=0;
$nlinhas=0;
    if ($acacodid)
    {
	$sql= "select u.usunome as proponente, p.pmastatus as statusUm, p.pmastatusspo as statusDois, p.pmajustificativa as justificativa, u2.usunome as usunomeum, u3.usunome as usunomedois, prg.prgcod||'-'||prg.prgdsc as destino from elabrev.proposta_migracao_acao p inner join elabrev.ppaprograma_proposta prg on prg.prgid=p.prgid inner join seguranca.usuario u on u.usucpf=p.usucpf left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.acaid=$acacodid order by p.pmastatusspo desc, p.pmastatus desc, u.usunome";
     $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
    }
     if ($nlinhas >= 0)
     {
        for ($ie=0;$ie<=$nlinhas;$ie++)
        {
            $resexc = $db->carrega_registro($RS,$ie);
            if(is_array($resexc)) foreach($resexc as $k=>$v) ${$k}=$v;
           if (fmod($ie,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
           print "<tr bgcolor='$marcado' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$marcado';\"><td>$proponente</td><td>$destino</td><td>$justificativa</td><td>";
            if ($statusdois=='Z')
            {
               print 'MEC</td><td>';
               if ($usunomedois) print $usunomedois;
            }
           else if ($statusdois=='S')
           {
               print 'SPO</td><td>';
               if ($usunomedois) print $usunomedois;
           }
            else if ($statusum=='D') 
            {
            	print 'Unidade</td><td>';
            	if ($usunomeum) print $usunomeum;
            }
            else 
            {
            	print 'Sem decisão</td><td>';
            }
            print "</td></tr>";
           
        }
     }
     else 
     print  "<tr align='center'><td colspan=4>Não há propostas de migração nesta ação</td></tr>";
}
if ($fusao)
{  	
	$_SESSION['prnfus']=1;
?>
</tbody></table>  

  <br>
<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>Propostas de Fusão na Ação <? print $prgcod.' - '.$acacod.' . '.$acadsc?></b></td></tr></table>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
<tr>
    <td><b>Proponente</td>
    <td><b>Funde com</td>    
    <td><b>Justificativa</td>
    <td><b>Status<br>Decisão</td>
    <td><b>Decisor</td>
</tr>
</thead><tbody>
<?
$proponente=0;
$pfastatus=0;
$pfastatusspo=0;
$usunome=0;
$usunomeum=0;
$usunomedois=0;
$nlinhas=0;
    if ($acacodid)
    {
	$sql= "select u.usunome as proponente, p.pfastatus as statusUm, p.pfastatusspo as statusDois, p.pfajustificativa as justificativa, u2.usunome as usunomeum, u3.usunome as usunomedois, a.prgcod||'-'||a.acacod||' . '||a.acadsc as destino from elabrev.proposta_fusao_acao p inner join elabrev.ppaacao_proposta a on a.acaid=p.acaid_externa inner join seguranca.usuario u on u.usucpf=p.usucpf left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.acaid=$acacodid order by p.pfastatusspo desc, p.pfastatus desc, u.usunome";
     $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
    }
     if ($nlinhas >= 0)
     {
        for ($ie=0;$ie<=$nlinhas;$ie++)
        {
            $resexc = $db->carrega_registro($RS,$ie);
            if(is_array($resexc)) foreach($resexc as $k=>$v) ${$k}=$v;
           if (fmod($ie,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
           print "<tr bgcolor='$marcado' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$marcado';\"><td>$proponente</td><td>$destino</td><td>$justificativa</td><td>";
            if ($statusdois=='Z')
            {
               print 'MEC</td><td>';
               if ($usunomedois) print $usunomedois;
            }
           else if ($statusdois=='S')
           {
               print 'SPO</td><td>';
               if ($usunomedois) print $usunomedois;
           }
            else if ($statusum=='D') 
            {
            	print 'Unidade</td><td>';
            	if ($usunomeum) print $usunomeum;
            }
            else 
            {
            	print 'Sem decisão</td><td>';
            }
            print "</td></tr>";
           
        }
     }
     else 
     print  "<tr align='center'><td colspan=4>Não há propostas de migração nesta ação</td></tr>";
}
if ($alteracao)
{
   $_SESSION['prnalt']=1;
?>
</tbody></table>

  <br>
<table  width="95%" class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>Propostas de Alteração de Atributos na Ação:  <? print $prgcod.' - '.$acacod.' . '.$acadsc?></b></td></tr></table>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
<tr>
    <td><b>Proponente</td>
    <td><b>Atributo</td>    
    <td><b>Proposta</td>     
    <td><b>Justificativa</td>
    <td><b>Status<br>Decisão</td>
    <td><b>Decisor</td>
</tr>
</thead><tbody>
<?
$proponente=0;
$erastatus=0;
$erastatusspo=0;
$usunome=0;
$usunomeum=0;
$usunomedois=0;
$nlinhas=0;
    if ($acacodid)
    {
	$sql= "select p.eracampo as atributo,p.eraproposta as proposta, u.usunome as proponente, p.erastatus as statusUm, p.erastatusspo as statusDois, p.erajustificativa as justificativa, u2.usunome as usunomeum, u3.usunome as usunomedois from elabrev.elaboracaorevisao p inner join seguranca.usuario u on u.usucpf=p.usucpf left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.eracampo <>'acaobs' and p.eracod=$acacodid and p.eratabela='ppaacao_proposta' order by p.eracampo,p.erastatusspo desc, p.erastatus desc, u.usunome";
 
     $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
    }
     if ($nlinhas >= 0)
     {
        for ($ie=0;$ie<=$nlinhas;$ie++)
        {
            $resexc = $db->carrega_registro($RS,$ie);
            if(is_array($resexc)) foreach($resexc as $k=>$v) ${$k}=$v;
           if (fmod($ie,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
           print "<tr bgcolor='$marcado' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$marcado';\"><td>$proponente</td><td>";
            print tradutor_att($atributo);
            print "</td><td>$proposta</td><td>$justificativa</td><td>";
            if ($statusdois=='Z')
            {
               print 'MEC</td><td>';
               if ($usunomedois) print $usunomedois;
            }
           else if ($statusdois=='S')
           {
               print 'SPO</td><td>';
               if ($usunomedois) print $usunomedois;
           }
            else if ($statusum=='D') 
            {
            	print 'Unidade</td><td>';
            	if ($usunomeum) print $usunomeum;
            }
            else 
            {
            	print 'Sem decisão</td><td>';
            }
            print "</td></tr>";
           
        }
     }
     else 
     print  "<tr align='center'><td colspan=4>Não há propostas de migração nesta ação</td></tr>";
}  
?>
</tbody></table>
