<?php

/**
 * Modelo de Baixa de Bem
 * @author Alysson Rafael
 */
class BensMovimentacao extends Modelo{

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sap.movimentacaobens";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("mvbid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos	=	array(
		'mvbid' => null,
    	'tmvid' => null,
		'nu_matricula_siape' => null,
		'co_orgao' => null,
		'uendid' => null,
		'ternum' => null,
		'usucpf' => null,
		'mvbresponsaveldevolucaonumatriculasiape' => null,
		'mvbresponsaveldevolucaocoorgao' => null,
		'mvbresponsavelrecebimentonumatriculasiape' => null,
		'mvbresponsavelrecebimentocoorgao' => null,
		'mvbdevolucao' => null,
		'mvbdata' => null,
		'mvbstatus' => null,
		'mvbobservacao' => null,
    	'mvbtipo' => null,
    	'uendid_anterior' => null,
    	'ternumanterior' => null
	);


	/**
	 * Salva a Movimentação
	 * @name salvarMovimentacao
	 * @author Alysson Rafael
	 * @access public
	 * @param array $_POST - Array com os dados
	 * @return array
	 */
	public function salvarMovimentacao($post){

		extract($post);

		// tratando campos defaults
		$mvbdata = date('Y-m-d H:i:s');
		$usucpf = $_SESSION['usucpf'];
		$tmvid = empty($tmvid) ? 'null' : $tmvid;

		if($mvbtipo == 'L'){

			if(empty($tmvid)){
				$tmvid = 'null';
			}
			if(empty($uendidnova)){
				$uendidnova = 'null';
			}

			$sql = "INSERT INTO $this->stNomeTabela(tmvid,uendid,usucpf,mvbdata,mvbstatus,mvbobservacao,mvbtipo,uendid_anterior) ";
			$sql .= "VALUES(".$tmvid.",".$uendidnova.",'".$_SESSION['usucpf']."','".date('Y-m-d H:i:s')."','I','".$mvbobservacao."','".$mvbtipo."',".$uendidatual.") ";
			$sql .= "RETURNING mvbid ";

		}else if($mvbtipo == 'S'){

			$sql = "INSERT INTO
						$this->stNomeTabela
						(tmvid,nu_matricula_siape,usucpf,mvbdata,mvbstatus,mvbobservacao,mvbtipo)
					VALUES
						($tmvid,'$nu_matricula_siape_novo','$usucpf',
						'$mvbdata','I','$mvbobservacao','$mvbtipo')
					RETURNING mvbid";

		}else if($mvbtipo == 'G'){

			$mvbresponsaveldevolucaonumatriculasiape = empty($mvbresponsaveldevolucaonumatriculasiape) ? 'null' : "'".$mvbresponsaveldevolucaonumatriculasiape."'";
			$mvbresponsavelrecebimentonumatriculasiape = empty($mvbresponsavelrecebimentonumatriculasiape) ? 'null' : "'".$mvbresponsavelrecebimentonumatriculasiape."'";

			$sql = "INSERT INTO
						$this->stNomeTabela
						(tmvid, nu_matricula_siape, uendid, usucpf, mvbdata, mvbstatus, mvbobservacao,
						mvbtipo, mvbresponsaveldevolucaonumatriculasiape, mvbresponsavelrecebimentonumatriculasiape)
					VALUES
						($tmvid,'$nu_matricula_siape',$uendid,'$usucpf',
						'$mvbdata','I','$mvbobservacao','$mvbtipo', $mvbresponsaveldevolucaonumatriculasiape, $mvbresponsavelrecebimentonumatriculasiape)
					RETURNING mvbid";

		}

		//executa a query com este método para gravar log
		$mvbid = $this->pegaUm($sql);

		if(!empty($mvbid)){
       		$this->commit();
       		return array(0=>true,1=>$mvbid);
       	}
       	else{
       		return array(0=>false,1=>$mvbid);
       	}

	}

	/**
	 * Cancela todos os rgps incluidos na movimentacao
	 * @name cancelar
	 * @author Silas Matheus
	 * @access public
	 * @param int $mvbid - Identificador da Movimentação
	 * @return bool
	 */
	public function cancelar($mvbid){

		$resultado = true;
		$mvbid = addslashes($mvbid);

		// buscando todos os rgps da movimentacao
		$sql = "SELECT
					rgpid
				FROM
					sap.bensmovimentacaorgp
				WHERE
					mvbid=$mvbid";
		$rgps = $this->carregar($sql);

		// excluindo rgps da movimentacao
		$oBensMovimentacaoRgp = new BensMovimentacaoRgp();
		if(is_array($rgps)){

			foreach($rgps as $rgp){

				$resultado = $oBensMovimentacaoRgp->excluir($rgp['rgpid'], $mvbid);

			}
		}

		return $resultado;

	}

	/**
	 * Recupera registro de siape
	 * @name pegarMovimentacaoSiape
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador da movimentacao
	 * @return array
	 */
	public function pegarMovimentacaoSiape($id){

		$id = addslashes($id);

		$sql = "SELECT DISTINCT
					mvb.*,
					nu_matricula_siape_anterior nu_matricula_siape_atual,
					ser.no_servidor no_servidor_atual,
					ser.sg_funcao cargo,
					nu_matricula_siape_atual nu_matricula_siape_novo,
					ser1.no_servidor no_servidor_novo,
					tmv.tmvdsc
				FROM
					$this->stNomeTabela mvb
				LEFT JOIN
					sap.tipomovimentacao tmv
					ON tmv.tmvid=mvb.tmvid
				LEFT JOIN
					sap.vwrgps rgp
				 	ON rgp.mvbid=mvb.mvbid
				LEFT JOIN
					siape.vwservidorativo ser
					ON ser.nu_matricula_siape=nu_matricula_siape_anterior
				LEFT JOIN
					siape.vwservidorativo ser1
					ON ser1.nu_matricula_siape=nu_matricula_siape_atual
				WHERE
					mvb.mvbid=$id";


		$return = $this->carregar($sql);
		return $return[0];

	}

	/**
	 * Recupera registro da movimentacao geral
	 * @name pegarMovimentacao
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador da movimentacao
	 * @return array
	 */
	public function pegarMovimentacao($id){

		$sql = "SELECT
					*,
					uorno unidade,
					tmv.tmvdsc,
					ser1.no_servidor no_servidor_recebimento,
					ser2.no_servidor no_servidor_devolucao,
					ser.no_servidor no_servidor,
					ser.nu_matricula_siape,
					funcao.ds_funcao
				FROM
					$this->stNomeTabela mvb
				INNER JOIN
					siape.vwservidorativo ser
					ON ser.nu_matricula_siape=mvb.nu_matricula_siape
				LEFT JOIN
					siape.tb_siape_funcao funcao
					ON ser.sg_funcao=funcao.co_funcao
				INNER JOIN
					siorg.uorgendereco uorgend
					ON uorgend.uendid=mvb.uendid
				LEFT JOIN
					siape.vwservidorativo ser1
					ON ser1.nu_matricula_siape=mvb.mvbresponsavelrecebimentonumatriculasiape
				LEFT JOIN
					siape.vwservidorativo ser2
					ON ser2.nu_matricula_siape=mvb.mvbresponsaveldevolucaonumatriculasiape
				LEFT JOIN
					siorg.enderecoandarsala endandarsala
					ON uorgend.easid=endandarsala.easid
				LEFT JOIN
					siorg.enderecoandar endandar
					ON endandarsala.enaid=endandar.enaid
				LEFT JOIN
					siorg.endereco endereco
					ON endandar.endid=endereco.endid
				LEFT JOIN
					siorg.unidadeorganizacional unidade
					ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
				LEFT JOIN
					sap.tipomovimentacao tmv
					ON tmv.tmvid=mvb.tmvid
				WHERE
					mvb.mvbid=$id";

		$return = $this->carregar($sql);
		return $return[0];

	}

	/**
	 * Recupera os dados de uma movimentação
	 * @name pegarMovimentacaoLocalidade
	 * @author Alysson Rafael
	 * @access public
	 * @param int $mvbid - Identificador da movimentação
	 * @return array
	 */
	public function pegarMovimentacaoLocalidade($get){

		extract($get);

		//$sql = "SELECT uendid_anterior FROM sap.vwrgps WHERE mvbid='".$_GET['mvbid']."' ";
		$sql = "SELECT uendid_anterior FROM sap.movimentacaobens WHERE mvbid='{$mvbid}' ";
		$uendid_anterior = $this->pegaUm($sql);

		/*if(!$uendid_anterior){
			$sql = "SELECT bem.uendid FROM sap.rgp rgp ";
			$sql .= "JOIN sap.bensmaterial bmt ON rgp.bmtid=bmt.bmtid ";
			$sql .= "JOIN sap.bens bem ON bmt.benid=bem.benid ";
			$sql .= "WHERE rgp.rgpid='".$_GET['rgpid']."' ";
			$uendid_anterior = $this->pegaUm($sql);
		}*/

		if($uendid_anterior){
			$sqlView = "SELECT uorgend.uendid AS uendidatual,
		                   	   unidade.uorno AS unidadeatual,
		                       endereco.enduf AS endufatual,
		                       endereco.endcid AS endcidatual,
		                       endereco.endcep AS endcepatual,
		                       endereco.endlog AS endlogatual,
		                       endereco.endnum,
		                       endereco.endcom,
		                       endandar.enadescricao AS enadescricaoatual,
		                       endandarsala.easdescricao AS easdescricaoatual
		                       FROM siorg.uorgendereco uorgend
		                       LEFT JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
						       LEFT JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
						       LEFT JOIN siorg.endereco endereco ON endandar.endid=endereco.endid
						       LEFT JOIN siorg.unidadeorganizacional unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
						       WHERE uorgend.uendid='".$uendid_anterior."' ";
			$resView = $this->carregar($sqlView);

			$log = $resView[0]['endlogatual'];
			if(!empty($resView[0]['endnum'])){
				$log .= ','.$resView[0]['endnum'];
			}
			if(!empty($resView[0]['endcom'])){
				$log .= ','.$resView[0]['endcom'];
			}
			$resView[0]['endlogatual'] = $log;
		}



		$sql = "SELECT mov.mvbid,
		               mov.tmvid,
		               mov.uendid AS uendidnova,
		               mov.mvbobservacao,
		               mov.mvbstatus,
		               mov.ternum,
		               mov.mvbobservacao,
		               mov.mvbtipo,
		               unidade.uorno AS unidadenova,
		               endereco.enduf AS endufnova,
		               endereco.endcid AS endcidnova,
		               endereco.endcep AS endcepnova,
		               endereco.endlog AS endlognova,
		               endandar.enadescricao AS enadescricaonova,
		               endandarsala.easdescricao AS easdescricaonova
		               FROM sap.movimentacaobens mov
		               LEFT JOIN siorg.uorgendereco uorgend ON mov.uendid=uorgend.uendid
		               LEFT JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
		               LEFT JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
		               LEFT JOIN siorg.endereco endereco ON endandar.endid=endereco.endid
		               LEFT JOIN siorg.unidadeorganizacional unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
		               WHERE mov.mvbid='".$mvbid."' ";

		$resultado = $this->carregar($sql);

		if($uendid_anterior){
			$resultado[0]['uendidatual'] = $resView[0]['uendidatual'];
			$resultado[0]['unidadeatual'] = $resView[0]['unidadeatual'];
			$resultado[0]['endufatual'] = $resView[0]['endufatual'];
			$resultado[0]['endcidatual'] = $resView[0]['endcidatual'];
			$resultado[0]['endcepatual'] = $resView[0]['endcepatual'];
			$resultado[0]['endlogatual'] = $resView[0]['endlogatual'];
			$resultado[0]['enadescricaoatual'] = $resView[0]['enadescricaoatual'];
			$resultado[0]['easdescricaoatual'] = $resView[0]['easdescricaoatual'];
		}

		return $resultado[0];
	}

	/**
	 * Atualiza os dados da movimentacao
	 * @name atualizarMovimentacao
	 * @author Silas Matheus
	 * @access public
	 * @param int $_POST - Array com os dados
	 * @return array
	 */
	public function atualizarMovimentacao($post){

		extract($post);

		$mvbresponsaveldevolucaonumatriculasiape = empty($mvbresponsaveldevolucaonumatriculasiape) ? 'null' : "'".$mvbresponsaveldevolucaonumatriculasiape."'";
		$mvbresponsavelrecebimentonumatriculasiape = empty($mvbresponsavelrecebimentonumatriculasiape) ? 'null' : "'".$mvbresponsavelrecebimentonumatriculasiape."'";

		$sql = "UPDATE
					$this->stNomeTabela
				SET
					tmvid=$tmvid,
					nu_matricula_siape='$nu_matricula_siape',
					mvbobservacao='".$mvbobservacao."',
					uendid='".$uendid."',
					mvbresponsaveldevolucaonumatriculasiape=$mvbresponsaveldevolucaonumatriculasiape,
					mvbresponsavelrecebimentonumatriculasiape=$mvbresponsavelrecebimentonumatriculasiape
				WHERE
					mvbid='".$mvbid."'
				RETURNING mvbid";

		$mvbid = $this->pegaUm($sql);

		if(!empty($mvbid)){

			// gravando historico para as movimentacoes ativas
			$oBensHistorico = new BensHistorico();
			$oBensHistorico->gravar($bhsjustificativa, 'NULL', 'NULL', $mvbid);

			$this->commit();
       		return array(0=>true,1=>$mvbid);

       	}else{

       		return array(0=>false,1=>$mvbid);

       	}

	}

	/**
	 * Atualiza o registro de movimentação com status A
	 * @name concluirMovimentacao
	 * @author Alysson Rafael
	 * @access public
	 * @param array $_POST - Array com os dados
	 * @return array
	 */
	public function concluirMovimentacao($post){

		extract($post);

		if($mvbtipo == 'L'){
			if(empty($ternumanterior)){
				$sql = "UPDATE $this->stNomeTabela SET tmvid='".$tmvid."',ternum='".$ternum."',uendid='".$uendidnova."',";
				$sql .= "mvbstatus='A',mvbobservacao='".$mvbobservacao."' WHERE mvbid='".$mvbid."' ";
				$sql .= "RETURNING mvbid ";
			}
			else{
				$sql = "UPDATE $this->stNomeTabela SET tmvid='".$tmvid."',ternum='".$ternum."',uendid='".$uendidnova."',";
				$sql .= "mvbstatus='A',mvbobservacao='".$mvbobservacao."',ternumanterior='".$ternumanterior."' WHERE mvbid='".$mvbid."' ";
				$sql .= "RETURNING mvbid ";
			}
			
		}
		// se for por siape
		else if($mvbtipo == 'S'){

			if(empty($ternumanterior)){
				$sql = "UPDATE
						$this->stNomeTabela
					SET
						tmvid=$tmvid,
						ternum='".$ternum."',
						nu_matricula_siape='$nu_matricula_siape_novo',
						mvbstatus='A',
						mvbobservacao='".$mvbobservacao."'
					WHERE
						mvbid='".$mvbid."'
					RETURNING mvbid";
			}
			else{
				$sql = "UPDATE
						$this->stNomeTabela
					SET
						tmvid=$tmvid,
						ternum='".$ternum."',
						nu_matricula_siape='$nu_matricula_siape_novo',
						mvbstatus='A',
						mvbobservacao='".$mvbobservacao."',
						ternumanterior='".$ternumanterior."'
					WHERE
						mvbid='".$mvbid."'
					RETURNING mvbid";
			}
			

		}
		// se for geral
		else if($mvbtipo == 'G'){

			$mvbresponsaveldevolucaonumatriculasiape = empty($mvbresponsaveldevolucaonumatriculasiape) ? 'null' : "'".$mvbresponsaveldevolucaonumatriculasiape."'";
			$mvbresponsavelrecebimentonumatriculasiape = empty($mvbresponsavelrecebimentonumatriculasiape) ? 'null' : "'".$mvbresponsavelrecebimentonumatriculasiape."'";

			if(empty($ternumanterior)){
				$sql = "UPDATE
							$this->stNomeTabela
						SET
							tmvid=$tmvid,
							ternum='".$ternum."',
							nu_matricula_siape='$nu_matricula_siape',
							mvbstatus='A',
							mvbobservacao='".$mvbobservacao."',
							uendid='".$uendid."',
							mvbresponsaveldevolucaonumatriculasiape=$mvbresponsaveldevolucaonumatriculasiape,
							mvbresponsavelrecebimentonumatriculasiape=$mvbresponsavelrecebimentonumatriculasiape
						WHERE
							mvbid='".$mvbid."'
						RETURNING mvbid";
			}
			else{
				$sql = "UPDATE
							$this->stNomeTabela
						SET
							tmvid=$tmvid,
							ternum='".$ternum."',
							nu_matricula_siape='$nu_matricula_siape',
							mvbstatus='A',
							mvbobservacao='".$mvbobservacao."',
							uendid='".$uendid."',
							mvbresponsaveldevolucaonumatriculasiape=$mvbresponsaveldevolucaonumatriculasiape,
							mvbresponsavelrecebimentonumatriculasiape=$mvbresponsavelrecebimentonumatriculasiape,
							ternumanterior=$ternumanterior
						WHERE
							mvbid='".$mvbid."'
						RETURNING mvbid";
			}
			
			

		}

		//executa a query com este método para gravar log
		$mvbid = $this->pegaUm($sql);

		if(!empty($mvbid)){

			// gravando historico
			$oBensHistorico = new BensHistorico();
			$oBensHistorico->gravar($bhsjustificativa, 'NULL', 'NULL', $mvbid);

			// atualizando as situacoes dos bens
			$oBensMovimentacaoRgp = new BensMovimentacaoRgp();
			$oBensMovimentacaoRgp->atualizarRgps($mvbid, $tmvid);

       		return array(0=>true,1=>$mvbid);

       	}else{

       		return array(0=>false,1=>$mvbid);

       	}
	}

	/**
	 * Exclui um registro de movimentação
	 * @name excluirMovimentacao
	 * @author Alysson Rafael
	 * @access public
	 * @param int $mvbid - Identificador da movimentação
	 * @return void
	 */
	public function excluirMovimentacao($mvbid){
		$sql = "DELETE FROM sap.movimentacaobens WHERE mvbid='".$mvbid."' ";
		$this->executar($sql);
		$this->commit();
	}


	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaMovimentacaoLocalidade
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @param int $uendidatual - id do endereço de origem
     * @return array;
     */
	/*public function cabecalhoVisualizaMovimentacaoLocalidade($mvbid,$uendidatual){
		if(!empty($mvbid)){

			$mvbid = addslashes($mvbid);

			$sql = "SELECT tipo.tmvdsc,
			               unidade.uorno AS unidadenova,
			               endereco.endlog AS endlognova,
			               endandar.enadescricao AS enadescricaonova,
			               endandarsala.easdescricao AS easdescricaonova,
			               mov.mvbobservacao
			               FROM sap.movimentacaobens mov
			               JOIN sap.tipomovimentacao tipo ON mov.tmvid=tipo.tmvid
			               JOIN siorg.uorgendereco uorgend ON mov.uendid=uorgend.uendid
			               JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
			               JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
			               JOIN siorg.endereco ON endandar.endid=endereco.endid
			               JOIN siorg.unidadeorganizacional unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
			               WHERE mov.mvbid='".$mvbid."' ";

			$arDados = $this->pegaLinha($sql);

			$sql = "SELECT unidade.uorno AS unidadeatual,
			               endereco.endlog AS endlogatual,
			               endandar.enadescricao AS enadescricaoatual,
			               endandarsala.easdescricao AS easdescricaoatual
			               FROM siorg.uorgendereco uorgend
			               JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
			               JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
			               JOIN siorg.endereco ON endandar.endid=endereco.endid
			               JOIN siorg.unidadeorganizacional unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
			               WHERE uorgend.uendid='".$uendidatual."' ";

			$res = $this->pegaLinha($sql);

			$arDados['unidadeatual'] = $res['unidadeatual'];
			$arDados['endlogatual'] = $res['endlogatual'];
			$arDados['enadescricaoatual'] = $res['enadescricaoatual'];
			$arDados['easdescricaoatual'] = $res['easdescricaoatual'];

			$arDados = $arDados ? $arDados : array();
		}else{
			$arDados = array();
		}
		return $arDados;
	}*/

	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaMovimentacaoSiape
     * @author Silas Matheus
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array;
     */
	/*public function cabecalhoVisualizaMovimentacaoSiape($mvbid){

		if(!empty($mvbid)){

			$res = $this->pegarMovimentacaoSiape($mvbid);

			$arDados['nu_matricula_siape_atual'] = $res['nu_matricula_siape_atual'];
			$arDados['no_servidor_atual'] = $res['no_servidor_atual'];
			$arDados['nu_matricula_siape_novo'] = $res['nu_matricula_siape_novo'];
			$arDados['no_servidor_novo'] = $res['no_servidor_novo'];
			$arDados['tmvdsc'] = $res['tmvdsc'];
			$arDados['mvbobservacao'] = $res['mvbobservacao'];

			$arDados = $arDados ? $arDados : array();
		}else{
			$arDados = array();
		}
		return $arDados;
	}*/

	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaMovimentacaoGeral
     * @author Silas Matheus
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array;
     */
	/*public function cabecalhoVisualizaMovimentacaoGeral($mvbid){

		if(!empty($mvbid)){

			$arDados = $res = $this->pegarMovimentacao($mvbid);
			$arDados = $arDados ? $arDados : array();

		}else{
			$arDados = array();
		}
		return $arDados;
	}*/
	
	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaMovimentacaoGeral
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array
     */
	public function cabecalhoVisualizaMovimentacaoGeral($mvbid){
		if(!empty($mvbid)){

			$mvbid = pg_escape_string($mvbid);
			
			$sql = "SELECT tipo.tmvdsc ||
					CASE WHEN mov.mvbtipo='G' THEN ' Geral'
					ELSE
						CASE WHEN mov.mvbtipo='S' THEN ' por Siape'
						ELSE
							CASE WHEN mov.mvbtipo='L' THEN ' por Localidade'
							END
						END
					END as tipo,
			TO_CHAR(mov.mvbdata,'DD/MM/YYYY') AS mvbdata,
			uniatual.uorno AS uornoatual,
			endeatual.endlog AS endlogatual,
			endeatual.endcom AS endcomatual,
			andaratual.enadescricao AS enadescricaoatual,
			'Sala '||salaatual.easdescricao AS salaatual,
			uniantigo.uorno AS uornoantigo,
			endeantigo.endlog AS endlogantigo,
			endeantigo.endcom AS endcomantigo,
			andarantigo.enadescricao AS enadescricaoantigo,
			'Sala '||salaantigo.easdescricao AS salaantigo,
			respatual.no_servidor AS no_servidoratual,
			respatual.nu_matricula_siape AS nu_matricula_siapeatual,
			respantigo.no_servidor AS no_servidorantigo,
			respantigo.nu_matricula_siape AS nu_matricula_siapeantigo,
			mov.mvbobservacao,
			mov.ternumanterior,
			mov.mvbtipo
			
			FROM sap.tipomovimentacao tipo
			JOIN sap.movimentacaobens mov ON tipo.tmvid=mov.tmvid
			LEFT JOIN siape.vwservidorativo respatual ON mov.nu_matricula_siape=respatual.nu_matricula_siape
			LEFT JOIN siorg.uorgendereco uorgendatual ON mov.uendid=uorgendatual.uendid
			LEFT JOIN siorg.enderecoandarsala salaatual ON uorgendatual.easid=salaatual.easid
			LEFT JOIN siorg.enderecoandar andaratual ON salaatual.enaid=andaratual.enaid
			LEFT JOIN siorg.endereco endeatual ON andaratual.endid=endeatual.endid
			LEFT JOIN siorg.unidadeorganizacional uniatual ON uorgendatual.uorco_uorg_lotacao_servidor=uniatual.uorco_uorg_lotacao_servidor
			LEFT JOIN sap.vwrgps rgps ON mov.mvbid=rgps.mvbid
			LEFT JOIN siorg.uorgendereco uorgendantigo ON rgps.uendid_anterior=uorgendantigo.uendid
			LEFT JOIN siorg.enderecoandarsala salaantigo ON uorgendantigo.easid=salaantigo.easid
			LEFT JOIN siorg.enderecoandar andarantigo ON salaantigo.enaid=andarantigo.enaid
			LEFT JOIN siorg.endereco endeantigo ON andarantigo.endid=endeantigo.endid
			LEFT JOIN siorg.unidadeorganizacional uniantigo ON uorgendantigo.uorco_uorg_lotacao_servidor=uniantigo.uorco_uorg_lotacao_servidor
			LEFT JOIN siape.vwservidorativo respantigo ON rgps.nu_matricula_siape_anterior=respantigo.nu_matricula_siape
			
			WHERE mov.mvbid='{$mvbid}' ";
			
			$arDados = $this->carregar($sql);
			
			$arDados = $arDados ? $arDados : array();

		}else{
			$arDados = array();
		}
		return $arDados;
	}
	
	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaMovimentacaoDevolucao
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array
     */
	public function cabecalhoVisualizaMovimentacaoDevolucao($mvbid){
		if(!empty($mvbid)){

			$mvbid = pg_escape_string($mvbid);
			
			$sql = "SELECT tipo.tmvdsc ||
					CASE WHEN mov.mvbtipo='G' THEN ' Geral'
					ELSE
						CASE WHEN mov.mvbtipo='S' THEN ' por Siape'
						ELSE
							CASE WHEN mov.mvbtipo='L' THEN ' por Localidade'
							END
						END
					END as tipo,
			TO_CHAR(mov.mvbdata,'DD/MM/YYYY') AS mvbdata,
			resp.no_servidor,
			resp.nu_matricula_siape,
			mov.mvbobservacao,
			respdevolucao.no_servidor AS nomerespdevolucao,
			respdevolucao.nu_matricula_siape AS matricularespdevolucao,
			resprecebimento.no_servidor AS nomeresprecebimento,
			resprecebimento.nu_matricula_siape AS matricularesprecebimento,
			mov.ternumanterior,
			mov.mvbtipo
			
			FROM sap.tipomovimentacao tipo
			JOIN sap.movimentacaobens mov ON tipo.tmvid=mov.tmvid
			LEFT JOIN siape.vwservidorativo resp ON mov.nu_matricula_siape=resp.nu_matricula_siape
			LEFT JOIN siape.vwservidorativo respdevolucao ON mov.mvbresponsaveldevolucaonumatriculasiape=respdevolucao.nu_matricula_siape
			LEFT JOIN siape.vwservidorativo resprecebimento ON mov.mvbresponsavelrecebimentonumatriculasiape=resprecebimento.nu_matricula_siape
			
			WHERE mov.mvbid='{$mvbid}' ";
			
			$arDados = $this->carregar($sql);
			
			$arDados = $arDados ? $arDados : array();

		}else{
			$arDados = array();
		}
		return $arDados;
	}

	/**
     * Carrega os registros filhos da movimentação para o relatório
     * @name listaVisualizaMovimentacaoBens
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return void;
     */
	/*public function listaVisualizaMovimentacaoBens($mvbid){
		$sql = array();
		if(!empty($mvbid)){

			$mvbid = addslashes($mvbid);

			$sql = "SELECT rgp.rgpnum||' ' AS matcodigo,material.matdsc,bemmaterial.bmtitmat||' ' AS bmtitmat,bemmaterial.bmtvlrunitario,";
			$sql .= "'<span style=\"display:none;\">' || bemmaterial.bmtgarantia || '</span>' || TO_CHAR(bemmaterial.bmtgarantia, 'DD/MM/YYYY') AS bmtgarantia ";
			$sql .= "FROM sap.material material ";
			$sql .= "JOIN sap.bensmaterial bemmaterial ON material.matid=bemmaterial.matid ";
			$sql .= "JOIN sap.rgp rgp ON bemmaterial.bmtid=rgp.bmtid ";
			$sql .= "JOIN sap.bensmovimentacaorgp bmr ON rgp.rgpid=bmr.rgpid ";
			$sql .= "WHERE bmr.mvbid='".$mvbid."' ";
		}
		$cabecalho = array('RGP','Descri&ccedil;&atilde;o do Material', 'Qtd. Entrada', 'Valor Unit&aacute;rio', 'Garantia');
		$this->monta_lista($sql,$cabecalho,20,10,false,'center','S','','',array('center','center','center','right','center'));
	}*/

	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaMovimentacaoSiape
     * @author Silas Matheus
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array;
     */
	/*public function cabecalhoVisualizaTermoSiape($mvbid){
		if(!empty($mvbid)){

			$res = $this->pegarMovimentacaoSiape($mvbid);
			$arDados['nu_matricula_siape_atual'] = $res['nu_matricula_siape_atual'];
			$arDados['no_servidor_atual'] = $res['no_servidor_atual'];
			$arDados['nu_matricula_siape_novo'] = $res['nu_matricula_siape_novo'];
			$arDados['no_servidor_novo'] = $res['no_servidor_novo'];
			$arDados['tmvdsc'] = $res['tmvdsc'];
			$arDados = array_merge($res, $arDados);

		}else{
			$arDados = array();
		}
		return $arDados;
	}*/

	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaTermoGeral
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array
     */
	public function cabecalhoVisualizaTermoGeral($mvbid){
		if(!empty($mvbid)){

			$mvbid = pg_escape_string($mvbid);
			
			$sql = "SELECT tipo.tmvdsc ||
					CASE WHEN mov.mvbtipo='G' THEN ' Geral'
					ELSE
						CASE WHEN mov.mvbtipo='S' THEN ' por Siape'
						ELSE
							CASE WHEN mov.mvbtipo='L' THEN ' por Localidade'
							END
						END
					END as tipo,
			TO_CHAR(mov.mvbdata,'DD/MM/YYYY') AS mvbdata,
			uniatual.uorno AS uornoatual,
			termo.ternum,
			endeatual.endlog AS endlogatual,
			endeatual.endcom AS endcomatual,
			andaratual.enadescricao AS enadescricaoatual,
			'Sala '||salaatual.easdescricao AS salaatual,
			uniantigo.uorno AS uornoantigo,
			endeantigo.endlog AS endlogantigo,
			endeantigo.endcom AS endcomantigo,
			andarantigo.enadescricao AS enadescricaoantigo,
			'Sala '||salaantigo.easdescricao AS salaantigo,
			TO_CHAR(termo.teremissaodt,'DD/MM/YYYY') AS teremissaodt,
			respatual.no_servidor AS no_servidoratual,
			respatual.nu_matricula_siape AS nu_matricula_siapeatual,
			respantigo.no_servidor AS no_servidorantigo,
			respantigo.nu_matricula_siape AS nu_matricula_siapeantigo,
			mov.mvbobservacao,
			mov.ternumanterior,
			mov.mvbtipo
			
			FROM sap.tipomovimentacao tipo
			JOIN sap.movimentacaobens mov ON tipo.tmvid=mov.tmvid
			JOIN sap.termo termo ON mov.ternum=termo.ternum
			LEFT JOIN siape.vwservidorativo respatual ON mov.nu_matricula_siape=respatual.nu_matricula_siape
			LEFT JOIN siorg.uorgendereco uorgendatual ON mov.uendid=uorgendatual.uendid
			LEFT JOIN siorg.enderecoandarsala salaatual ON uorgendatual.easid=salaatual.easid
			LEFT JOIN siorg.enderecoandar andaratual ON salaatual.enaid=andaratual.enaid
			LEFT JOIN siorg.endereco endeatual ON andaratual.endid=endeatual.endid
			LEFT JOIN siorg.unidadeorganizacional uniatual ON uorgendatual.uorco_uorg_lotacao_servidor=uniatual.uorco_uorg_lotacao_servidor
			LEFT JOIN sap.vwrgps rgps ON mov.mvbid=rgps.mvbid
			LEFT JOIN siorg.uorgendereco uorgendantigo ON rgps.uendid_anterior=uorgendantigo.uendid
			LEFT JOIN siorg.enderecoandarsala salaantigo ON uorgendantigo.easid=salaantigo.easid
			LEFT JOIN siorg.enderecoandar andarantigo ON salaantigo.enaid=andarantigo.enaid
			LEFT JOIN siorg.endereco endeantigo ON andarantigo.endid=endeantigo.endid
			LEFT JOIN siorg.unidadeorganizacional uniantigo ON uorgendantigo.uorco_uorg_lotacao_servidor=uniantigo.uorco_uorg_lotacao_servidor
			LEFT JOIN siape.vwservidorativo respantigo ON rgps.nu_matricula_siape_anterior=respantigo.nu_matricula_siape
			
			WHERE mov.mvbid='{$mvbid}' ";
			
			$arDados = $this->carregar($sql);
			
			$arDados = $arDados ? $arDados : array();

		}else{
			$arDados = array();
		}
		return $arDados;
	}
	
	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaTermoDevolucao
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array
     */
	public function cabecalhoVisualizaTermoDevolucao($mvbid){
		if(!empty($mvbid)){

			$mvbid = pg_escape_string($mvbid);
			
			$sql = "SELECT tipo.tmvdsc ||
					CASE WHEN mov.mvbtipo='G' THEN ' Geral'
					ELSE
						CASE WHEN mov.mvbtipo='S' THEN ' por Siape'
						ELSE
							CASE WHEN mov.mvbtipo='L' THEN ' por Localidade'
							END
						END
					END as tipo,
			TO_CHAR(mov.mvbdata,'DD/MM/YYYY') AS mvbdata,
			termo.ternum,
			TO_CHAR(termo.teremissaodt,'DD/MM/YYYY') AS teremissaodt,
			resp.no_servidor,
			resp.nu_matricula_siape,
			mov.mvbobservacao,
			respdevolucao.no_servidor AS nomerespdevolucao,
			respdevolucao.nu_matricula_siape AS matricularespdevolucao,
			resprecebimento.no_servidor AS nomeresprecebimento,
			resprecebimento.nu_matricula_siape AS matricularesprecebimento,
			mov.ternumanterior,
			mov.mvbtipo
			
			FROM sap.tipomovimentacao tipo
			JOIN sap.movimentacaobens mov ON tipo.tmvid=mov.tmvid
			JOIN sap.termo termo ON mov.ternum=termo.ternum
			LEFT JOIN siape.vwservidorativo resp ON mov.nu_matricula_siape=resp.nu_matricula_siape
			LEFT JOIN siape.vwservidorativo respdevolucao ON mov.mvbresponsaveldevolucaonumatriculasiape=respdevolucao.nu_matricula_siape
			LEFT JOIN siape.vwservidorativo resprecebimento ON mov.mvbresponsavelrecebimentonumatriculasiape=resprecebimento.nu_matricula_siape
			
			WHERE mov.mvbid='{$mvbid}' ";
			
			$arDados = $this->carregar($sql);
			
			$arDados = $arDados ? $arDados : array();

		}else{
			$arDados = array();
		}
		return $arDados;
	}


	/**
     * Carrega os registros para montar o cabeçalho do termo
     * @name cabecalhoVisualizaTermoLocalidade
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array;
     */
	/*public function cabecalhoVisualizaTermoLocalidade($mvbid){
		if(!empty($mvbid)){
			$sql = "SELECT mov.ternum,
		                   endereco.*,
		                   endandar.enadescricao,
		                   endandarsala.easdescricao
		                   FROM sap.movimentacaobens mov
		                   JOIN siorg.uorgendereco uorgend ON mov.uendid=uorgend.uendid
		                   JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
			               JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
			               JOIN siorg.endereco ON endandar.endid=endereco.endid
			               WHERE mov.mvbid='".$mvbid."' ";
			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
		}else{
			$arDados = array();
		}
		return $arDados;
	}*/


	/**
     * Carrega os registros para montar o corpo do termo
     * @name listaVisualizaTermo
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Id do registro a ser pesquisado
     * @return array
     */
	/*public function listaVisualizaTermo($mvbid){
		$sql = array();
		if(!empty($mvbid)){
			$sql = "SELECT rgp.rgpid||' ' AS rgpid,
		                   rgp.rgpnum||' ' AS rgpnum,
		                   rgp.rgpnumserie,
		                   material.matdsc,
		                   bemmaterial.bmtvlrunitario,
		                   estado.ecodsc,
		                   s1.no_servidor
		                   FROM sap.rgp rgp
		                   JOIN sap.bensmovimentacaorgp bmr ON rgp.rgpid=bmr.rgpid
		                   JOIN sap.movimentacaobens mov ON bmr.mvbid=mov.mvbid
		                   JOIN sap.bensmaterial bemmaterial ON rgp.bmtid=bemmaterial.bmtid
		                   JOIN sap.material material ON material.matid=bemmaterial.matid
		                   JOIN sap.estadomotivos estadomotivo ON rgp.esmid=estadomotivo.esmid
		                   JOIN sap.estadoconservacao estado ON estadomotivo.ecoid=estado.ecoid
		                   JOIN sap.vwrgps ON bmr.mvbid=vwrgps.mvbid AND rgp.rgpid=vwrgps.rgpid AND material.matid=vwrgps.matid AND mov.mvbid=vwrgps.mvbid 
		                   JOIN siape.vwservidorativo s1 on s1.nu_matricula_siape=vwrgps.nu_matricula_siape_anterior
		                   WHERE mov.mvbid='".$mvbid."' ";
		}
		
		$cabecalho = array('Item','RGP', 'Número Série', 'Descrição do Bem', 'Valor Histórico', 'E.C.*', '
		Responsável');
		$this->monta_lista($sql,$cabecalho,9000,10,false,'center','S','','',array('center','center','center','center','right','center','center'));
	}*/


	/**
     * Lista as movimentações de acordo com o filtro
     * @name listarMovimentacaoPorFiltro
     * @author Alysson Rafael
     * @access public
     * @param array $_POST - Array de dados do form
     * @param string $filtroBenStatus - Indica se será para filtrar os registros em aberto ou os fechados
     * @return void
     */
	public function listarMovimentacaoPorFiltro($post,$filtroBenStatus){

		extract($post);

		$where = " WHERE 1=1 ";
		if(!empty($tmvid)){
			$tmvid = addslashes($tmvid);
			$where .= " AND mov.tmvid='".$tmvid."' ";
		}
		if(!empty($ternum)){
			$ternum = addslashes($ternum);
			$where .= " AND mov.ternum='".$ternum."' ";
		}
		if(!empty($nu_matricula_siape)){
			$nu_matricula_siape = addslashes($nu_matricula_siape);
			$where .= " AND mov.nu_matricula_siape='".$nu_matricula_siape."' ";
		}
		if(!empty($uorco_uorg_lotacao_servidor) && empty($enduf)){
			$uorco_uorg_lotacao_servidor = addslashes($uorco_uorg_lotacao_servidor);
			$where .= " AND mov.uendid IN(SELECT uendid FROM siorg.uorgendereco WHERE uorco_uorg_lotacao_servidor='{$uorco_uorg_lotacao_servidor}') ";
		}
		if(!empty($uendid) && !empty($enduf)){
			$uendid = addslashes($uendid);
			$where .= " AND mov.uendid='".$uendid."' ";
		}
		if(!empty($data_de)){
			$data_de = addslashes($data_de);
			$data_de = formata_data_sql($data_de);
			$where .= " AND mov.mvbdata >= '".$data_de."' ";
		}
		if(!empty($data_ate)){
			$data_ate = addslashes($data_ate);
			$data_ate = formata_data_sql($data_ate);
			$where .= " AND mov.mvbdata <= '".$data_ate."' ";
		}
		if(!empty($rgpnum)){
			$rgpnum = addslashes($rgpnum);
			$where .= " AND rgp.rgpnum='".$rgpnum."' ";
		}

		$sql = "SELECT
					'<center>'||
						CASE WHEN
							(mov.mvbstatus = 'I')
						THEN
							'<a style=\"cursor:pointer;\" onclick=\"detalharMovimentacaoEmAberto(\''||mov.mvbid||'\',\''||mov.mvbtipo||'\');\">
								<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
							</a>
							<a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Todos os dados serão perdidos. Confirma a exclusão do processo aberto de movimentação de bens?\')){removerMovimentacao(\''||mov.mvbid||'\');}\">
								<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
							</a>'
						ELSE
							'<a style=\"cursor:pointer;\" onclick=\"detalharMovimentacao(\''||mov.mvbid||'\',\''||mov.mvbtipo||'\');\">
								<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
							</a>'
						END
					||'</center>' as acao,


					tipo.tmvdsc ||
						CASE WHEN
							mov.mvbtipo='G' THEN ' Geral'
						ELSE
							CASE WHEN mov.mvbtipo='S' THEN ' por Siape'
							ELSE
								CASE WHEN mov.mvbtipo='L' THEN ' por Localidade'
								END
							END
						END as tipo,

					mov.ternum||' ' AS ternum,
					responsavel.nu_matricula_siape ||' - '|| responsavel.no_servidor as resp,
					unidade.uorno,
					endereco.enduf ||', '|| endcid ||', '|| endlog ||', '|| endandar.enadescricao ||', '|| endandarsala.easdescricao AS endereco,
					'<span style=\"display:none;\">' || mov.mvbdata || '</span>' || TO_CHAR(mov.mvbdata, 'DD/MM/YYYY') AS datamov

                    FROM
                    	 sap.movimentacaobens mov
                    JOIN sap.tipomovimentacao tipo ON mov.tmvid=tipo.tmvid
                    LEFT JOIN sap.bensmovimentacaorgp bmr ON mov.mvbid=bmr.mvbid
                    LEFT JOIN sap.rgp rgp ON bmr.rgpid=rgp.rgpid
                    LEFT JOIN siape.vwservidorativo responsavel ON mov.nu_matricula_siape=responsavel.nu_matricula_siape
                    LEFT JOIN siorg.uorgendereco uorgend ON mov.uendid=uorgend.uendid
                    LEFT JOIN siorg.enderecoandarsala endandarsala ON uorgend.easid=endandarsala.easid
                    LEFT JOIN siorg.enderecoandar endandar ON endandarsala.enaid=endandar.enaid
                    LEFT JOIN siorg.endereco endereco ON endandar.endid=endereco.endid
                    LEFT JOIN siorg.unidadeorganizacional unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor ";

		$sql .= $where." AND mov.mvbstatus='$filtroBenStatus' GROUP BY mov.mvbid,mov.mvbtipo,mov.mvbstatus,tipo,ternum,resp,unidade.uorno,endereco,datamov,mov.mvbdata ORDER BY mov.mvbdata ";

		$cabecalho = array("Comando","Tipo","Número do Termo","Responsável","Unidade","Endereço","Data");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center','center','center','center'));

	}





	public function pegaTipoStatusMovimentacaoAnterior($mvbid,$rgpid){
		$sql = "SELECT mov.mvbid,mov.tmvid,mov.mvbstatus FROM $this->stNomeTabela mov ";
		$sql .= "JOIN sap.bensmovimentacaorgp bmr ON mov.mvbid=bmr.mvbid ";
		$sql .= "WHERE mov.mvbid<>'{$mvbid}' AND bmr.rgpid='{$rgpid}' ORDER BY mvbdata DESC ";

		$result = $this->carregar($sql);
		return $result[0];
	}


	/**
     * Lista as contas contábeis 
     * @name listaContaContabilBem
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Identificador da movimentação
     * @return array
     */
	public function listaContaContabilBem($mvbid){
		
		if(!empty($mvbid)){
			$mvbid = pg_escape_string($mvbid);
			
			$sql = "SELECT conta.ccbid,conta.ccbdsc
					FROM sap.contacontabil conta
					JOIN sap.itemcontacontabil item ON conta.ccbid=item.ccbid
					JOIN sap.catalogo cat ON item.icbid=cat.icbid AND item.ccbid=cat.ccbid
					JOIN sap.material mat ON cat.catid=mat.catid
					JOIN sap.bensmaterial bemmat ON mat.matid=bemmat.matid
					JOIN sap.rgp rgp ON bemmat.bmtid=rgp.bmtid
					JOIN sap.bensmovimentacaorgp movrgp ON rgp.rgpid=movrgp.rgpid
					JOIN sap.movimentacaobens mov ON movrgp.mvbid=mov.mvbid
					WHERE mov.mvbid='{$mvbid}'
					GROUP BY conta.ccbid,conta.ccbdsc
					ORDER BY conta.ccbid ";
		
			return $this->carregar($sql);
		}
		
		
	}
	
	/**
     * Pega os rgps para o termo 
     * @name pegaDadosVisualizaTermo
     * @author Alysson Rafael
     * @access public
     * @param int $mvbid - Identificador da movimentação
     * @param int $ccbid - Identificador da conta contábil
     * @return array
     */
	public function pegaDadosVisualizaTermo($mvbid,$ccbid){
		
		if(!empty($mvbid) && !empty($ccbid)){
			$mvbid = addslashes($mvbid);
			$ccbid = addslashes($ccbid);
                        $dataHojeSql = formata_data_sql(date("d/m/Y"));

			$sql = "SELECT rgp.rgpid,
                                    rgp.rgpnum
                                    || ' ' AS rgpnum,
                                    rgp.rgpnumserie
                                    || ' ' AS rgpnumserie,
                                    bmt.bmtdscit,
                                    resp.no_servidor,
                                    bmt.bmtvlrunitario,
                                    x.vdepreciacaoacumulada AS valor_depreciado
                                FROM   sap.rgp rgp
                                    INNER JOIN sap.fn_relatorio_depreciacao_mensal('{$dataHojeSql}') x
                                            ON rgp.rgpnum = x.rgpnum
                                    JOIN sap.bensmovimentacaorgp movrgp
                                        ON rgp.rgpid = movrgp.rgpid
                                    JOIN sap.movimentacaobens mov
                                        ON movrgp.mvbid = mov.mvbid
                                    JOIN sap.bensmaterial bmt
                                        ON rgp.bmtid = bmt.bmtid
                                    JOIN sap.material mat
                                        ON bmt.matid = mat.matid
                                    JOIN sap.catalogo cat
                                        ON mat.catid = cat.catid
                                    JOIN sap.itemcontacontabil item
                                        ON cat.icbid = item.icbid
                                            AND cat.ccbid = item.ccbid
                                    JOIN sap.contacontabil conta
                                        ON item.ccbid = conta.ccbid
                                    JOIN sap.vwrgps vw
                                        ON rgp.rgpid = vw.rgpid
                                    LEFT JOIN siape.vwservidorativo resp
                                        ON vw.nu_matricula_siape_anterior = resp.nu_matricula_siape
                                WHERE mov.mvbid='{$mvbid}' 
                                        AND conta.ccbid='{$ccbid}'
                                        AND vw.mvbid='{$mvbid}'
                                ORDER BY rgp.rgpnum ";
			return $this->carregar($sql);
		}
		
		
	}
	
	public function pegaDadosMovimentacao($mvbid){
		$sql = "SELECT TO_CHAR(mov.mvbdata,'DD/MM/YYYY') as mvbdata
				FROM $this->stNomeTabela mov
				WHERE mov.mvbid='{$mvbid}' ";
		
		return $this->pegaLinha($sql);
	}



}