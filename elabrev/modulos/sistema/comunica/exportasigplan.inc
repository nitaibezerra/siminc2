<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:exportasigplan.inc
   Finalidade: permitir exporta os dados do SIGPLAN
   */
$modulo=$_REQUEST['modulo'] ;//

include "includes/cabecalho.inc";
if ($_REQUEST['envia']=='enviar')
{
  unset ($erro);
if ($_REQUEST['cbaca'] or $_REQUEST['cbtd'])
{
// pegar ação por ação

  
   $sql = "select acaid from acao where prgano= '".$_SESSION['exercicio']."'";
   $RSa = $db->record_set($sql);
   $nlinhasa = $db->conta_linhas($RSa);
   if ($nlinhasa >= 0)
   {
      for ($ia=0; $ia<=$nlinhasa;$ia++)
      {
        $res = $db->carrega_registro($RSa,$ia);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // para cada ação devo verificar a avaliação mês a mês, iniciando do mês 1.
        unset($texto);
        $texto='';
        unset($textoatuav);
        $textoatuav='';
        unset($textoatuacf);
        $textoatuacf='';

        $sql = "select refdsc, refmes_ref::integer, refcod from referencia where refano_ref = '".$_SESSION['exercicio']."' order by refmes_ref";
        $RSr = $db->record_set($sql);
        $nlinhasr = $db->conta_linhas($RSr);
        if ($nlinhasr >= 0)
        {
           for ($ir=0; $ir<=$nlinhasr;$ir++)
             {
                $res = $db->carrega_registro($RSr,$ir);
               // a linha abaixo transforma em variáveis todos os campos do array
                if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
                // com a referencia e a ação, verifico se existe avaliação
                $sql = "select av.avptexto,s.tpsdsc as sitavaliacao,av.avpdata,u.usunome,av.usucpf from avaliacaoparecer av, tiposituacao s, usuario u, acao a where av.avptexto <> '' and av.tpscod=s.tpscod and u.usucpf=av.usucpf and a.acaid = av.acaid and av.acaid>0 and a.prgano='".$_SESSION['exercicio']."' and avpliberada='t' and av.tpaid=1 and av.acaid=$acaid and av.refcod =".$refcod;
                $RSav = $db->record_set($sql);
                $nlinhasav = $db->conta_linhas($RSav);
                if ($nlinhasav >= 0)
                {
                   for ($iav=0; $iav<=$nlinhasav;$iav++)
                   {
                      $res = $db->carrega_registro($RSav,$iav);
                     // a linha abaixo transforma em variáveis todos os campos do array
                      if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
                      //print 'tem avaliação'.$acaid."ref = $refcod <br>";
                      //$texto.= $refdsc."\n ------------------ \n Avaliação \n Avaliador - CPF:".formatar_cpf($usucpf)." - $usunome \n Data: $avpdata \n Situação: $sitavaliacao \n ".ereg_replace("<[^>]*>","",$avptexto);
                      //$textoatuav = $refdsc."\n ------------------ \n Avaliação \n Avaliador - CPF:".formatar_cpf($usucpf)." - $usunome \n Data: $avpdata \n Situação: $sitavaliacao \n ".ereg_replace("<[^>]*>","",$avptexto);
                      //print 'tem avaliação'.$acaid."ref = $refcod <br>";
                     $texto.= $refdsc."\n Situação: $sitavaliacao \n ".ereg_replace("<[^>]*>","",$avptexto);
                     $textoatuav = $refdsc."\n Situação: $sitavaliacao \n ".ereg_replace("<[^>]*>","",$avptexto);

                   }
                }

               $sql = "select distinct e.exprealizado,e.expobs, e.expdata, u.usunome,e.usucpf,s.tpsdsc as expsituacao  from execucaopto e left join tiposituacao s on s.tpscod=e.tpscod left join usuario u on u.usucpf=e.usucpf where e.expobs<>'' and e.acaid=$acaid and e.refcod = $refcod";
                $RSe = $db->record_set($sql);
                $nlinhase = $db->conta_linhas($RSe);
                if ($nlinhase >= 0)
               {
                   for ($ie=0; $ie<=$nlinhase;$ie++)
                   {
                      $res = $db->carrega_registro($RSe,$ie);
                    //  a linha abaixo transforma em variáveis todos os campos do array
                      if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
                     //$texto.= $refdsc."\n ------------------ \n Avaliação da Execução Física \n Avaliador - CPF:".formatar_cpf($usucpf)." - $usunome \n Data: $expdata \n Situação: $expsituacao \n ".ereg_replace("<[^>]*>","",$expobs);
                    // $textoatuacf = $refdsc."\n ------------------ \n Avaliação da Execução Física \n Avaliador - CPF:".formatar_cpf($usucpf)." - $usunome \n Data: $expdata \n Situação: $expsituacao \n ".ereg_replace("<[^>]*>","",$expobs);
                     
                    $texto.= $refdsc."\n Situação: $expsituacao \n ".ereg_replace("<[^>]*>","",$expobs);
                     $textoatuacf = $refdsc."\n Situação: $expsituacao \n ".ereg_replace("<[^>]*>","",$expobs);
                  }
                }
             } // fim da referencia
        }
          $execucaocmt = $texto;
          $atualdsc = $textoatuav.'\n'.$textoatuacf;
          $atualdsc = trim(str_replace("'","''",$atualdsc));
           // altera o banco
          $sql = "update acao set  ACADscComentExecucao='".trim(str_replace("'","''",$execucaocmt))."',ACADscSituacaoAtual='$atualdsc' where acaid=$acaid";
          $saida = $db->executar($sql);
       } // fim da ação
      $db -> commit();
     
   }
     $sql = 'select PRGAno as "PRGAno", PRGCod as "PRGCod", ACACod as "ACACod", SACCod as "SACCod",LOCCod as "LOCCod",PROCod as "PROCod",UNICod as "UNICod",SITCodEstagio as "SITCodEstagio",SITCodAndamento as "SITCodAndamento",SITCodCronograma as "SITCodCronograma",    ACAPercExecucao as "ACAPercExecucao",ACADscComentarios as "ACADscComentarios",    ACASNMetaNaoCumulativa as "ACASNMetaNaoCumulativa",ACADscComentSituacao as "ACADscComentSituacao",ACADscSituacaoAtual as "ACADscSituacaoAtual",    ACADscResultadosEspObt as "ACADscResultadosEspObt",ACAMesPrevisaoConclusao as "ACAMesPrevisaoConclusao",ACAAnoPrevisaoConclusao as "ACAAnoPrevisaoConclusao", ACADscComentExecucao as "ACADscComentExecucao",    ACADscComentExecucaoRap as "ACADscComentExecucaoRap" from acao where prgano='."'".$_SESSION['exercicio']."'";
    if (! $db->ConvertToXML(&$clientedb, $sql, "RetornaAcao.xml","Acao"))
    $erro = 'Ação\n';
    
   
}

if ($_REQUEST['cbresproaca'] or $_REQUEST['cbtd'])
{
    $sql = 'select a.PRGAno as "PRGAno", a.PRGCod as "PRGCod",a.ACACod as "ACACod",a.SACCod as "SACCod",r.TRSCod as "TRSCod",r.RSANum as "RSANum", case when r.TPICod is null then 2 end as "TPICod",case when r.CMPCod is null then 0 end as "CMPCod", to_char(r.RSADataInclusao,\'YYYY-MM-DDT\') || to_char(r.RSADataInclusao,\'HH24:MI:SS\') as "RSADataInclusao", r.RSADscRestricao as "RSADscRestricao", r.RSADscProvidencia as "RSADscProvidencia", to_char(r.RSADataSuperacao,\'YYYY-MM-DDT\') || to_char(r.RSADataSuperacao,\'HH24:MI:SS\') as "RSADataSuperacao", r.RSASNRelatSituacao as "RSASNRelatSituacao",r.RSASNTempoHabil as "RSASNTempoHabil",r.RSAObsMonitor as "RSAObsMonitor",r.RSAObsControle  as "RSAObsControle" from restricaoacao r inner join acao a on a.acaid=r.acaid and a.prgano='."'".$_SESSION['exercicio']."' where r.rsasnliberado='t' order by a.acacod,a.unicod,a.loccod,a.saccod";
    if (! $db->ConvertToXML(&$clientedb, $sql, "RetornaRestricaoAcao.xml","RestricaoAcao"))
    $erro .='Restrição e Providência de Ação\n';
}


if ($_REQUEST['cbvalidacao'] or $_REQUEST['cbtd'])
{
    $sql = 'select a.PRGAno as "PRGAno", a.PRGCod as "PRGCod",a.ACACod as "ACACod",a.SACCod as "SACCod", substr(r.refdsc,1,1) as "Trimestre", substr(c.corsignificadopar,1,1) as "SituacaoValidacao", case when a.prgano <>'."''".' then '." ''". ' end as "ObservacoesGerente", avp.avptexto as "JustificativaDiscordanciaGerente" from acao a 
    inner join avaliacaoparecer avp on a.acaid=avp.acaid and avp.avpliberada = '."'t'".
    ' and avp.tpaid=2 inner join cor c on c.corcod=avp.corcod inner join referencia r on r.refcod=avp.refcod where a.prgano='."'".$_SESSION['exercicio']."' order by a.prgano,a.prgcod,a.acacod,a.unicod,a.loccod,a.saccod";

    if (! $db->ConvertToXML(&$clientedb, $sql, "RetornaValidacaoAcao.xml","ValidacaoTrimestral"))
    $erro .='Validação de Ação\n';
}

if ($_REQUEST['cbresproprg'] or $_REQUEST['cbtd'])
{
    $sql = 'select p.PRGAno as "PRGAno", p.PRGCod as "PRGCod",r.RSPNum as "RSPNum", r.TRSCod as "TRSCod",case when r.TPICod is null then 2 end as "TPICod",case when r.CMPCod is null then 0 end as "CMPCod", to_char(r.RSPDataInclusao,\'YYYY-MM-DDT\') || to_char(r.RSPDataInclusao,\'HH24:MI:SS\') as "RSPDataInclusao", r.RSPDscRestricao as "RSPDscRestricao", r.RSPDscProvidencia as "RSPDscProvidencia", to_char(r.RSPDataSuperacao,\'YYYY-MM-DDT\') || to_char(r.RSPDataSuperacao,\'HH24:MI:SS\') as "RSPDataSuperacao", r.RSPSNRelatSituacao as "RSPSNRelatSituacao",r.RSPSNTempoHabil as "RSPSNTempoHabil",r.RSPObsMonitor as "RSPObsMonitor",r.RSPObsControle as "RSPObsControle"  from restricaoprograma r inner join programa p on p.prgid=r.prgid and p.prgano='."'".$_SESSION['exercicio']."' where r.rspsnliberado='t' order by p.prgano,p.prgcod";
    if (! $db->ConvertToXML(&$clientedb, $sql, "RetornaRestricaoPrograma.xml","RestricaoPrograma"))     $erro .='Restrição e Providência de Programa\n';

}

if ($_REQUEST['cbdadfis'] or $_REQUEST['cbtd'])
{
  // esta função deve ser disparada após a atualização da tabela dados fisicos
  // o primeiro passo é selecionar os campos que interessam em dados fisicos
  $sql = "select acaid from dadofisico";
  $RSd = $db->record_set($sql);
  $nlinhasd = $db->conta_linhas($RSd);
  for ($id=0;$id<=$nlinhasd;$id++)
  {
   $res = $db->carrega_registro($RSd,$id);
   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

  // aqui se seleciona o acaid para buscar na tabela execucaopto
    // com o acaid faço a busca em execucaopto
    for ($j=1;$j<13;$j++)
    {
        $ok=0;
        $sql3 = "select e.expid,e.exprealizado from execucaopto e, referencia r where e.refcod=r.refcod and e.acaid =".$acaid ."  and r.refmes_ref::integer =$j";
        $RS3 = $db->record_set($sql3);
        $nlinhas3 = $db->conta_linhas($RS3);
        if ($nlinhas3 >=0)
        { $ok=1;
          $res = $db->carrega_registro($RS3,0);
          if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
          ${'realizado'.$j}=$exprealizado;
        } else ${'realizado'.$j}='null';
        
    }
    if ($ok)
    {
      $sql = "update dadofisico set FISQtdeRealizado1=$realizado1,FISQtdeRealizado2 =$realizado2,  FISQtdeRealizado3 =$realizado3 ,  FISQtdeRealizado4=$realizado4  ,  FISQtdeRealizado5=$realizado5  ,  FISQtdeRealizado6=$realizado6 ,  FISQtdeRealizado7=$realizado7  ,  FISQtdeRealizado8=$realizado8  ,  FISQtdeRealizado9=$realizado9 ,  FISQtdeRealizado10 =$realizado10 ,  FISQtdeRealizado11=$realizado11  ,  FISQtdeRealizado12=$realizado12 where acaid=$acaid";
     $db->executar($sql);
     $db->commit();
    }
  }

    $sql = 'select PRGAno as "PRGAno" ,PRGCod as "PRGCod",ACACod as "ACACod",SACCod as "SACCod",REGCod as "REGCod",FISQtdeCronInicial1 as "FISQtdeCronInicial1",FISQtdeCronInicial2 as "FISQtdeCronInicial2",FISQtdeCronInicial3 as "FISQtdeCronInicial3",FISQtdeCronInicial4 as "FISQtdeCronInicial4",FISQtdeCronInicial5 as "FISQtdeCronInicial5",FISQtdeCronInicial6 as "FISQtdeCronInicial6",FISQtdeCronInicial7 as "FISQtdeCronInicial7",FISQtdeCronInicial8 as "FISQtdeCronInicial8",FISQtdeCronInicial9 as "FISQtdeCronInicial9",FISQtdeCronInicial10 as "FISQtdeCronInicial10",FISQtdeCronInicial11 as "FISQtdeCronInicial11",FISQtdeCronInicial12 as "FISQtdeCronInicial12",FISQtdeCronogramado1 as "FISQtdeCronogramado1", FISQtdeCronogramado2 as "FISQtdeCronogramado2",FISQtdeCronogramado3 as "FISQtdeCronogramado3",FISQtdeCronogramado4 as "FISQtdeCronogramado4",FISQtdeCronogramado5 as "FISQtdeCronogramado5",FISQtdeCronogramado6 as "FISQtdeCronogramado6",FISQtdeCronogramado7 as "FISQtdeCronogramado7",FISQtdeCronogramado8 as "FISQtdeCronogramado8",FISQtdeCronogramado9 as "FISQtdeCronogramado9",FISQtdeCronogramado10 as "FISQtdeCronogramado10",FISQtdeCronogramado11 as "FISQtdeCronogramado11",FISQtdeCronogramado12 as "FISQtdeCronogramado12",FISQtdeRealizado1 as "FISQtdeRealizado1",FISQtdeRealizado2 as "FISQtdeRealizado2",  FISQtdeRealizado3  as "FISQtdeRealizado3",  FISQtdeRealizado4  as "FISQtdeRealizado4",  FISQtdeRealizado5  as "FISQtdeRealizado5",  FISQtdeRealizado6  as "FISQtdeRealizado6",  FISQtdeRealizado7  as "FISQtdeRealizado7",  FISQtdeRealizado8  as "FISQtdeRealizado8",  FISQtdeRealizado9  as "FISQtdeRealizado9",  FISQtdeRealizado10  as "FISQtdeRealizado10",  FISQtdeRealizado11  as "FISQtdeRealizado11",  FISQtdeRealizado12  as "FISQtdeRealizado12",FISQtdeprevistoano as "FISQtdePrevistoAno",FISQtdeCronInicialano as "FISQtdeCronInicialAno",FISQtdeatualano as "FISQtdeAtualAno",FISQtdeCronogramadoAno as "FISQtdeCronogramadoAno",FISQtdeRealizadoAno as "FISQtdeRealizadoAno",FISDscComentExecucao as "FISDscComentExecucao" from dadofisico';

   if (! $db->ConvertToXML(&$clientedb, $sql, "RetornaDadoFisico.xml","DadoFisico")) $erro .='Dados Físicos\n';;
}
if ($_REQUEST['cbprg'] or $_REQUEST['cbtd'])
{
   $sql = 'select PRGAno as "PRGAno", PRGCod as "PRGCod", PRGUrl as "PRGUrl",PRGDscContexto as "PRGDscContexto",to_char(PRGDataAtuContexto,\'YYYY-MM-DDT\') || to_char(PRGDataAtuContexto,\'HH24:MI:SS\') as "PRGDataAtuContexto",SITCodEstagio as "SITCodEstagio", SITCodAndamento as "SITCodAndamento",SITCodCronograma as "SITCodCronograma", PRGPercExecucao as "PRGPercExecucao", PRGDscComentSituacao as "PRGDscComentSituacao", to_char(PRGDataAtuSituacao,\'YYYY-MM-DDT\') || to_char(PRGDataAtuSituacao,\'HH24:MI:SS\') as "PRGDataAtuSituacao", PRGDscSituacaoAtual as "PRGDscSituacaoAtual", PRGDscResultadosEspObt as "PRGDscResultadosEspObt", to_char(PRGDataAtuSituacaoAtual,\'YYYY-MM-DDT\') || to_char(PRGDataAtuSituacaoAtual,\'HH24:MI:SS\') as "PRGDataAtuSituacaoAtual"  from programa p where prgano ='."'".$_SESSION['exercicio']."'";
  if (! $db->ConvertToXML(&$clientedb, $sql, "RetornaPrograma.xml","Programa")) $erro .=' Programa\n';
}
    if (! $erro){?>
            <script>
              alert('Operação realizada com sucesso!\n Os dados foram disponibilizados em simec/xml/exportacao');
             </script><?
  		    
      } else
{?>
            <script>
              alert('Operação realizada com problemas!\n Os arquivos abaixo não foram gerados corretamente\n <?=$erro?>');
             </script><?

      }
}
?>
<br>
<div class="notprint">
<?

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Exportação de Dados para o SIGPLAN';
monta_titulo($titulo_modulo,'');
?>
<html>
<head>

<link rel="stylesheet" type="text/css" href="includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='includes/listagem.css'>
<script language="JavaScript" src="includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="envia" >
    <center>
   <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
   <tr><td><font face="Verdana" size="3"></font>O envio de dados para o SIGPLAN deverá ocorrer quantas vezes o Administrador julgar conveniente.<p>Ao pressionar o botão "Enviar dados", o sistema irá gerar um conjunto de arquivos no formato XML e disponibilizá-los para o SIGPLAN.</p>
</td>
	 </tr>
    <tr><td><input type='checkbox' name ='cbtd'> Todos&nbsp;&nbsp;<input type='checkbox' name ='cbprg'> Programas&nbsp;&nbsp;<input type='checkbox' name ='cbaca'> Ações&nbsp;&nbsp;<input type='checkbox' name ='cbdadfis'> Dados Físicos&nbsp;&nbsp;<input type='checkbox' name ='cbdadfin'> Dados Financeiros<br><input type='checkbox' name ='cbresproprg'> Restrições e Providências de Programa&nbsp;&nbsp;<input type='checkbox' name ='cbresproaca'> Restrições e Providências de Ação&nbsp;&nbsp;<input type='checkbox' name ='cbvalidacao'> Validação de Ação
</td>
	 </tr>
	 <td ><input type='button' class="botao" value='Enviar dados' onclick="envia_dados()"></td>
	 </tr>
  </table>
</form> 
<script>
function envia_dados()
{
  document.formulario.envia.value='enviar';
  document.formulario.submit();
}

</script>
</body>
</html>
