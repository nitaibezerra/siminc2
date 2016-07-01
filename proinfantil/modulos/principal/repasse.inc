<?php
include APPRAIZ ."includes/workflow.php";
include APPRAIZ . "includes/cabecalho.inc";
include_once "_funcoes_proinfancia.php";
echo '<br/>';

$obrid = $_SESSION['proinfantil']['obrid'];
$pinid = $_SESSION['proinfantil']['pinid'];

$perfis = pegaPerfil($_SESSION['usucpf']);

if( !in_array(PERFIL_SUPER_USUARIO,$perfis) && !in_array(PERFIL_COORDENADOR,$perfis) && !in_array(PERFIL_ANALISTA_PAGAMENTO,$perfis) && !in_array(PERFIL_ADMINISTRADOR,$perfis) && !in_array(CONSULTA_GERAL,$perfis)){
		print "<script>alert('Seu perfil não possui acesso a esta página!');</script>";
		print "<script>history.back();</script>";
		exit;	
}

montaAbasProinfantil( $abacod_tela, $url );

monta_titulo('Período para Cálculo do Repasse dos Recursos', '');
cabecalhoProInfantil($obrid);

$docid = recuperaDocumentoProInfantil($pinid);
$pagamento = verificaPagamentoEfetuado($docid);

if(empty($pagamento)){
	$sql = "SELECT 		COALESCE(pi.pinperiodorepasse2,0) + COALESCE(pi.pinperiodorepasse,0) as pinperiodorepasse
			FROM 		proinfantil.proinfantil pi
			LEFT JOIN 	proinfantil.termoadesao t on t.pinid = pi.pinid and t.teatipoano = '1'
			WHERE 		pi.pinid = {$pinid}";
} else {
	$sql = "SELECT 		pi.pinperiodorepasse2 as pinperiodorepasse
			FROM 		proinfantil.proinfantil pi
			LEFT JOIN 	proinfantil.termoadesao t on t.pinid = pi.pinid and t.teatipoano = '1'
			WHERE 		pi.pinid = {$pinid}";
}

//Carrega Campos
$dados = $db->pegaLinha($sql);
if($dados) extract($dados);
?>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle;border:0px}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
	.div_historico { background: #white; }
	.div_historico:hover { background: #eeeeaa; }
</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width:95%">
    <tr>
    	<td>
			<table border="0" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
			    	<td align="left" style="font-size: 11px">
			    		(RESOLUÇÃO CD/FNDE Nº 15 DE 16 MAIO DE 2013 - Art. 6º Parágrafo único. A referência para
			    		a base de cálculo será sempre o valor anual mínimo por matrícula em creche e em pré-escola,
			    		em período integral e parcial, estabelecido nacionalmente pelo Fundeb para o ano anterior,
			    		computando-se 1/12 desse valor para cada mês de funcionamento, disposto em portaria do
			    		Ministro de Estado da Educação.)
			    	</td>
			    </tr>
			</table>	    	
    		<br>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="90%">
				<tr class="SubTitulotabela">
			    	<td colspan="5" class="center bold">VALOR DO REPASSE</td>
			    </tr>	
				<tr>
			        <td width="25%" class="SubTituloDireita">Valor Repassado:</td>
			        <td><?php echo $entnome; ?></td>
			    </tr>
				<tr>
			        <td class="SubTituloDireita">Valor a Repassar:</td>
			        <td><?php echo $obrdesc; ?></td>
			    </tr>
			</table>
		</td>
		<td width="80" valign="top"  align="right">
			<?php wf_desenhaBarraNavegacao( recuperaDocumentoProInfantil($pinid) , array( 'pinid' => $pinid ) ); ?>
		</td>
	</tr>
</table>