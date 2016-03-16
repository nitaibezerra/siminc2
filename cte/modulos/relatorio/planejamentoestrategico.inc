<?
//print_r($_REQUEST); exit; 
if(isset($_REQUEST['estcod']) && $_REQUEST['estcod'] != ""){
	if($_REQUEST['tipoRelatorio']=='html' || $_REQUEST['tipoRelatorio']=='xls'){
	    include 'planejamentoestrategico_resultado.inc';
		exit;
	}else if($_REQUEST['tipoRelatorio']=='grafico'){
		include 'planejamentoestrategico_gerargraficos.inc';
		exit;
	}
}
include APPRAIZ . 'includes/cabecalho.inc';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
include APPRAIZ . 'includes/Agrupador.php';
global $db;
$ufPOST 			= "";
$microregiaoPOST 	= "";
$municipioPOST 		= "";
$disciplinaPOST 	= "";
if(isset($_REQUEST['estcod'])){
	$ufPOST = $_REQUEST['estcod'];
}

if(isset($_REQUEST['microregioes'])){
	$microregiaoPOST = $_REQUEST['microregioes'];
}

if(isset($_REQUEST['municipios'])){
	$municipioPOST = $_REQUEST['municipios'];
}

if(isset($_REQUEST['fodid'])){
	$disciplinaPOST = $_REQUEST['fodid'];
}

?>
<script type="text/javascript" src="../includes/planejamentoestrategico.js"></script>
<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" name="post">

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
        	<td>Filtros</td>
        </tr>
        <tr>
        	<td class="SubTituloDireita" valign="top" width="20%">Unidade de Federação: </td>
        	<td>
        	<?php
                $sqlComboEstado = "
					SELECT
                   		e.estuf as codigo, e.estdescricao as descricao
               		FROM territorios.estado e
                   	INNER JOIN cte.usuarioresponsabilidade ur ON
                       	ur.estuf = e.estuf
                  	INNER JOIN seguranca.perfil p ON
                       	p.pflcod = ur.pflcod
                   	INNER JOIN seguranca.perfilusuario pu ON
                       	pu.pflcod = ur.pflcod AND
                       	pu.usucpf = ur.usucpf
               		WHERE ";
               			if(!cte_possuiperfil(array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR))){
                			$sqlComboEstado .= "ur.usucpf = '" . $_SESSION['usucpf'] . "' AND ";
               			}
                   		$sqlComboEstado .= "ur.rpustatus = 'A' AND
                   		p.sisid = " . $_SESSION['sisid'] . "
               		GROUP BY
                   		codigo, descricao
                   	ORDER BY descricao
                ";
                $estados = $db->carregar( $sqlComboEstado );
                //combo_popup( "estcod", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400 );
      	  	?>
      	  	<select width="5" name="estcod" id="estcod" class="CampoEstilo" style="width:400px;" onChange="popularCombos('estcod', 'municipios', 'combo_municipios_planejamentoestrategico', 'microregioes', 'combo_microregiao_planejamentoestrategico');">
        		<?
        			echo "<option value=''>--Selecione--</option>";
        			foreach($estados as $estado){
        				$selected = "";
        				if($estado == $ufPOST){
        					$selected = "selected='true'";
        				}
        				echo "<option value='".$estado['codigo']."'" . $selected . ">" . $estado['descricao'] . "</option>";
        			}
        		?>
        	</select>
        	</td>
        </tr>
        <tr>
        	<td class="SubTituloDireita" valign="top" width="20%">Micro-Região: </td>
        	<td>
        		<select width="5" name="microregioes" id="microregioes"  onChange="popularCombo('microregioes', 'municipios', 'combo_municipios_planejamentoestrategico');" class="CampoEstilo" style="width:400px;" >
        			<option value=''>--Todos--</option>
        		</select>
        	</td>
        </tr>
        
        <tr>
        	<td class="SubTituloDireita" valign="top" width="20%">Município: </td>
        	<td>
        		<select width="5" name="municipios" id="municipios" class="CampoEstilo" style="width:400px;">
        			<option value="">--Todos--</option>
        		</select>
       		</td>
		</tr>
		<tr>
        	<td class="SubTituloDireita" valign="top" width="20%">Disciplina: </td>
        	<td>
        		<select width="5" name="fodid" id="fodid" class="CampoEstilo" style="width:400px;" >
        			<option value="">--Todos--</option>
        		<?php
        			$sqlComboDisciplina = "SELECT fodid as codigo, foddsc as descricao FROM cte.formacaodisciplina ORDER BY descricao";
        			$disciplinas = $db->carregar( $sqlComboDisciplina );
        			foreach($disciplinas as $disciplina){
        				echo '<option value="' . $disciplina['codigo'] . '">' . $disciplina['descricao'] . '</option>';
					} 
        			//combo_popup( "fodid", $sqlComboDisciplina, "Disciplina", "400x400", 0, array(), "", "S", false, false, 5, 400 ); 
        		?>
        		</select>
       		</td>
		</tr>
    	<tr>
            <td class="SubTituloDireita" valign="top">
                &nbsp;
            </td>
            <td class="SubTituloDireita" style="text-align:left;">
                <input type="button" id="relatorioHtml" name="relatorio" value="Visualizar" onclick="exibirRelatorio('html');" />
                <input type="button" id="relatorioPlanilha" name="relatorioPlanilha" value="Exportar Planilha" onclick="exibirRelatorio('xls');"/>
                <input type="button" id="grafico" name="grafico" value="Gerar Gr&aacute;fico" onclick="exibirRelatorio('grafico');"/>
                <input type="hidden" id="tipoRelatorio" name="tipoRelatorio" value="" />
            </td>
        </tr>
	</table>
	<input type="hidden" name="tipo" id="tipo" value="">
</form>