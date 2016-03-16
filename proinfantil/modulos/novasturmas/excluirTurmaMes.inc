<?php
if( $_POST['requisicao'] == 'excluirmes' ){

	if( is_array($_POST['ntmmmes']) ){
		$muncod = $_POST['muncod'];
		$ntmmparecerexclusao =  str_ireplace('\"', '', $_POST['ntmmparecerexclusao']);
		$ntmmparecerexclusao =  str_ireplace('"', '', $ntmmparecerexclusao);
		$ntmmparecerexclusao =  str_ireplace("'", '', $ntmmparecerexclusao);
		
		foreach ($_POST['ntmmmes'] as $ano => $mes) {
			/*$sql = "DELETE FROM proinfantil.novasturmasdiligencia 
					WHERE 
						(diltexto = 'null' or diltexto is null)
						and turid in (select t.turid from proinfantil.turma t 
									where t.muncod = '$muncod' 
										and t.turmes = '$mes' 
										and t.turano = '$ano'
										and t.turid not in (select turid from proinfantil.novasturmasworkflowturma where muncod = '$muncod' and ntwdataenvio is not null)
					    				)";
			$db->executar($sql);
			$sql = "delete from proinfantil.novasturmasalunoatendido where turid in (
						select t.turid from proinfantil.turma t 
									where t.muncod = '$muncod' 
										and t.turmes = '$mes' 
										and t.turano = '$ano'
										and t.turid not in (select turid from proinfantil.novasturmasworkflowturma where muncod = '$muncod' and ntwdataenvio is not null)
					)";
			$db->executar($sql);
			$sql = "delete from proinfantil.fotos where turid in (
						select t.turid from proinfantil.turma t 
									where t.muncod = '$muncod' 
										and t.turmes = '$mes' 
										and t.turano = '$ano'
										and t.turid not in (select turid from proinfantil.novasturmasworkflowturma where muncod = '$muncod' and ntwdataenvio is not null)
					)";
			$db->executar($sql);
			$sql = "delete from proinfantil.novasturmasworkflowturma where turid in (
						select t.turid from proinfantil.turma t 
									where t.muncod = '$muncod' 
										and t.turmes = '$mes' 
										and t.turano = '$ano'
										and t.turid not in (select turid from proinfantil.novasturmasworkflowturma where muncod = '$muncod' and ntwdataenvio is not null)
					)";
			$db->executar($sql);*/
			//$sql = "delete from proinfantil.turma where muncod = '$muncod' and turano = '$ano' and turmes = '$mes'";
			$sql = "update proinfantil.turma set turstatus = 'I' where muncod = '$muncod' and turano = '$ano' and turmes = '$mes'";
			$db->executar($sql);
			//$sql = "delete from proinfantil.novasturmasdadosmunicipiospormes where muncod = '$muncod' and ntmmano = '$ano' and ntmmmes = '$mes'";
			$sql = "update proinfantil.novasturmasdadosmunicipiospormes set ntmmstatus = 'I', ntmmparecerexclusao = '$ntmmparecerexclusao', ntmmusucpfexclusao = '{$_SESSION['usucpf']}' where muncod = '$muncod' and ntmmano = '$ano' and ntmmmes = '$mes'";
			$db->executar($sql);
		}
		$db->commit();
		$db->sucesso('novasturmas/excluirTurmaMes');
		exit();		
	}
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function filtraMunicipio(estuf) {
	if(estuf!=''){
		$('#formulario').submit();
	}
}

function pesquisarMunicipio(){
	if( $('[name="estuf"]').val() == ''){
		alert('Informe o Estado');
		$('[name="estuf"]').focus();
		return false;
	} else if( $('[name="muncod"]').val() == ''){
		alert('Informe o Município');
		$('[name="muncod"]').focus();
		return false;
	} else {
		$('#formulario').submit();
	}
}

function excluirMes(){
	if( $('[name="estuf"]').val() == ''){
		alert('Informe o Estado');
		$('[name="estuf"]').focus();
		return false;
	} else if( $('[name="muncod"]').val() == ''){
		alert('Informe o Município');
		$('[name="muncod"]').focus();
		return false;
	} else if( $('[name="ntmmparecerexclusao"]').val() == ''){
		alert('Informe o Parecer de Exclusão');
		$('[name="ntmmparecerexclusao"]').focus();
		return false;
	} else {
		if( $('.chk"]:checked').length > 0 ){
			$('#requisicao').val('excluirmes');
			$('[name="formulario"]').submit();
		}else{
			alert('Escolha pelo menos um mês.');
			return false;
		}
	}
}
</script>

<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" id="requisicao" name="requisicao" value="">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
        <td class="SubTituloDireita" width="25%">Estado:</td>
        <td>
        	<?php
        	$estuf = $_REQUEST['estuf']; 
        	$sql = "SELECT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estdescricao"; 
			$db->monta_combo( "estuf", $sql, "S", "Selecione...", 'filtraMunicipio', "","","","N","","",$estuf); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="25%">Município:</td>
        <td id="td_municipio">
        	<?php
        	$muncod = $_REQUEST['muncod']; 
        	$sql = "SELECT ter.muncod AS codigo, ter.mundescricao AS descricao FROM territorios.municipio ter WHERE ter.estuf = '$estuf' ORDER BY ter.estuf, ter.mundescricao"; 
        	$db->monta_combo("muncod",$sql,"S","Selecione...",'',"","","","N","","",$muncod); ?>
        </td>
    </tr>
    <tr>
	    <td align='right' class="SubTituloDireita">Parecer de Exclusão:</td>
	    <td><?=campo_textarea('ntmmparecerexclusao', 'N', 'S', 'Parecer de Exclusão', 120, 15, 4000, '', '', '', '', 'Parecer de Exclusão');?></td>
	</tr>
    <tr bgcolor="#cccccc" align="center">
		<td colspan="2">
			<input type="button" value="Pesquisar" name="btn_pesquisar" onclick="pesquisarMunicipio();" />
			<input type="button" value="Excluir Mês" name="btn_listar_todas" onclick="excluirMes();"/>
		</td>
	</tr>
	<?if( $muncod ){ 
		
		$sql = "select
				CASE WHEN ntm.ntmmmes = (SELECT max(ntmmmes) FROM proinfantil.novasturmasdadosmunicipiospormes n WHERE muncod = ntm.muncod and ntmmano = '{$_SESSION['exercicio']}' and ntmmstatus = 'A')
					THEN '<center><input type=checkbox class=chk id=ptrid name=ntmmmes['||ntm.ntmmano||'] value='||ntm.ntmmmes||' onclick=validaChk()></center>'
					ELSE '<center><input type=checkbox class=chk id=ptrid name=ntmmmes['||ntm.ntmmano||'] disabled value='||ntm.ntmmmes||'></center>'
				END as acao,
			    case when ntm.ntmmmes = 1 then 'Janeiro' 
			        when ntm.ntmmmes = 2 then 'Fevereiro'
			        when ntm.ntmmmes = 3 then 'Março' 
			        when ntm.ntmmmes = 4 then 'Abril' 
			        when ntm.ntmmmes = 5 then 'Maio' 
			        when ntm.ntmmmes = 6 then 'Junho' 
			        when ntm.ntmmmes = 7 then 'Julho' 
			        when ntm.ntmmmes = 8 then 'Agosto' 
			        when ntm.ntmmmes = 9 then 'Setembro' 
			        when ntm.ntmmmes = 10 then 'Outubro' 
			        when ntm.ntmmmes = 11 then 'Novembro' 
			        when ntm.ntmmmes = 12 then 'Dezembro' end||'/'||ntm.ntmmano as mes,
			    (select count(turid) from proinfantil.turma where muncod = ntm.muncod and turmes = ntm.ntmmmes and turano = cast(ntm.ntmmano as integer) ) as turma
			from proinfantil.novasturmasdadosmunicipiospormes ntm
			where
				ntm.muncod = '$muncod'
				and ntmmstatus = 'A'
				and ntm.ntmmano = '{$_SESSION['exercicio']}'
			order by ntm.ntmmmes asc";
		
		$cabecalho = array("Ação", "Mês/Ano", "Qtd Turmas");
		echo monta_titulo('Lista de Mês Cadastrado', '');
		$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '95%', '', true, '', '', true);
	}
	?>
</table>
</form>