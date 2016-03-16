<?php
function arredondar_dois_decimal($valor) {
	   $float_arredondar=round($valor * 100) / 100;
	   return $float_arredondar;
}

function cteMontaTituloFinanceiro( $titulo, $subtitulo = '&nbsp;', $url = NULL )
{
	global $db;
	monta_titulo( $titulo, $subtitulo );
	$estuf = cte_pegarEstuf( $_SESSION['inuid'] );
	$muncod = cte_pegarMuncod( $_SESSION['inuid'] );
	if ( $estuf )
	{
		$descricao = cte_pegarEstdescricao( $estuf );
	}
	else if ( $muncod )
	{
		$descricao = cte_pegarMundescricao( $muncod );
	}
	else
	{
		return;
	}
	
	$percentagem = cte_pegarPercentagem( $_SESSION['inuid'] );
	$estado_documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	if($url != NULL){
		$url = $url;
	}else{
		$url = "?modulo=principal/estrutura_avaliacao&acao=A";
	}
	?>
	<table align="center" border="0" class="tabela" cellpadding="0" cellspacing="0" style="border-bottom: 0 !important;">
		<colgroup>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td  style="padding: 0 15px 0 15px; background-color:#fafafa; color:#404040;">
					<div style="float: left; position: relative;">
						<h3 title="Unidade da Federação">
							<a href="<?=$url; ?>"><?= $descricao ?></a>
						</h3>
					</div>
					<?php if( $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ): ?>
					<div style="float: right; text-align: right; position: relative; top: 15px; margin-bottom: 30px;">
						<?= $percentagem ?>%
						<div style="margin-left: 0; padding: 1px; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;" title="<?= $percentagem ?>%">
						<div style="font-size:4px;width: <?= $percentagem ?>%; height: 6px; max-height: 6px; background-color:#339933;">
					</div>
					<?php endif; ?>
				</td>
				<td style="padding: 0 15px 0 15px; background-color:#fafafa; color:#404040; text-align: right;" >
					Ajuda
				</td>
			</tr>
		</tbody>
	</table>
	<?php
}

//////////// VER SE USA
function monitoramentoAnteriorEncerrado($prsid){
	global $db;
	$sql = "SELECT --to_char(hmcdatamonitoramento, 'MM') AS mes,
			hmcstatus 
			FROM cte.historicomonitoramentoconvenio 
			WHERE prsid = ".$prsid." ORDER BY hmcdatamonitoramento DESC LIMIT 1"; // recupera o mes do ultimo monitoramento feito.
	
	$status = $db->pegaUm($sql);
	if($status == "I"){
		return false;
	}else{
		return true;
	}
}

/**
 * function carregaConvenio($ano, $instrumentoUnidade, $prsid);
 * @desc   : Recupera o convênio apos ele finalizado. 
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $instrumentoUnidade (Instrumento unidade)
 * @param  : numeric $ano (ano de referencia)
 * @return : array $prsid (id do convênio)
 * @since 17/03/2009
 */
function carregaConvenio($ano, $instrumentoUnidade, $prsid){
	global $db;
	$sql = "SELECT p.prsid, p.prsano, p.prsnumconvsape
			FROM cte.projetosape p
			WHERE p.prsano =".$ano." 
			AND p.inuid = ".$instrumentoUnidade."AND p.prsid = ".$prsid;
	$convenios 	= $db->carregar($sql);
	return $convenios;
}

function retiraMonitoramentosDaSession($prsid,$hmcid ){
	unset($_SESSION['hmc'][$prsid]);
	return true;
	
}

/**
 * function existeDadosFinanceirosCadastrados($prsid);
 * @desc   : Verifica se o convenio pode iniciar.
 * 			 Regra: Para iniciar o monitoramento tem que ter cadastrado os Dados Financeiros para o mes de monitoramento.
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $prsid 
 * @return : boleano true ou false
 * @since 05/06/2009
 */
function existeDadosFinanceirosCadastrados($prsid){
	global $db;
	$sql="SELECT hmdmes, hmdano FROM cte.historicomonitoramentodadosbancarios where prsid = ".$prsid;
	$dados = $db->carregar($sql);
	$existe = 0;
	if($dados){
		foreach($dados as $dados){
			//if( (date('m') >= $dados['hmdmes']) && (date('Y') >= $dados['hmdano']) ){
			if(date('Y') >= $dados['hmdano'] ){
				$existe = 1;
			}
		}
		if($existe == 1){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}


?>