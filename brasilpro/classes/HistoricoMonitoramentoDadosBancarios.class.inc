<?php
	
class HistoricoMonitoramentoDadosBancarios extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicomonitoramentodadosbancarios";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hmdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hmdid' => null, 
									  	'hmdmes' => null, 
									  	'hmdano' => null, 
									  	'hmdsaldo1diames' => null, 
									  	'hmdsaldoultimodiames' => null, 
									  	'hmdvalorcontrapartida' => null, 
									  	'hmdrendimento' => null, 
									  	'prsid' => null, 
									  );
									  
	public function listaDadosBancarios($prsid){
		$listaDados = '	SELECT 	
								\'<center><img id="removeMes" name="removeMes" onclick="return removerMes(\' || hmdid || \')" src="/imagens/excluir.gif" title="Excluir" alt="Excluir \' || hmdmes || \'" /></center>\' as deletar,
								hmdmes, 
								hmdano, 
								hmdsaldo1diames, 
								hmdsaldoultimodiames, 
								hmdvalorcontrapartida, 
								hmdrendimento 
						FROM cte.historicomonitoramentodadosbancarios 
						WHERE prsid = '.$prsid.' 
						ORDER BY hmdano, hmdmes ASC';

		$cabecalho = array("Ação","Mês","Ano", "Saldo 1° dia do Mês", "Saldo último dia mês","Valor contra partida", "rendimento");
		
		return $this->monta_lista($listaDados,$cabecalho,20, 10, 'N', '100%', '');
	}
	
	/**
	 * function carregaMeses($prsid, $ano);
	 * @desc   : Faz a busca no banco e retorna a lista de Meses passivel de monitoramento do Convênio. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : 
	 * @return : 
	 * @since 17/03/2009
	 */
	public function carregaMeses($prsid, $ano){
		global $db;
		$temMonitoramento = 0;
		$sql = "	SELECT db.hmdmes, db.hmdano, hc.hmcid, to_char(hc.hmcdatamonitoramento, 'MM') AS mes, db.prsid , hc.hmcstatus
					FROM cte.historicomonitoramentodadosbancarios db
					LEFT JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = db.prsid 
									and to_char(hmcdatamonitoramento, 'MM')::integer = hmdmes
									and to_char(hmcdatamonitoramento, 'YYYY')::integer = hmdano
					WHERE db.prsid = ".$prsid." and db.hmdano >= ".$ano." ORDER BY hmdano, hmdmes";
		$meses = $db->carregar($sql);
		if(is_array($meses) ){
			foreach($meses as $dados){
				if($dados['hmcid']){
					$temMonitoramento = 1;
					break;
				}
			}
		}
		
		$sql = "SELECT db.hmdano, count(hmdano) AS ano
				FROM cte.historicomonitoramentodadosbancarios db
				LEFT JOIN cte.historicomonitoramentoconvenio hc ON hc.prsid = db.prsid 
					and to_char(hmcdatamonitoramento, 'MM')::integer = hmdmes
					and to_char(hmcdatamonitoramento, 'YYYY')::integer = hmdano
				WHERE db.prsid = ".$prsid."  and db.hmdano >= ".$ano."
				GROUP BY db.hmdano
				ORDER BY count(hmdano) desc";
		$totalMesesPorAno = $db->carregar($sql);
		
		$conteudo = $this->getMeses($meses, $prsid, $temMonitoramento, $totalMesesPorAno);
		return $conteudo; 
		
	}
	
	/**
	 * function getMeses;
	 * @desc   : Monta o HTML de lista de meses. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : 
	 * @return : 
	 * @since 17/03/2009
	 */
	public function getMeses($meses, $prsid, $temMonitoramento, $totalMesesPorAno = NULL){
		$conteudo = '
		<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">
				<tr>
					<th class="texto" >Selecione um mês do convênio para iniciar o Monitoramento.</th>
				</tr>
		</table>
		<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">';
		if(is_array($meses)){
			
			$arMeses = array();
			foreach( $meses as $arDados ){
				$arMeses[$arDados["hmdano"]][] = $arDados; 
			}
			
			$nrMaior = $totalMesesPorAno[0]["ano"];
			
			foreach( $totalMesesPorAno as $arDados ){
				$arColspan[$arDados["hmdano"]] = $arDados["ano"]; 
			}
			
			global $db;
			$data = new Data();
			
			$porcentagemTotal = 0;
			
			foreach( $arMeses as $ano => $arDados ){
				
				$colspan = ( $arColspan[$ano] != $nrMaior ) ? $nrMaior - $arColspan[$ano] : 1; 
				
				$conteudo .= '<tr>';
				foreach( $arDados as $dados ){
					
					if($dados['hmcid'] == $_SESSION['hmc'][$prsid] && $dados['mes'] == $dados['hmdmes'] && $dados['hmcstatus'] == "I"){
						$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmcid'].');"';
					}else if( $dados['hmcstatus'] == "F"){
						$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmcid'].');"';
					}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'],$dados['hmdano'], 1 ) == true){
						$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].');"';	
					}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'], $dados['hmdano'],1) == false ){
						$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].');"';
					}
					
					$mes = $data->mesTextual($dados['hmdmes']);
					$mes = substr($mes,0,3);
					$conteudo .='	<td class="SubTituloEsquerda" ><a style="cursor:pointer;" '.$btn.'>'.$mes.'/'.$dados["hmdano"].'</a></td>';
					
				}
				$conteudo .= $colspan != 1 ? '<td class="SubTituloEsquerda" colspan="'.$colspan.'">&nbsp;</td>' : "";
				$conteudo .= '</tr><tr>';
				foreach( $arDados as $dados ){
					
					$imagemStatus =  porcentagensMesesMonitoramento($_SESSION['inuid'],$_SESSION['ano'], $dados['prsid'], $dados['mes'], $porcentagemTotal);
				
					if($dados['hmcid'] == $_SESSION['hmc'][$prsid] && $dados['mes'] == $dados['hmdmes'] && $dados['hmcstatus'] == "I"){
						$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].');"';
						$imagem = '<div style=" position: relative; "> <img width="13" height="13"  src="../../imagens/lapis.png"/> </div>';
						
					}else if( $dados['hmcstatus'] == "F"){
						$btn = ' onclick="carregaMonitoramento('.$prsid.','.$dados['hmdmes'].');"';
						$imagem = '<div style=" position: relative; "> <img width="13" height="13"  src="../../imagens/check_p.gif"/> </div>';
						
					}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'],$dados['hmdano'], 1 ) == true){
						$imagem = '<div style=" position: relative; "> <img width="18" height="13"  src="../../imagens/cadeadoAberto.png"/> </div>';
						$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].');"';
						
					}else if($dados['hmcstatus'] == NULL && validaSePodeInserirMonitoramento($dados['prsid'], $dados['hmdmes'], $dados['hmdano'],1) == false ){
						$btn = ' onclick="validaSePodeInserirMonitoramento('.$prsid.','.$dados['hmdmes'].','.$dados['hmdano'].');"';
						$imagem = '<div style=" position: relative; "><img width="13" height="13"  src="../../imagens/cadiado.png"/> </div>';
						
					}
					$conteudo .='<td style="background-color: #F5F5F5;"><a style="cursor:pointer;" onmouseout="SuperTitleOff( this );" onmousemove="SuperTitleAjax(\'../../cte/monitoraFinanceiro/infoMonitora.php?hmcid='.$dados['hmcid'].'&ano='.$dados['hmdano'].'\')", this)  '.$btn.'>'.$imagemStatus.$imagem.'</a></td>';
					
				}
				$conteudo .= $colspan != 1 ? '<td style="background-color: #F5F5F5;" colspan="'.$colspan.'">&nbsp;</td>' : "";
				$conteudo .= '</tr>';
			}
		}else{
			$conteudo.= '<td class="SubTituloEsquerda"  style="padding-top: 10px; padding-bottom: 10px; background-color:#F5F5F5;"> Não existe meses cadastrados para ser monitorado. Entre em contato com o Gestor do Sistema. </td>';
		}
		$conteudo .='</table>';
		return $conteudo;
	}
}