<?php

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo('Analisados', '');
?>
<script type="text/javascript" src="./js/par.js"></script>
<form method="post" name="formulario" id="formulario" action="" class="formulario" >
    <input type="hidden" name="requisicao" id="requisicao" value=""/>

    <table class="tabela" border="0" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr bgcolor="#cccccc">
            <td style="text-align: center;" colspan="6"><strong>Filtros</strong></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 15%">Nome:</td>
            <td style="width: 35%"><?php
            $usunome = $_REQUEST['usunome'];
            echo campo_texto('usunome', 'N', $habilitado, 'Id da Ação', 40, 50, '', '', '', '', '', 'id="usunome"', ''); ?></td>
        </tr>
        <tr>
        	<td class="SubTituloDireita">CRN:</td>
            <td style="width: 35%"><?php
            	$dncrn = $_REQUEST['dncrn'];
            	echo campo_texto('dncrn', 'N', 'S', 'Id da Ação', 10, 50, '[#]', '', '', '', '', 'id="dncrn"', ''); ?></td>
        </tr>
        <tr>
			<td class="subtituloDireita">UF:</td>
			<td colspan="3">
				<?php
					$estuf = $_POST['estuf'];
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita"> Município:</td>
			<td id="td_muncod" colspan="3">
				<?php
					if( $estuf ){
						$muncod = $_POST['muncod'];
						$sql = "SELECT muncod as codigo, mundescricao as descricao 
								FROM territorios.municipio 
								WHERE estuf = '$estuf'
								ORDER BY 2 ASC";
						$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
					}else{
						echo "Escolha uma UF.";
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita"> </td>
			<td>
				<input type="button" class="pesquisar" value="Pesquisar"/>
				<input type="button" value="Todos" onclick="window.location.href = window.location"/>
			</td>
		</tr>
	</table>
</form>
<div id="div_nutricionista" title="Dados Nutricionista" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_nutricionista"></div>
</div>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#cccccc">
    	<td style="text-align: center;" colspan="4"><strong>Lista</strong></td>
    </tr>
</table>
<?php 

if( $_REQUEST['usunome'] ) 	$filtro .= " and u.usunome ilike '%".$_REQUEST['usunome']."%'";
if( $_REQUEST['dncrn'] ) 	$filtro .= " and dn.dncrn = '".$_REQUEST['dncrn']."'";
if( $_REQUEST['estuf'] ) 	$filtro .= " and (inu.estuf = '".$_REQUEST['estuf']."' or inu.mun_estuf = '".$_REQUEST['estuf']."')";
if( $_REQUEST['muncod'] ) 	$filtro .= " and inu.muncod = '".$_REQUEST['muncod']."'";

//764.233.103-78
//743.405.658-49
$sql = "select distinct
			array_to_string(array(
            				select 
                            	case when inu1.itrid = 1 then e1.estuf||' - '||e1.estdescricao else m.estuf||' - '||m.mundescricao end  as municipio
                            from par.vinculacaonutricionista vn
								inner join par.instrumentounidade inu1 ON inu1.inuid = vn.inuid
                              	left join territorios.municipio m ON m.muncod = inu1.muncod
                              	left join territorios.estado e ON e.estuf = m.estuf
                              	left join territorios.estado e1 ON e1.estuf = inu1.estuf
                            where vn.vncpf = dn.dncpf and vn.vnstatus = 'A' order by e1.estdescricao, m.mundescricao
            					), ', <br>'),
		    u.usucpf,
		    '<span style=\"font-size:14px; color: #02025A; cursor:pointer;\" title=\"Abrir Dados Nutricionista\" class=\"glyphicon glyphicon-user\" onclick=\"abrirDadosNutricionista(\''||dn.dncpf||'\')\"></span>&nbsp;&nbsp;&nbsp;'|| u.usunome as nome,
			dn.dncrn as crn,
		    es.estdescricao as regiao,
		    sn.sndescricao as situacao
		from 
			par.dadosnutricionista dn
		   	inner join seguranca.usuario u on u.usucpf = dn.dncpf
		    inner join par.situacaonutricionista sn on sn.snid = dn.snid
		    left join territorios.estado es on es.estuf = dn.dncrnuf    
		    inner join par.vinculacaonutricionista vn on vn.vncpf = dn.dncpf and vn.vnstatus = 'A'
			inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
		where
			dn.dnstatus = 'A' and dn.snid in (4, 5) $filtro";

$cabecalho = array('Estado/Município', 'CPF', 'Nome', 'CRN', 'Região', 'Situação'); 
$db->monta_lista($sql, $cabecalho, 50, 10, '', 'center', 'N', 'formulario_lista', '', '', '', '');

function carregaMunicipios( $dados ){

	global $db;

	extract($dados);

	$sql = "SELECT muncod as codigo, mundescricao as descricao
			FROM territorios.municipio
			WHERE estuf = '$estuf'
			ORDER BY 2 ASC";
	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
}
?>

<script type="text/javascript">
	jQuery(document).ready(function(){	

		jQuery('[name="estuf"]').change(function(){
			if( jQuery('[name="esfera"]').val() != 'E' ){
				if( jQuery(this).val() != '' ){
					jQuery('#aguardando').show();
					jQuery.ajax({
				   		type: "POST",
				   		url: window.location.href,
				   		data: "requisicao=carregaMunicipios&estuf="+jQuery(this).val(),
				   		async: false,
				   		success: function(resp){
				   			jQuery('#td_muncod').html(resp);
				   			jQuery('#aguardando').hide();
				   		}
				 	});
				}else{
					jQuery('#td_muncod').html('Escolha uma UF.');
				}
			}
		});

		jQuery('.pesquisar').click(function(){
			jQuery('[name="formulario"]').submit();
		});
	});
</script>