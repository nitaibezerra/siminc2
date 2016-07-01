<?php 

/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Gustavo Fernandes da Guarda
 */
if($_POST['acao']){
	extract($_POST);
	
	if(count($arEspid) > 25){
		die("<script type=\"text/javascript\">alert('Não é permitido mais de 25 escolas.');history.go(-1);</script>");
	}
	
	$arEspid = ($arEspid) ? $arEspid : array();
	$oTecnicoMonitoraEscola = new TecnicoMonitoraEscola();
	$oTecnicoMonitoraEscola->excluiMonitoriaEscolaPorTecid($tecid);
	foreach($arEspid as $espid){
		$oTecnicoMonitoraEscola->espid = $espid;
		$oTecnicoMonitoraEscola->tecid = $tecid;
		$oTecnicoMonitoraEscola->salvar();
	}

	if($oTecnicoMonitoraEscola->commit()){
		die("<script type=\"text/javascript\">alert('Operação Realizada com Sucesso!');window.opener.location.replace(window.opener.location);window.close();</script>");
	}
	unset($oTecnicoMonitoraEscola);
}
extract($_GET);

$stWhere = "";
if($busca){
	$buscaTemp = removeAcentos($busca);
	$stWhere .= " and (public.removeacento(e.entnome) ilike '%{$buscaTemp}%' ";
	$stWhere .= " or e.entcodent like '%".trim($buscaTemp)."%' )";
}

if($chkEscolas == 2){
	$stWhere .= " and ep.espid not in (SELECT espid FROM escolaativa.tecnicomonitoraescola WHERE tecid <> $tecid) "; 
}

$sql = "select ep.espid, 
			   e.entnome, 
			   e.entcodent,
			   ee.endcep  || ' - '  || ee.endlog  || ', '  || coalesce(ee.endnum, '') || ' ' || coalesce(ee.endcom, '') || ' ' || coalesce(ee.endbai, '') || ' - ' || m.mundescricao || ' - ' || m.estuf as endereco,
			(select
				case when tme1.espid is not null and tme1.tecid = {$tecid} then '<input type=\"checkbox\" checked=\"checked\" name=\"arEspid[]\" value=\"'|| ep.espid ||'\">'
                when tme1.espid is not null then '<img src=\"../imagens/check_p.gif\" border=\"0\" class=\"checkAssociado\" alt=\"Professor Associado: '|| e1.entnome ||'\" >'
			    else '<input type=\"checkbox\" name=\"arEspid[]\" value=\"'|| ep.espid ||'\">'
			    end as acao	    
				from escolaativa.escolaparticipante ep1
			    left join escolaativa.tecnicomonitoraescola tme1 on ep1.espid = tme1.espid
                left join escolaativa.tecnico te1 on tme1.tecid = te1.tecid
                left join entidade.entidade e1 on te1.entid = e1.entid
			where 
                   ep1.espid = ep.espid ) as acao
		from cte.escolaativa ea
		inner join cte.instrumentounidade inu on inu.inuid = ea.inuid 
		inner join cte.escolaativaescolas eae on ea.esaid = eae.esaid
		inner join entidade.entidade e on eae.entid = e.entid
		inner join entidade.endereco ee on e.entid = ee.entid
		inner join escolaativa.escolaparticipante ep on eae.eaeid = ep.eaeid
		inner join territorios.municipio m on inu.muncod = m.muncod
	where inu.muncod = '{$muncod}' 
	$stWhere
	order by e.entnome";

	ver($sql,d);
$arDados = $db->carregar($sql);
$arDados = ($arDados) ? $arDados : array();

?>

<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	
	<link rel='stylesheet' type='text/css' href='../includes/superTitle.css'/>
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script type="text/javascript" src="../includes/superTitle.js"></script>
	
	<script type="text/javascript">
		<!--
		
		$(document).ready(function(){
			$("input").not( $(":button") ).keypress(function (evt) {
				if (evt.keyCode == 13) {
					$('#btbuscar').click();
					return false;
				}
			});
			
			$('#btbuscar').click(function(){
				var busca = $('#busca').val();
				var tecid = $('#tecid').val();
				var muncod = $('#muncod').val();
				var chkEscolas = $('input[name=chkEscolas]:checked').val();
				window.location.href = 'escolaativa.php?modulo=principal/popupAssociarEscolaProfessor&acao=A&busca=' + busca + '&tecid='+tecid+'&muncod='+muncod+'&chkEscolas='+chkEscolas;
			});

			$('#botaoOk').click(function(){
				$('#formulario').submit();
			});

			if($('input[name=arEspid[]]:checked').length > 24){
				$('input[name=arEspid[]]:not(:checked)').each(function() {
					$(this).attr('disabled', true);
			    });
			}
			
			$('input[name=arEspid[]]').click(function(){
				if($('input[name=arEspid[]]:checked').length > 24){
					$('input[name=arEspid[]]:not(:checked)').each(function() {
						$(this).attr('disabled', true);
				    });
				} else {
					$('input[name=arEspid[]]:disabled').each(function() {
						$(this).attr('disabled', false);
				    });
				}
			});

			jQuery('.checkAssociado').mousemove(function(){
					SuperTitleOff( this );
					if($(this).find('img')[0]){
						SuperTitleOn( this,$(this).find('img').attr('alt'));
					}
			    })
			    .mouseout(function(){
			    	if($(this).find('img')[0]){
			    		SuperTitleOff( this );
					}
		    });
			
		});

		//-->
	</script>
</head>
<body>
<?php
$titulo_modulo = "Associar escolas a professores";
monta_titulo( $titulo_modulo, '' );

if($chkEscolas == 2){
	$ckNaoAssociadas = "checked=\"checked\"";
} else {
	$ckTodas = "checked=\"checked\"";
}
//ver($tecid,d);
?>
<form name="formulario" id="formulario" method="post" action="escolaativa.php?modulo=principal/popupAssociarEscolaProfessor&acao=A">
	<input name="acao" id="acao" type="hidden" value="1">
	<input name="tecid" id="tecid" type="hidden" value="<?php echo $tecid ?>">
	<input name="muncod" id="muncod" type="hidden" value="<?php echo $muncod ?>">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" width="200px">Nome ou Código da Escola</td>
			<td><input type="text" class="normal" id="busca" name="busca" value="<?php echo $busca ?>" /></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita"></td>
			<td>
				<input type="radio" name="chkEscolas" id="todas" value="1" <?php echo $ckTodas;?> /> <label for="todas">Todas</label> 
				<input type="radio" name="chkEscolas" id="naoAssociadas" value="2" <?php echo $ckNaoAssociadas;?> /> <label for="naoAssociadas">Não Associadas</label>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td ></td>
			<td><input type="button" class="botao" id="btbuscar" name="btbuscar" value="Buscar"></td>
		</tr>
	</table>
<?php 
// CABEÇALHO da lista
$arCabecalho = array("Ação","Código","Descrição da Escola","Endereço");
		
// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
$arParamCol[0] = array("type" => Lista::TYPESTRING, 
					   "style" => "width:50px; color: #696969",
					   "html" => "{entcodent}",
					   "align" => "center");
$arParamCol[1] = array("type" => Lista::TYPESTRING, 
					   "style" => "width:300px;",
					   "html" => "{entnome}",
					   "align" => "left");
$arParamCol[2] = array("type" => Lista::TYPESTRING, 
					   //"style" => "width:1200px;",
					   "html" => "{endereco}",
					   "align" => "left");

$acao = "<center>
		   {acao}
		 <center>";
		
// ARRAY de parametros de configuração da tabela
$arConfig = array("style" => "width:95%;",
				  "totalLinha" => false,
				  "totalRegistro" => true);

$oLista = new Lista($arConfig);
$oLista->setCabecalho( $arCabecalho );
$oLista->setCorpo( $arDados, $arParamCol );
$oLista->setAcao( $acao );
$oLista->setClassTr( 'checkAssociado' );
$oLista->show();
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#cccccc">
		<td width="9px"></td>
		<td><input type="button" class="botao" id="botaoOk" name="bta" value="Salvar"></td>
	</tr>
</table>
</form>
</body>
</html>