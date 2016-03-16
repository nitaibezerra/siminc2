<?php


$dmdid = $_REQUEST['dmdid'];
$_SESSION['dmdid'] = $dmdid;


if(!$dmdid){
	print "<script>
				alert('Acesso Negado. Acesse novamente o link para acessar a demanda!');
				window.close(); 
		   </script>";
	exit;
}


date_default_timezone_set ('America/Sao_Paulo');


// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
//include_once '_funcoes.php';
//include_once '_componentes.php';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

print '<br>';

monta_titulo( 'Demanda - Código: '.$dmdid, '' );
?>
<html>
 <head>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">


<?php 

$sql = "

SELECT 	
	d.dmdtitulo as titulo,
	o.orddescricao ||' - '|| t.tipnome AS origem,
	d.dmddsc as descricao,
	
	'Setor:' || upper(ua.unasigla) ||' - '|| ua.unadescricao || 
	'<br>Edifício:' || la.lcadescricao ||
	'<br>Andar:' || aa.anddescricao ||
	'<br>Sala:' || d.dmdsalaatendimento AS local, 
	
	d.laaid,
	l.lcaid,
	d.unaid,
	d.motid,
	(CASE WHEN o.ordid = 2 OR o.ordid = 13 OR o.ordid = 14 THEN -- 2=Redes - 13=Gestão Documentos CGI 
			t.celid
		  ELSE
			c.celid
	END) AS celid,
	CASE WHEN d.dmdqtde > 1 THEN d.dmdqtde ELSE '1' END as qtde,
	CASE 
	  	WHEN esddsc <> 'Finalizada' AND esddsc <> 'Validada' AND esddsc <> 'Validada Fora do Prazo' THEN
	  		'<font color=red><b>' || esddsc || '</b></font>'
	  	ELSE
	  		'<font color=blue><b>' || esddsc || '</b></font>'
	END as situacao,
	CASE 
	  	WHEN d.dmdnomedemandante != '' THEN  upper(d.dmdnomedemandante)
	  	ELSE  upper(u.usunome)
	END as solicitante,
	u2.usunome as tecnico,
	to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI') AS datainclusao,
	to_char(d.dmddatainiprevatendimento, 'DD/MM/YYYY HH24:MI') AS dataprevini,
	to_char(d.dmddatafimprevatendimento, 'DD/MM/YYYY HH24:MI') AS dataprevfim,
	dataconc as dataconclusao,
	ce.celnome as celula		

FROM demandas.demanda AS d
	
LEFT JOIN demandas.tiposervico AS t ON t.tipid = d.tipid 
LEFT JOIN demandas.origemdemanda AS o ON o.ordid = t.ordid 
LEFT JOIN demandas.sistemadetalhe AS s ON s.sidid = d.sidid 
LEFT JOIN demandas.sistemacelula AS c ON c.sidid = d.sidid
LEFT JOIN demandas.celula AS ce ON ce.celid = c.celid or ce.celid = t.celid
LEFT JOIN workflow.documento doc ON doc.docid = d.docid
LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid 
LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
LEFT JOIN demandas.unidadeatendimento ua ON ua.unaid = d.unaid
LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
LEFT JOIN demandas.localatendimento AS la ON la.lcaid = l.lcaid
LEFT JOIN demandas.andaratendimento AS aa ON aa.andid = l.andid
LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc						
						from 	workflow.historicodocumento a
							inner join workflow.documento c on c.docid = a.docid
					where a.aedid in (146, 191) 
					group by a.docid
					) ) as hst ON hst.docid = d.docid
WHERE d.dmdid = ".$dmdid;

//dbg($sql,1);

$dados = $db->pegaLinha($sql);

extract($dados);


if(!$dataprevini) 	$dataprevini 	= 'Não Informado';
if(!$dataprevfim) 	$dataprevfim 	= 'Não Informado';
if(!$tecnico) 	  	$tecnico 	  	= 'Não Informado';
if(!$dataconclusao) $dataconclusao 	= 'Não Informado';

if(!$situacao) $situacao = '<font color=red><b>Em Processamento</b></font>';

?>
	<table align="center" class="Tabela" style='border-bottom:2px solid #000;'>
			 <tbody>
				<tr>
					<td width='30%'  style="text-align: right;" class="SubTituloEsquerda">Solicitante:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$solicitante?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Origem / Tipo:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$origem?></td>
				</tr>
				<?php if($celula){?>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Célula:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$celula?></td>
				</tr>
				<?php }?>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Quantidade do serviço:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$qtde?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Assunto:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$titulo?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Descrição:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$descricao?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Local de atendimento:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$local?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Data de abertura:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$datainclusao?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Previsão de início do atendimento:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dataprevini?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Previsão de término do atendimento:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dataprevfim?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Técnico responsável:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$tecnico?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Data de conclusão do atendimento:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dataconclusao?></td>
				</tr>
			 	<tr>
					<td  style="text-align: right;" class="SubTituloEsquerda">Situação:</td>
					<td  style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$situacao?></td>
				</tr>
			 </tbody>
	</table>


</body>
</html>
