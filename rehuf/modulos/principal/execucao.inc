<?php

function execucaoDetalhamento($dados) {
	global $db2;
	
	for($i=1;$i<=12;$i++) {
		$f_elementodespesa[] = "substr(ob.classificacao1_".sprintf("%02d", $i).",6,2 )='".$dados['edpcod']."'";
		$f_naturezadespesa[] = "ob.classificacao1_".sprintf("%02d", $i)." like '%".str_replace(".","",$dados['natureza'])."%'";
	}
	
	
	$sql = "select  ne.numero_ne,
			 -- ne.observacao,
			 ob.numero_ob,
			 ob.observacao,
			 -- substr(ob.classificacao1_01,6, 2 ) as elementodespesa,
			 -- ob.classificacao1_01 as natureza,
			 ob.valor_transacao_01 as valorOB
			from siafi".$dados['ano'].".ne ne
			inner join siafi".$dados['ano'].".ns ns ON trim(codigo_inscricao1_01) = substr(ne.numero_ne,12,12) 
			inner join siafi".$dados['ano'].".ob ob ON trim(gr_an_nu_documento_referencia) = ns.titulo_credito
			inner join ( select c.it_co_credor, c2.it_no_credor
			                     from (select c.it_co_credor, max(c.it_da_transacao) as it_da_transacao
			                                  from siafi2012.credor c
			                                  where length(c.it_co_credor) = 14 
			                                  group by c.it_co_credor) c
			                     inner join siafi".$dados['ano'].".credor c2 ON c2.it_co_credor = c.it_co_credor and c2.it_da_transacao = c.it_da_transacao ) c ON c.it_co_credor = ob.codigo_favorecido
			left join dw.naturezadespesa n ON n.ndpcod = substr(ob.classificacao1_01,2,8) 
			left join dw.ug u ON u.ungcod = ne.codigo_ug_operador 
			left join financeiro.listansdetalhes ld on ld.ns = ns.numero_ns and ld.nob = ob.numero_ob and (ld.ug = u.ungcod || ' - ' || u.ungdsc)
			where u.ungcod='".$dados['ungcod']."' and (".implode(" or ", $f_naturezadespesa).") order by 1";
	
	$cabecalho = array("Número do empenho","Número da OB","Observações da OB","Valor OB");
	$db2->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
	exit;
}


// Parâmetros para a nova conexão com o banco do SIAFI
$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

// Cria o novo objeto de conexão
$db2 = new cls_banco();



/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('execucaoDetalhamento'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */


if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';
/* Tratamento de segurança */
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
/* FIM - Tratamento de segurança */

// tratamento especial para os hospitais federais (ver somente plantões)
if($permissoes['hospfederal']) {
	echo "<script>
			window.location='rehuf.php?modulo=pregao/listaPregaoPreenchimento&acao=A';
		  </script>";
	exit;
}


if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}

echo "<script language=\"JavaScript\" src=\"../includes/prototype.js\"></script>";
echo "<script language=\"JavaScript\" src=\"./js/rehuf.js\"></script>";
echo "<script>
		function mudarAnoElementoDespesa() {
			document.getElementById('formanoelementodespesa').submit();
		}
	  </script>";

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */
/* montando cabeçalho */
monta_titulo( "Lista de Elementos de despesa", "");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
/* fim montando cabeçalho */

if(!$_REQUEST['anoelementodespesa']) $_REQUEST['anoelementodespesa'] = date("Y");

$s = "select entungcod from entidade.entidade where entid='".$_SESSION['rehuf_var']['entid']."'";
$ungcod = $db->pegaUm($s);


$sql = "select '<center><img src=\"../imagens/mais.gif\" title=\"mais\" border=\"0\" style=\"cursor:pointer;\" onclick=\"inserirDetalhamento(this,\'requisicao=execucaoDetalhamento&edpcod='||edpcod||'&natureza='||natureza||'&ano=".$_REQUEST['anoelementodespesa']."&ungcod='||ungcod||'\');\"></center>' as mais, 
			   '<center>'||natureza||'</center>' as natureza, 
			   natureza_desc, 
       		   edpdsc, sum(valor1) as empenhado, 
       		   sum(valor2) as liquidado, 
       		   sum(valor3) as pago
		from
		(
		
			SELECT 
			       sld.ungcod,sld.edpcod,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod AS natureza,
			       ung.ungdsc,edp.edpdsc,ndp.ndpdsc AS natureza_desc,
			       CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN 
			    sld.sldvalor 
			    END AS valor1,
			   CASE WHEN sld.sldcontacontabil in ('292130201','292130202','292130209','292130301') THEN 
			    sld.sldvalor END AS valor2,
			   CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN 
			    sld.sldvalor END AS valor3
			          FROM
			       dw.saldo".$_REQUEST['anoelementodespesa']." sld
			       LEFT JOIN dw.ug ung ON ung.ungcod = sld.ungcod LEFT JOIN dw.elementodespesa edp ON edp.edpcod = sld.edpcod LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, ndpdsc from dw.naturezadespesa where sbecod = '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod
			       WHERE sld.ungcod in ('".$ungcod."') AND sld.acacod in ('20RX') AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130201','292130202','292130209','292130301','292130301','292410403') 
			     
			group by  sld.ungcod,sld.edpcod,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod,
			  ung.ungdsc,edp.edpdsc,ndp.ndpdsc, sld.sldcontacontabil, sld.sldvalor
			order by sld.ungcod
			
		) as foo
		group by ungcod, edpcod, natureza, 
		       ungdsc, edpdsc, natureza_desc
		order by edpcod";


echo '<form method=post id=formanoelementodespesa>';
echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
echo '<tr><td class="SubTituloDireita"><b>Ano:</b></td><td>';

$anos = array(0=>array("codigo"=>"2009","descricao"=>"2009"),
			  1=>array("codigo"=>"2010","descricao"=>"2010"),
			  2=>array("codigo"=>"2011","descricao"=>"2011"),
			  3=>array("codigo"=>"2012","descricao"=>"2012"));
			  
$db->monta_combo('anoelementodespesa', $anos, 'S', '', 'mudarAnoElementoDespesa', '', '', '200', 'S', 'anoelementodespesa','',$_REQUEST['anoelementodespesa']);
echo '</td></tr>';
echo '</table>';
echo '</form>';

$cabecalho = array("&nbsp;","Cód. Natureza","Natureza de despesa","Elemento de despesa","Despesas Empenhadas(R$)","Despesas Liquidadas(R$)","Valores Pagos(R$)");
$db2->monta_lista_simples($sql,$cabecalho,50,5,'S','95%','S');

?>
