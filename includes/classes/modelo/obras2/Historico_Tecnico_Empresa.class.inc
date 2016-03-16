<?php
	
class Historico_Tecnico_Empresa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.historico_tecnico_empresa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hteid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hteid' => null, 
									  	'temid' => null, 
									  	'usucpf' => null, 
									  	'htedata' => null, 
    									'htedado' => null,
    									'htestatus' => null
									  );

	public function listaHistoricoTecnico( $temid )
	{
		
		$sql = "SELECT
					'<img style=\"cursor:pointer\" src=\"../imagens/consultar.gif\" onclick=\"exibirDadosTecnico(' || hteid || ')\"  />' as historico,
					usunome,
					case when htestatus = 'A'
						then 'Alterou / Ativou o técnico'
						else 'Alterou / Inativou  o técnico'
					end as acao,
					TO_CHAR(htedata, 'dd/mm/yyyy') AS data
				FROM
					obras2.historico_tecnico_empresa hte
				INNER JOIN 
					seguranca.usuario usu ON usu.usucpf = hte.usucpf
				WHERE
					temid = $temid";
		$arrCabecalho = array("Histórico","Responsável","Ação","Data");
		$this->monta_lista($sql,$arrCabecalho,100,10,"N","center","N");
	}
	
	public function exibirHistoricoTecnico( $hteid  = null)
	{
		$hteid = $hteid ? $hteid : $_REQUEST['hteid'];
		$sql = "SELECT
					htedado,
					TO_CHAR(htedata, 'dd/mm/yyyy') AS data
				FROM
					obras2.historico_tecnico_empresa hte
				WHERE
					hteid = $hteid";
		$arrHis = $this->pegaLinha($sql);
		$arrDados = unserialize($arrHis['htedado']);
		if($arrDados){
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
				<tr>
					<td colspan="2" class="SubTituloCentro">Dados do técnico em <?php echo $arrHis['data'] ?></td>
				</tr>
				<tr>
					<td width="35%" class="SubTituloDireita">CPF</td>
					<td>
						<?php echo $arrDados['temcpf']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Nome</td>
					<td>
						<?php echo $arrDados['temnome']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Cargo</td>
					<td>
						<?php echo $arrDados['temcargo']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Nº CREA/CAU</td>
					<td>
						<?php echo $arrDados['temnumcreacau']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Data de Capacitação</td>
					<td>
						<?php echo $arrDados['temdtcapacitacao']; ?>
					</td>
				</tr>
				<tr>
			        <td class="SubTituloDireita" >Status</td>
					<td>
						<?php
						if($arrDados['temstatus'] == "A"){
							echo "Ativo";
						}else{
							echo "Inativo";
						}
			            ?>
					</td>
				</tr>
			</table>
		<?php }
	}
	
}