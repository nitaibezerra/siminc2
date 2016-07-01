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
	
	$arTecid = ($arTecid) ? $arTecid : array();
	$oTecnicoMonitoraTecnico = new TecnicoMonitoraTecnico();
	$oTecnicoMonitoraTecnico->excluiProfessorPorTecidSuper($tecid);
	foreach($arTecid as $tecidmonitorado){
		$oTecnicoMonitoraTecnico->tmtid 		  = null;
		$oTecnicoMonitoraTecnico->tecidmonitorado = $tecidmonitorado;
		$oTecnicoMonitoraTecnico->tecidsuper 	  = $tecid;
		$oTecnicoMonitoraTecnico->salvar();
	}

	if($oTecnicoMonitoraTecnico->commit()){
		die("<script type=\"text/javascript\">alert('Operação Realizada com Sucesso!');window.opener.location.replace(window.opener.location);window.close();</script>");
	}
	unset($oTecnicoMonitoraTecnico);		
}
extract($_GET);

$sql = "select 
			    distinct
			    t.tecid,
			    e.entid,
			    e.entnome,
                tmt.tecidsuper,
                t2.nomesupervisor,
                case when tmt.tecidsuper is not null and tmt.tecidsuper = $tecid then '<input type=\"checkbox\" checked=\"checked\" name=\"arEspid[]\" value=\"'|| t.tecid ||'\">'
                when tmt.tecidsuper is not null then '<img src=\"../imagens/check_p.gif\" border=\"0\" class=\"checkAssociado\" alt=\"Supervisor: '|| t2.nomesupervisor ||'\" >'
                else '<input type=\"checkbox\" name=\"arTecid[]\" value=\"'|| t.tecid ||'\">'
                end as acao
			from escolaativa.tecnico t
			    inner join entidade.entidade e on t.entid = e.entid
			    inner join escolaativa.tecnicoresponsabilidade tr on t.tecid = tr.tecid
			    inner join escolaativa.usuarioresponsabilidade ur on e.entnumcpfcnpj = ur.usucpf and ur.rpustatus = 'A'
			    inner join territorios.municipio m on ur.muncod = tr.muncod
                left join escolaativa.tecnicomonitoratecnico tmt on t.tecid = tecidmonitorado
                left join (SELECT t2.tecid, e2.entnome as nomesupervisor FROM escolaativa.tecnico t2
                               inner join entidade.entidade e2 on t2.entid = e2.entid
                           ) t2 on t2.tecid = tmt.tecidsuper
			where 
			m.estuf = '$estuf' and 
			ur.pflcod = ".ESCOLAATIVA_PERFIL_MULTIPLICADOR."
	order by e.entnome";
	
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
$titulo_modulo = "Associar professores Supervisor";
monta_titulo( $titulo_modulo, '' );

if($chkEscolas == 2){
	$ckNaoAssociadas = "checked=\"checked\"";
} else {
	$ckTodas = "checked=\"checked\"";
}
?>
<form name="formulario" id="formulario" method="post" action="escolaativa.php?modulo=principal/popupAssociarProfessorSupervisor&acao=A">
	<input name="acao" id="acao" type="hidden" value="1">
	<input name="tecid" id="tecid" type="hidden" value="<?php echo $tecid ?>">
	<input name="estuf" id="estuf" type="hidden" value="<?php echo $estuf ?>">
<?php 
// CABEÇALHO da lista
$arCabecalho = array("Ação","Professor Multiplicador");
		
// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
$arParamCol[0] = array("type" => Lista::TYPESTRING, 
					   "style" => "width:90%;",
					   "html" => "{entnome}",
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
		<td><input type="submit" class="botao" id="botaoOk" name="bta" value="Salvar"></td>
	</tr>
</table>
</form>
</body>
</html>