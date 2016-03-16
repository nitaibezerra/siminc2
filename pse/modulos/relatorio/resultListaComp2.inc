<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000);  

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$titulo_modulo = "Relatório - Lista Componente 2";
monta_titulo( $titulo_modulo, '' );
?>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" id="requisicao" name="requisicao" value=""/>
	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>  
			<td class="SubTituloDireita">Tipo:</td>
			<td colspan='2'>
				<input type="radio" name="tiporel" value="" <?if(!$_POST['tiporel']){echo 'checked="checked"';}?> > Avaliações Realizadas
				&nbsp;
				<input type="radio" name="tiporel" value="S" <?if($_POST['tiporel'] == 'S'){echo 'checked="checked"';}?> > Avaliações Realizadas na SSE
				&nbsp;
				<input type="radio" name="tiporel" value="N" <?if($_POST['tiporel'] == 'N'){echo 'checked="checked"';}?> > Avaliações Realizadas Fora da SSE
			</td>
		</tr>
<!--                <tr id="trexercicio">
                    <td class="subtitulodireita" id="tdexercicio" style="display: none">Exercício:</td>
                    <td colspan='2'>
                            <select id="exercicio" name="exercicio" class='CampoEstilo' style="width: 80px; display: none">
                                    <option value="2013" <?php if ($_POST['exercicio'] == '2013'){ echo 'selected=selected'; } ?>>2013</option>
                                    <option value="2014" <?php if ($_POST['exercicio'] == '2014'){ echo 'selected=selected'; } ?>>2014</option>
                            </select>
                    </td>
                </tr>-->
                <tr>
			<td class="subtituloDireita">Exercício:</td>
			<td>
				<?php
				$sql = "
                                    select distinct to_char(avldtavaliacao, 'YYYY') as codigo, 
                                    to_char(avldtavaliacao, 'YYYY') as descricao from pse.avaliaescola 
                                    ORDER BY codigo ASC
                                    ";
                                $ano= $_POST['ano']; 
                    $db->monta_combo('ano', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'ano', null, '', $ano, '', null, null);
                    ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="25%">UF:</td>
			<td>
				<?php 
				$sql = "SELECT		estuf as codigo,
									estdescricao as descricao
						FROM		territorios.estado
						ORDER BY 	estuf";
				$estuf = $_POST['estuf'];
				$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município:</td>
			<td>
				<?php
				$mundescricao = $_POST['mundescricao']; 
				echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Gerar Excel" id="btnGerarExcel" />
				<input type="button" value="Limpar" id="btnLimpar" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
$(document).ready( function() {
    
    if($('input[name=tiporel]:radio:checked').val() == 'S'){
          $( "#exercicio" ).show();
          $( "#tdexercicio" ).show();
    }
    $('input[name=tiporel]:radio').click(function () {
        if($('input[name=tiporel]:radio:checked').val() == 'S'){
          $( "#trexercicio" ).show();
          $( "#tdexercicio" ).show();
          $( "#exercicio" ).show();
        }else{
          $( "#exercicio" ).hide();
          $( "#tdexercicio" ).hide();
          $( "#trexercicio" ).hide();
        }
    });
});
$(function(){
	$('#btnPesquisar').click(function(){	
		if($('[name=estuf]').val() == '' && $('[name=mundescricao]').val() == '' && $('[name=esdid]').val() == ''){
			alert('Preencha pelo menos um filtro!');
			return false;
		}
		$('[name="requisicao"]').val('pesquisar');
		$('#formulario').submit();
	});
	$('#btnGerarExcel').click(function(){
		$('[name="requisicao"]').val('excel');
		$('#formulario').submit();
	});
	$('#btnLimpar').click(function(){
		document.location.href = 'pse.php?modulo=relatorio/resultListaComp2&acao=A';
	});;
});
</script>
<?php

//if($_POST['tiporel']){
//	$arWhere[] = "av.avlavalsseoumonit='".$_POST['tiporel']."'";
//}
if(($_POST['tiporel'])){
    if( ($_POST['tiporel'] == 'N') ||  ($_POST['tiporel'] == '') ){
            $arWhere[] = " av.avlavalsseoumonit='".$_POST['tiporel']."' ";
    }
    if( ($_POST['tiporel'] == 'S') && ($_POST['exercicio'] == 2014) ){
            $arWhere[] = " (av.avlavalsseoumonit = 'S') AND av.avldtavaliacao > '31-03-2014' ";
    }
    if( ($_POST['tiporel'] == 'S') && ($_POST['exercicio'] == 2013) ){
            $arWhere[] = " (av.avlavalsseoumonit = 'S') AND av.avldtavaliacao < '01-04-2014' ";
    }
}

if($_POST['estuf']){
	$arWhere[] = "m.estuf = '{$_POST['estuf']}'";
}

if($_POST['mundescricao']){
	if( is_numeric( $_POST['mundescricao'] ) ){
		$arWhere[] = "m.muncod = '".trim($_POST['mundescricao'])."'";
	} else {
		$arWhere[] = "UPPER( removeacento(m.mundescricao) ) ilike '%".removeAcentos( str_to_upper( trim($_POST['mundescricao']) ) )."%'";
	}
}

    if( $_REQUEST['ano']){
                $anoInicio = $_REQUEST['ano']."-01-01";
                $anoFim = $_REQUEST['ano']."-12-31";
		$arWhere[] = " avldtavaliacao between '{$anoInicio}' and  '{$anoFim}'";		
	}

$sql = "SELECT 
                        m.estuf as estado, 
                        m.mundescricao as municipio,
                        esc.muncod as codibge,
                        a.cnecodigocnes as cnes,
                        a.cnenomefantasia ||' - '|| a.cneseqequipe as equipe,
                        --ent.entnome as escola,
                        --ne.niveldescricao as nivel, 
                        sum(av.avlqtpactuada) as qtdpactuada,
                        sum(av.avlqtrealizada) as qtdrealizada --,ac.acaodescricao as acao,
                FROM pse.avaliaescola av
                --INNER JOIN pse.niveldeensino ne ON ne.idnivel=av.idnivel
                INNER JOIN pse.acao ac ON ac.acaoid = av.acaoid and ac.comid = 2 and ac.acaoano=2013 and ac.acaoid in (25,26,28,29,30,31,35)
                INNER JOIN pse.escolabasepse esc ON esc.idebp = av.idebp
                --INNER JOIN entidade.entidade ent ON ent.entcodent = esc.idecodinep
                INNER JOIN territorios.municipio m ON m.muncod = esc.muncod
                INNER JOIN pse.scnesavaliaescola sc ON sc.idave = av.idave
                INNER JOIN pse.scnes a ON sc.cneid = a.cneid
                WHERE	
                        1=1
                ".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
                    
         GROUP BY
                m.estuf, 
                m.mundescricao,
                esc.muncod,
                a.cnecodigocnes,
                a.cnenomefantasia ||' - '|| a.cneseqequipe
                --ent.entnome,
                --ne.niveldescricao
         ORDER BY
                m.estuf, m.mundescricao, a.cnecodigocnes, a.cnenomefantasia ||' - '|| a.cneseqequipe
                --, ent.entnome
         ";
//$cabecalho = array('UF', 'Municípo', 'Código IBGE', 'CNES', 'Equipe', 'Escola', 'Nível', 'Quantidade Pactuada', 'Quantidade Realizada');
//$cabecalho = array('UF', 'Municípo', 'Código IBGE', 'CNES', 'Equipe', 'Nível', 'Quantidade Pactuada', 'Quantidade Realizada');
$cabecalho = array('UF', 'Municípo', 'Código IBGE', 'CNES', 'Equipe', 'Quantidade Pactuada', 'Quantidade Realizada');

if( $_POST['requisicao'] == 'excel' ){
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelComp2PorEquipe".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelComp2PorEquipe".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%','');
	exit;
} else {
        if(isset($_POST['tiporel']) || isset($_POST['exercicio']) || isset($_POST['estuf']) || isset($_POST['mundescricao']) ){
            $db->monta_lista_simples($sql,$cabecalho,100000,1,'','95%', 'S', '', '', '', true);
        }else{
            echo "<div style=\"font-size:12px\" ><center><font color=red><p>";
            echo       "Selecione um filtro, para carregar a lista.";
            echo "</p></font></center></div>";
        }
}
?>