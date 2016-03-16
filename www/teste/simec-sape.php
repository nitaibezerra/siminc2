<?php
session_start();
define ('HOST','172.20.65.115');
define ('PORT','8201');
define ('TIMEOUT',300);
error_reporting(E_ALL);
$xml_exportacao = gerar_xml();
echo "<pre>";
print_r($xml_exportacao);
die('xml gerado');
$erro_conexao = false;
abrir_conexao($sk,$erro_conexao);
if($erro_conexao) {
  die($erro_conexao);
}
$codigo = $mensagem = false;
echo "conexao aberta, buscando resposta\n";
$resposta_completa = ler_status($sk,$codigo,$mensagem);

if($codigo !== 0)
{
  fclose($sk);
  die("Erro cod(".$codigo.") '".$mensagem."'" );
}
else
{
  echo "Conectado, enviando xml '$xml_exportacao' \n";
  enviar_xml($sk,$xml_exportacao);
  echo "xml enviado, buscando resposta \n";
  $codigo = $mensagem = false;
  ler_status($sk,$codigo,$mensagem);
  if($codigo !== 0)
  {
    fclose($sk);
    die("Erro cód(".$codigo.") '".$mensagem."'" );
    #TRATAR ERRO
  }
  else {
    echo "Envio com sucesso:'$mensagem'";
  }
}
fclose($sk);


#######################################################


function enviar_xml($sk,$xml) {
  fputs($sk, $xml);
}

function abrir_conexao(&$sk,&$erro) {
  echo "Abrindo conexao";
  $sk=fsockopen(HOST,PORT,$errnum,$errstr,TIMEOUT) ;
  if (!is_resource($sk)) {$erro_conexao = "Erro de Conexao:".$errnum." ".$errstr;}
}

function ler_status($sk,&$codigo,&$mensagem) {
  $lido = "";
  ///while (!feof($sk)) {
   $lido.= fgets ($sk, 1024);
 // }
  $codigo = (int)pegar_valor($lido,'codigo');
  $mensagem = pegar_valor($lido,'msg');
  echo "RESPOSTA COMPLETA:".$lido."\n";
  echo "COD($codigo)\n";
  echo "MSG:$mensagem\n";
}

function pegar_valor($xml,$tag) {
  $inicio = (strpos  ( $xml, "<$tag>" ) + strlen($tag) + 2) ;
  $tamanho = strpos  ( $xml, "</$tag>" ) - $inicio;
  return  substr($xml, $inicio , $tamanho);
}

########################################################################
########################################################################
########################################################################

/*
function gerar_xml(){
	require_once "config.inc";
  include APPRAIZ . "includes/classes_simec.inc";
  include APPRAIZ . "includes/funcoes.inc";
  $db = new cls_banco();
  $sql = sql();
  $recurso = $db->record_set( $sql );
  $total = $db->conta_linhas( $recurso ) + 1;
  $xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<pta xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"file:///c:/ProjetosFNDE/1-Sistemas/PTA/resources/pta.xsd\">";
  $xml .= montar_xml($db,$recurso,$total);
  $xml .= "</pta>";
  echo $xml;
  return $xml;
}

function montar_xml($db,$recurso,$total) {
  echo "montar_xml..........";
  
  $linhas = array();
  for ( $linha = 0; $linha < $total; $linha++ )
  {
    $dados = $db->carrega_registro( $recurso, $linha );
    $linhas[] = $dados;
    // $xml .= cada_linha($dados);
  }
  $_SESSION['carga'] = $linhas;
  echo "lidas ".count($linhas)." linhas.";
  print_r($_SESSION['carga']);
  die();
}
*/


function gerar_xml() {
  $indice =0;

  $ar_dados = $_SESSION['carga'];
  $indice++;
  $xml = "";
  gerarEntidade($ar_dados, $indice, $xml );
  echo $xml;
  die;
  var_dump($c[$a]);

  return $xml;
}
function gerarEntidade(&$ar_dados, &$indice, &$xml){
  $pi_old =0;
  $codigoDimensao = 0;
  $dados = $ar_dados[$indice];
  $xml .=  "<entidade>\n";
  $xml .=  "<codigoIbge>".$dados['codigoibge']."</codigoIbge>\n";
  $xml .=  "<nomeEntidade>".$dados['nomeentidade']."</nomeEntidade>\n";
  $xml .=  "<nomeMunicipio>".$dados['nomemunicipio']."</nomeMunicipio>\n";
  $xml .=  "<uf>".$dados['uf']."</uf>\n";
  $xml .=  "<nomeDirigente>".$dados['nomedirigente']."</nomeDirigente>\n";
  $xml .= "<projetos>";
  gerarProjeto($ar_dados, $indice, $xml, $pi_old, &$codigoDimensao);
  $xml .= "</projetos>";
  $xml .= "</entidade>";
}

function gerarProjeto(&$ar_dados, &$indice, &$xml, &$pi_old){
  $dados = $ar_dados[$indice];
  $pi = $dados['pi'];
  

 if ( $pi_old != $pi ){  
	$xml.="<projeto>";
  $xml.="<ano>".$dados['ano']."</ano>";
	$xml.="<pi>".pi."</pi>";
	 
  gerarAcao($xml);
  $xml.="</projeto>\n";  
  $indice++;
   gerarProjeto($ar_dados, $indice, $xml, $pi_old);
  
}else{
  gerarAcao($ar_dados, $indice, $xml, $pi_old);
}
$pi_old = $pi;
  
}
function gerarAcao(&$ar_dados, &$indice, &$xml, &$pi_old){
 $dados = $ar_dados[$indice];
  $xml.= "<acao></acao>";
}
function cada_linha($dados) {
    $loop_entidade = ($codigoibge_loop === false || $dados['codigoibge'] !== $codigoibge_loop);
    $fecha_loop_entidade = ($codigoibge_loop !== false && $dados['codigoibge'] !== $codigoibge_loop);

    $loop_pi = ($pi_loop === false || $dados['pi'] !== $pi_loop);
    $fecha_loop_pi = ($pi_loop !== false && $dados['pi'] !== $pi_loop);
    
    if($fecha_loop_pi)
    {
      $xml .= "</acoes>";
      $xml .= "</projeto>";
    }
    if($fecha_loop_entidade)
    {
      $xml .=  "</projetos>";
      $xml .=  "</entidade>";
    }

    if($loop_entidade) 
    {
      $xml .=  "<entidade>\n";
	  	$xml .=  "<codigoIbge>".$dados['codigoibge']."</codigoIbge>\n";
		  $xml .=  "<nomeEntidade>".$dados['nomeentidade']."</nomeEntidade>\n";
		  $xml .=  "<nomeMunicipio>".$dados['nomemunicipio']."</nomeMunicipio>\n";
		  $xml .=  "<uf>".$dados['uf']."</uf>\n";
		  $xml .=  "<nomeDirigente>".$dados['nomedirigente']."</nomeDirigente>\n";
      $xml .=  "<projetos>\n";
    }
    if($loop_pi) {
      $xml .= "<projeto>";
      $xml .= "<ano>".$dados['ano']."</ano>\n";
      $xml .= "<pi>".$dados['pi']."</pi>\n";
      $xml .= "<cronogramaExecucaoInicial>".$dados['cronogramaexecucaoinicial']."</cronogramaExecucaoInicial>\n";
      $xml .= "<cronogramaExecucaoFinal>".$dados['cronogramaexecucaofinal']."</cronogramaExecucaoFinal>\n";
      $xml .= "<acoes>\n";
    }
    $codigoibge_loop = $dados['codigoibge'];
    $pi_loop = $dados['pi'];
}



function sql() {
  return "
  select

m.muncod as codigoIbge,

m.mundescricao as nomeEntidade,

m.mundescricao as nomeMunicipio,

m.estuf as UF,

iu.inucadsec as nomeDirigente,

'2008' as ano,

pr.prgplanointerno as pi,

a.acidtinicial as cronogramaExecucaoInicial,

a.acidtfinal as cronogramaExecucaoFinal,

d.dimcod as codigoDimensao,

d.dimdsc as descricaoDimensao,

--descrição da subação + estratégia de implementação + descrição da unidade de medida + quantidade (por ano) + valor unitário

s.sbadsc || ' ' || s.sbastgmpl || ' ' || u.unddsc || ' ' || 'Ano1:'||s.sba0ano

||'Ano2:'|| s.sba1ano ||'Ano3:'|| s.sba2ano ||'Ano4:'|| s.sba3ano ||'Ano5:'|| s.sba4ano || s.sbaunt as detalhamento,

(csa2.valor*0.99) as valorConcedente,

(csa2.valor*0.01) as valorProponente,

e.entcodent as codigoInep, 

e.entnome as nomeEscola,

q.qfaqtd as quantidadeAlunos,

b.benid as codigoBeneficiario,

b.bendsc as descricaoBeneficiario,

sb.vlrrural as quantidadeZonaRural,

sb.vlrurbano as quantidadeZonaUrbana,

s.sbaid as codigoSubacao,

s.sbadsc as descricaoSubacao,

u.undid as codigoUnidadeMedida,

u.unddsc as descricaoUnidadeMedida,

(s.sba0ano + s.sba1ano + s.sba2ano + s.sba3ano + s.sba4ano) as quantidade,

s.sbaunt as valorUnitario,

csa.cosdsc as descricaoItem,

csa.cosqtd as quantidade,

csa.cosvlruni as valorUnitario

from

cte.dimensao d

inner join cte.areadimensao ad ON ad.dimid = d.dimid

inner join cte.indicador i ON i.ardid = ad.ardid

inner join cte.criterio c ON c.indid = i.indid

inner join cte.pontuacao p ON p.crtid = c.crtid and p.indid = i.indid

inner join cte.instrumentounidade iu ON iu.inuid = p.inuid

inner join territorios.municipio m ON m.muncod = iu.muncod

--inner join territorios.municipio m ON m.estuf = iu.estuf

inner join cte.acaoindicador a ON a.ptoid = p.ptoid

inner join cte.subacaoindicador s ON s.aciid = a.aciid

inner join cte.unidademedida u ON u.undid = s.undid

inner join cte.qtdfisicoano q ON q.sbaid = s.sbaid

inner join entidade.entidade e ON e.entid = q.entid

inner join cte.subacaobeneficiario sb ON sb.sbaid = s.sbaid

inner join cte.beneficiario b ON b.benid = sb.benid

inner join cte.programa pr ON pr.prgid = s.prgid

inner join cte.composicaosubacao csa ON csa.sbaid = s.sbaid

inner join (select sbaid, sum(cosqtd*cosvlruni) as valor from cte.composicaosubacao group by sbaid ) csa2 ON csa2.sbaid = s.sbaid



group by

m.muncod,

m.mundescricao,

m.estuf,

iu.inucadsec,

pr.prgplanointerno,

a.acidtinicial,

a.acidtfinal,

d.dimcod,

d.dimdsc,

s.sbadsc || ' ' || s.sbastgmpl || ' ' || u.unddsc || ' ' || 'Ano1:'||s.sba0ano

||'Ano2:'|| s.sba1ano ||'Ano3:'|| s.sba2ano ||'Ano4:'|| s.sba3ano ||'Ano5:'|| s.sba4ano || s.sbaunt,

csa2.valor,

e.entcodent, 

e.entnome,

q.qfaqtd,

b.benid,

b.bendsc,

sb.vlrurbano,

sb.vlrrural,

s.sbaid,

s.sbadsc,

u.undid,

u.unddsc,

s.sba0ano + s.sba1ano + s.sba2ano + s.sba3ano + s.sba4ano,

s.sbaunt,

csa.cosdsc,

csa.cosqtd,

csa.cosvlruni

  ";
}