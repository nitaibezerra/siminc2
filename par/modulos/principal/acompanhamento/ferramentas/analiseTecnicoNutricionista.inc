<?php

if( $_REQUEST['requisicao'] == 'gerar_xls_quadro' )
{
	$tipoXls = $_REQUEST['tipo'];
	if($tipoXls)
	{
		
		switch ($tipoXls) {
			case 'mun':
				
				$file_name="MunicipiosComCadastrosNutricionista.xls";
				$sql = "
						SELECT mundescricao as \"Município\", estuf as \"UF\"
							FROM
							(
								SELECT 
									distinct inu.inuid, m.mundescricao, m.estuf
									from
										par.vinculacaonutricionista vn 
										inner join par.dadosnutricionista dn on vn.vncpf = dn.dncpf 
										inner join seguranca.usuario u on u.usucpf = dn.dncpf
										inner join par.situacaonutricionista sn on sn.snid = vn.snid
										inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
										LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
									where
										true 
									AND
										vn.snid <> 2 
									AND
										vn.vnstatus = 'A'
									-- MUNICIPAL E DIFERENTE DO DF
									AND
										itrid = 2
									AND
									    inu.muncod <> '5300108'
							)as foo
						
						";
				$cabecalho = array("Município","UF");
				
				break;
			case 'mun_sem':
				$file_name="MunicipiosSemCadastrosNutricionista.xls";
				$sql = "SELECT mundescricao as \"Município\", estuf as \"UF\"
							FROM
							(
								select 
									distinct inuid, m.mundescricao , m.estuf
								from
									par.instrumentounidade inu 
									LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
								WHERE inuid NOT IN 
								(
									SELECT 
										distinct inu.inuid
										from
											par.vinculacaonutricionista vn 
											inner join par.dadosnutricionista dn on vn.vncpf = dn.dncpf 
											inner join seguranca.usuario u on u.usucpf = dn.dncpf
											inner join par.situacaonutricionista sn on sn.snid = vn.snid
											inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
											LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
										where
											true 
										AND
											vn.snid <> 2 
										AND
											vn.vnstatus = 'A'
										-- MUNICIPAL E DIFERENTE DO DF
										AND
											itrid = 2
										AND
										    inu.muncod <> '5300108'
								)
								AND
										    inu.muncod <> '5300108'
								and 
									itrid = 2
							) as foo";
				$cabecalho = array("Município","UF");
				
				break;
			case 'est':
				$file_name="EstadosComCadastrosNutricionista.xls";
				$sql = "
						SELECT 'SEDUC-' || estuf as \"SEDUC\", 
							estdescricao as \"Estado\"
						FROM
						(
						SELECT 
								distinct inu.inuid, e.estdescricao, inu.estuf
								from
									par.vinculacaonutricionista vn 
									inner join par.dadosnutricionista dn on vn.vncpf = dn.dncpf 
									inner join seguranca.usuario u on u.usucpf = dn.dncpf
									inner join par.situacaonutricionista sn on sn.snid = vn.snid
									inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
									LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
								where
									true 
								AND
									vn.snid <> 2 
								AND
									vn.vnstatus = 'A'
								-- MUNICIPAL E DIFERENTE DO DF
								AND
									( itrid = 1 OR inu.muncod = '5300108')
						
						) AS foo
						";
				$cabecalho = array("SEDUC","ESTADO");
				break;
			case 'est_sem':
				$file_name="EstadosSemCadastrosNutricionista.xls";
				$sql = "
						
					SELECT 'SEDUC-' || estuf as \"SEDUC\",  estdescricao as \"Estado\"
					FROM
					(
						select 
							distinct inu.inuid, e.estdescricao, inu.estuf
						from
							par.instrumentounidade inu 
							LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
						WHERE inuid NOT IN 
						(
							SELECT distinct inu.inuid
							from
								par.vinculacaonutricionista vn 
								inner join par.dadosnutricionista dn on vn.vncpf = dn.dncpf 
								inner join seguranca.usuario u on u.usucpf = dn.dncpf
								inner join par.situacaonutricionista sn on sn.snid = vn.snid
								inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
								LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
							where
								true 
							AND
								vn.snid <> 2 
							AND
								vn.vnstatus = 'A'
							-- MUNICIPAL E DIFERENTE DO DF
							AND
								( itrid = 1 OR inu.muncod = '5300108')
						)
						AND
								( itrid = 1 OR inu.muncod = '5300108')
					) as foo
											
						";
				$cabecalho = array("SEDUC","ESTADO");
				
				break;
		}
		
		header("Pragma: public");
		header("Expires: 0");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		header("Content-Type: application/force-download");
		header("Content-Type: application/octet-stream");
		header("Content-Type: application/download");
		header("Content-Disposition: attachment;filename=".$file_name);
		header("Content-Transfer-Encoding: binary ");
		
		
		$db->monta_lista_tabulado($sql, $cabecalho, 100000000, 5, 'N', '100%', '');
		
		die();
	}
	else
	{
		die('<script>window.close();</script>');
	}
}
	


if( $_REQUEST['requisicao'] == 'gerar_xls' )
{
	
	$file_name="AnaliseNutricionista.xls";
	header("Pragma: public");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Content-Type: application/force-download");
	header("Content-Type: application/octet-stream");
	header("Content-Type: application/download");
	header("Content-Disposition: attachment;filename=".$file_name);
	header("Content-Transfer-Encoding: binary ");
	
	$sql = "
	select 
	CASE WHEN inu.estuf IS NULL THEN
			m.mundescricao || '-' || inu.mun_estuf
		ELSE
			estdescricao
		END
			as \"Unidade\",
			replace(to_char(u.usucpf::bigint, '000:000:000-00'), ':', '.')  as \"CPF\",
			u.usunome as \"Nome\",
			dn.dncrn  as \"CRN\",
			dn.dncrnuf  as \"Região CRN\",
			CASE WHEN dncrnprovisorio = TRUE THEN
				'Sim'
			WHEN dncrnprovisorio = FALSE THEN
				'Não'
			END as \"Provisório\",
			CASE WHEN vn.vnstatus = 'A' THEN
				'Vinculado'
			ELSE
				'Desvinculado'
			END as \"Vinculação\",
			sn.sndescricao as \"Situação\",
			to_char(ent.entdatanasc, 'DD/MM/YYYY' )as \"Data de nascimento\",
			dn.dnnomemae as \"Nome da mãe\",
			CASE WHEN 
				ent.entsexo = 'M'
			THEN
					'masculino'
			WHEN 
					ent.entsexo = 'F'
			THEN 
					'feminino'	
			ELSE
					null
			END
			 AS \"Sexo\",
			 entemail as \"E-mail principal\",
			dn.dnemailalternativo as \"E-mail alternativo\",
			'(' || ent.entnumdddresidencial || ') ' || ent.entnumresidencial as \"Telefone Fixo\",		-- numeroresidencial
			'(' || ent.entnumdddcelular || ') ' || ent.entnumcelular as \"Telefone Residencial\",		-- numerocelular
			ed.endcep as \"CEP\",
			ed.endlog as \"Logradouro\",
			ed.endcom as \"Complemento\",
			ed.endnum as \"Número\",
			ed.endbai as \"Bairro\",
			ed.estuf  as  \"Estado\",
			m.mundescricao as \"Município\"
			
			
		from
			par.vinculacaonutricionista vn 
			inner join par.dadosnutricionista dn on vn.vncpf = dn.dncpf 
			INNER JOIN entidade.entidade 	 ent 	ON ent.entid = dn.entid
			LEFT JOIN entidade.endereco 	 ed 		ON ed.entid = ent.entid
			inner join seguranca.usuario u on u.usucpf = dn.dncpf
			inner join par.situacaonutricionista sn on sn.snid = vn.snid
			inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
			LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
			LEFT JOIN territorios.estado e ON e.estuf = inu.estuf

		order by \"Unidade\"	
			";
	$cabecalho = array("Unidade","CPF","Nome","CRN","Região CRN", "CRN provisório", "Vinculação","Situação","Data de nascimento","Nome da mãe","Sexo","E-mail principal","E-mail alternativo", "Telefone Fixo", "Telefone Residencial", "CEP","Logradouro","Complemento","Número","Bairro","Estado", "Município");
	$db->monta_lista_tabulado($sql, $cabecalho, 100000000, 5, 'N', '100%', '');
	
	die('');
}

if( $_REQUEST['requisicao'] == 'salvar' )
{	
	if( is_array($_REQUEST['situacao']) ){
		foreach ($_REQUEST['situacao'] as $cpf => $arrInuid) {
			foreach($arrInuid as $inuid => $snid)
			{
				$sql = "update par.vinculacaonutricionista set snid = $snid where vncpf = '$cpf' and inuid = {$inuid} and vnstatus = 'A' ";
				$db->executar($sql);
			}
		}
		$db->commit();
		$db->sucesso('principal/acompanhamento/ferramentas/analiseTecnicoNutricionista');
		exit();
	}
	else
	{
		echo '<script> alert("Selecione pelo menos uma opção na coluna \'Ação\' para modificar a situação do nutricionista"); </script>';
	}
}

if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	echo carregaMunicipios( $_REQUEST );
	exit();
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo('Análise do FNDE Nutricionista', '');
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
            	<td rowspan="4" width="25%">
            		
            		<?php 
            			$sql = "select		
									 qtd_mun as mun_cadastrados,
									 qtd_est as est_cadastrados,
									 ((select count( distinct inuid ) from par.instrumentounidade where itrid = 2 and   muncod <> '5300108') - qtd_mun) AS mun_nao_cadastrados,
									 ((select count( distinct inuid ) from par.instrumentounidade where itrid = 1 OR  muncod = '5300108' ) - qtd_est) AS est_nao_cadastrados	
								FROM
								(
								select
									(
										SELECT 
										count(distinct inu.inuid)
										from
											par.vinculacaonutricionista vn 
											inner join par.dadosnutricionista dn on vn.vncpf = dn.dncpf 
											inner join seguranca.usuario u on u.usucpf = dn.dncpf
											inner join par.situacaonutricionista sn on sn.snid = vn.snid
											inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
											LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
										where
											true 
										AND
											vn.snid <> 2 
										AND
											vn.vnstatus = 'A'
										-- MUNICIPAL E DIFERENTE DO DF
										AND
											itrid = 2
										AND
										    inu.muncod <> '5300108'
									) as qtd_mun,
									(
										SELECT 
										count(distinct inu.inuid)
										from
											par.vinculacaonutricionista vn 
											inner join par.dadosnutricionista dn on vn.vncpf = dn.dncpf 
											inner join seguranca.usuario u on u.usucpf = dn.dncpf
											inner join par.situacaonutricionista sn on sn.snid = vn.snid
											inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
											LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
										where
											true 
										AND
											vn.snid <> 2 
										AND
											vn.vnstatus = 'A'
										-- ESTADUAL E DF
										AND
											( itrid = 1 OR inu.muncod = '5300108')
									) as qtd_est
									    
										
								)
								as foo 
										";
            				$dadosCadastro = $db->pegaLinha($sql);
            				
            				$munCadastrados 	= $dadosCadastro['mun_cadastrados']; 
            				$munNaoCadastrados 	= $dadosCadastro['mun_nao_cadastrados']; 
            				$estCadastrados 	= $dadosCadastro['est_cadastrados']; 
            				$estNaoCadastrados 	= $dadosCadastro['est_nao_cadastrados']; 
            		
            		?>
            	
            	
            		<table cellspacing="1" cellpadding="3" border="0" align="center" class="tabela">
					<tbody><tr class="titulo">
						<td style="font-size:14px;font-weight:bold;text-align:center;background-color: #dcdcdc" colspan="3">
							Quadro resumo de cadastro
						</td>
					</tr>	<tr class="dados_resumo">
						<td width="60%" class="SubTituloDireita">Quantidade de municípios com cadastro:</td>
						<td><center><b><?php echo $munCadastrados ?></b></center></td>
						<td width="10%" ><center><b><img onclick="getExcelQuadroResumo( 'mun' );" border="0" title="Aguardando" src="../imagens/excel.gif" width="15px"></b></center></td>
					</tr>
					<tr class="dados_resumo">
						<td class="SubTituloDireita">Quantidade de municípios sem cadastro:</td>
						<td><center><b><?php echo $munNaoCadastrados?></b></center></td>
						<td width="10%"><center><b><img onclick="getExcelQuadroResumo( 'mun_sem' );" border="0" title="Aguardando" src="../imagens/excel.gif" width="15px"></b></center></td>
					</tr>
					<tr class="dados_resumo">
						<td class="SubTituloDireita">Quantidade de Estados com cadastro:</td>
						<td><center><b><?php echo $estCadastrados ?></b></center></td>
						<td><center><b><img onclick="getExcelQuadroResumo( 'est' );" border="0" title="Aguardando" src="../imagens/excel.gif" width="15px"></b></center></td>
					</tr>
					<tr class="dados_resumo">
						<td class="SubTituloDireita">Quantidade de Estados sem cadastro:</td>
						<td><center><b><?php echo $estNaoCadastrados?></b></center></td>
						<td><center><b><img onclick="getExcelQuadroResumo( 'est_sem' );" border="0" title="Aguardando" src="../imagens/excel.gif" width="15px"></b></center></td>
					</tr>
					

					
				</tbody></table>
            	
            
            </td>
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
		<tr id="">
			<td class="subtituloDireita"> Situação:</td>
			<td id="td_muncod" colspan="3">
				<?php
					 
				$snid_combo = $_REQUEST['snid_combo'];
				
                
                if( $snid_combo[0] ){

					$sql = "SELECT distinct
								snid as codigo,
								sndescricao as descricao
							FROM
								par.situacaonutricionista
                            WHERE
                                snid IN (".implode(',',$snid_combo).")
                            ORDER BY
								sndescricao";

                	$snid_combo = $db->carregar($sql);
                }
                
                $sql_combo = "
					SELECT distinct
						snid as codigo,
						sndescricao as descricao
					FROM
						par.situacaonutricionista  
					ORDER BY
						sndescricao";
                
                combo_popup( 'snid_combo', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, '', true);
                ?>
				
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita"> Vinculação: </td>
			<td>
			
			<?php
				$vnStatus = $_REQUEST['vnstatus'];
				
			?>
				<input type="radio" <?=($vnStatus == '')  ? "checked=checked" : "";?> name="vnstatus" value=""> Todos
				<input type="radio" <?=($vnStatus == 'A')  ? "checked=checked" : "";?> name="vnstatus" value="A"> Vinculado
				<input type="radio" <?=($vnStatus == 'I')  ? "checked=checked" : "";?> name="vnstatus" value="I"> Desvinculado
				
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita"> Entidade disponível para validação: </td>
			<td>
			
			<?php
				$statusTecnico = $_REQUEST['statustecnico'];
				
			?>
				<input type="radio" <?=($statusTecnico == '')  ? "checked=checked" : "";?> name="statustecnico" value=""> Todos
				<input type="radio" <?=($statusTecnico == 'A')  ? "checked=checked" : "";?> name="statustecnico" value="A"> Sim
				<input type="radio" <?=($statusTecnico == 'I')  ? "checked=checked" : "";?> name="statustecnico" value="I"> Não
				
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita"> </td>
			<td>
				<input type="button" class="pesquisar" value="Pesquisar"/>
				<input type="button" value="Todos" onclick="window.location.href = window.location"/>
				<input type="button" class="excel" value="Gerar Excel (Sem filtros)" onclick=""/>
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

if( $_REQUEST['usunome'] ) 		$filtro .= " and u.usunome ilike '%".$_REQUEST['usunome']."%'";
if( $_REQUEST['dncrn'] ) 		$filtro .= " and dn.dncrn = '".$_REQUEST['dncrn']."'";
if( $_REQUEST['estuf'] )		$filtro .= " and (inu.estuf = '".$_REQUEST['estuf']."' or inu.mun_estuf = '".$_REQUEST['estuf']."')";
if( $_REQUEST['muncod'] ) 		$filtro .= " and inu.muncod = '".$_REQUEST['muncod']."'";
if( $_REQUEST['snid_combo'][0] != "" )   $filtro .= " and vn.snid IN (".implode(',',$_REQUEST['snid_combo']).")";
if( $_REQUEST['vnstatus'] )   $filtro .= " and vn.vnstatus =  '{$_REQUEST['vnstatus']}'";

if( $_REQUEST['statustecnico'] )
{
	if($_REQUEST['statustecnico'] == 'A')
	{
		$filtro .= "
				AND EXISTS (
					SELECT vnid from par.vinculacaonutricionista  where dutid = 11 and inuid = inu.inuid and vnstatus = 'A' and snid in ( 1, 4, 5) 
				)
			";
	}
	elseif($_REQUEST['statustecnico'] == 'I')
	{
		$filtro .= "
				AND 
				(
					EXISTS 
						(
							SELECT vnid from par.vinculacaonutricionista 
							INNER JOIN par.dadosunidade du ON duncpf = vncpf
							where du.dutid = 11 and du.inuid = inu.inuid and ( vnstatus = 'I' OR snid in ( 3, 2 ) )
						)
					OR
					NOT EXISTS
						(
							SELECT dunid from par.dadosunidade du where du.dutid = 11 and du.inuid = inu.inuid 
						)
					
				)
		";
	}	
}


//print_r( $_REQUEST);die();

//764.233.103-78
//743.405.658-49
$sql = "select 
			CASE WHEN vn.snid IN (1,4,5) AND vn.vnstatus = 'A' THEN
				'<center><label for=\"snid_4_'||dn.dncpf||'\">
				<input type=\"radio\" 
				' ||
				CASE WHEN vn.snid = 4 THEN
				 	'checked' 
				 ELSE
				 	''
				 END
				 || '
				 name=\"situacao['||dn.dncpf||']['||vn.inuid||']\" id=\"snid_4_'||dn.dncpf||'\" value=\"4\"> Aprovado</label> &nbsp;&nbsp;&nbsp;&nbsp;
             	<input type=\"hidden\" id=\"mostra_salvar\" value=\"mostrar\">	 <label for=\"snid_5_'||dn.dncpf||'\"><input type=\"radio\" 
             	' ||
				CASE WHEN vn.snid = 5 THEN
				 	'checked' 
				 ELSE
				 	''
				 END
				 || '
             	name=\"situacao['||dn.dncpf||']['||vn.inuid||']\" id=\"snid_5_'||dn.dncpf||'\" value=\"5\"> Reprovado</label></center>' 
             ELSE
             	'-'
             END		 
             as acao,
			CASE WHEN inu.estuf IS NULL THEN
				m.mundescricao || '-' || inu.mun_estuf
			ELSE
				estdescricao
			END
				as unidade,
    		u.usucpf,
		    '<span style=\"font-size:14px; color: #02025A; cursor:pointer;\" title=\"Abrir Dados Nutricionista\" class=\"glyphicon glyphicon-user\" onclick=\"abrirDadosNutricionista(\''||dn.dncpf||'\')\"></span>&nbsp;&nbsp;&nbsp;'|| u.usunome as nome,
				dn.dncrn ,
				dn.dncrnuf ,
				CASE WHEN vn.vnstatus = 'A' THEN
					'Vinculado'
				ELSE
					'Desvinculado'
				END as vinculacao,
				sn.sndescricao 
			from
				par.vinculacaonutricionista vn 
				inner join par.dadosnutricionista dn on vn.vncpf = dn.dncpf 
				inner join seguranca.usuario u on u.usucpf = dn.dncpf
				inner join par.situacaonutricionista sn on sn.snid = vn.snid
				inner join par.instrumentounidade inu ON inu.inuid = vn.inuid
				LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
				LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
			where
				--dn.dnstatus = 'A'
				true 
				 $filtro";

				 
$cabecalho = array('Ação', 'Estado/Município', 'CPF', 'Nome', 'CRN', 'Região', 'Vinculação', 'Situação'); 
$db->monta_lista($sql, $cabecalho, 50, 10, '', 'center', 'N', 'formulario_lista', '', '', '', '');
?>
<div id="salvar_validacao" style="display: none" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#cccccc">
    	<td style="text-align: center;"><input type="button" class="salvar" value="Salvar Validação"/></td>
    </tr>
</table>
</div>
<?php
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

	function getExcelQuadroResumo( tipo )
	{
		window.open(window.location + "&requisicao=gerar_xls_quadro&tipo="+tipo, '_blank');
	}

	jQuery(document).ready(function(){


		if(jQuery('#mostra_salvar').val() == "mostrar")
		{
			jQuery('#salvar_validacao').show();
		}
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
			selectAllOptions(document.getElementById('snid_combo'));
			jQuery('[name="formulario"]').submit();
		});
		jQuery('.excel').click(function(){
			window.open(window.location + "&requisicao=gerar_xls", '_blank');
		});
		
		jQuery('.salvar').click(function(){
			//jQuery('#requisicao').val('');
			jQuery('[name="formulario_lista"]').append('<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>');
			jQuery('[name="formulario_lista"]').submit();
		});
	});
</script>