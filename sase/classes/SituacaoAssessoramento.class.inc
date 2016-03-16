<?php

/**
 * Classe Situação de Assessoramento
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de Situação de Assessoramento
 */
class SituacaoAssessoramento extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.situacaoassessoramento";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('stacod');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'stacod' => null,
		'stadsc' => null,
		'stacor' => null,
		'stastatus' => null,
		'esdid' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'stadsc'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'stacod',
		'stadsc',
		'stacor',
		'stastatus',
		'esdid'
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'stadsc',
		'stacor',
		'stastatus'
	);

	/**
	 * Monta query para a função monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 * @return $sql
	 */
	public function montaListaQuery( $filtros = false ){
		$sql = "
			SELECT 
				'<a style=\"cursor:pointer\" onclick=\"editarSituacaoAssessoramento( ' || stacod || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirSituacaoAssessoramento( ' || stacod || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				stadsc,
				stacor,
				CASE WHEN stastatus = 'I' THEN 'Inativo' WHEN stastatus = 'A' THEN 'Ativo' END AS stastatus
			FROM " . $this->stNomeTabela . " 
			WHERE stastatus = 'A' ";
		if( $filtros != false ){
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}
		$sql .= "             ORDER BY stacod ASC "; // não retirar os espaços!

		return $sql;
	}

	/**
	 * Atualiza Situacao de Assessoramento
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarSituacaoassessoramento(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente a situacao de assessoramento
 	 * 
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirSituacaoAssessoramento( $stacod ){
		$this->carregarPorId( $stacod );
		$this->arAtributos['stastatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaSituacaoAssessoramento( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;

		return true;
	}

	/**
	 * Cadastra Situacao Assessoramento Montada no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarSituacaoAssessoramento(){
		if( $this->validaCamposObrigatorios() ){
			$this->stacod = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
	 *
	 * @author Sávio Resende
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegenda( Array $estuf ){
		global $db;
		?>
		<div id="legendaMapaContainer">
		<div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
			<ul>
				<?php
				$sql = "
					SELECT 
						s.stacod,
						s.stadsc,
						s.stacor,
						count(a.assid) as total 
					FROM sase.situacaoassessoramento s
					LEFT JOIN sase.assessoramento a on a.stacod = s.stacod
					WHERE stastatus = 'A'
					GROUP BY 1,2,3
					ORDER BY stacod ASC ";
				$lista = $db->carregar( $sql );
				foreach ($lista as $key => $value) {

					$sql = "
						SELECT count(d.docid) AS total
						FROM workflow.documento d
						INNER JOIN sase.situacaoassessoramento s ON d.esdid = s.esdid AND s.stacod = " . $value['stacod'] . "
						INNER JOIN sase.assessoramento a ON a.docid = d.docid
						" . (($estuf!=''&&count($estuf)>0)?" WHERE a.estuf in ( '". (implode( "','", $estuf )) ."' ) ":"") . " ";
					$total = $db->pegaUm( $sql );

					echo "<li ><table><tr><td><span style='background:" . $value['stacor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($total!='')?$total:'0') . "</b>&nbsp;&nbsp;</td><td>" . $value['stadsc'] . "</td></tr></table></li>";
				}
				?>
			</ul>
		</div>
		<?php
	}

    /**
     * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
     *
     * @author Sávio Resende
     * @param array $estuf
     * @return html
     */
    public function montaLegendaMunicipioExterno(){
        global $db;
        ?>
        <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
            <div id="tituloLegenda">
                <div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5></div>
                <div style="float:right;"></div>
                <div style="clear:both;height:1px;">&nbsp;</div>
            </div>
            <!--div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div-->
            <ul>
                <?php
                $sql = "
					SELECT
						s.stacod,
						s.stadsc,
						s.stacor,
						count(a.assid) as total
					FROM sase.situacaoassessoramento s
					LEFT JOIN sase.assessoramento a on a.stacod = s.stacod
					WHERE stastatus = 'A'
					GROUP BY 1,2,3
					ORDER BY stacod ASC ";
                $lista = $db->carregar( $sql );
                foreach ($lista as $key => $value) {

                    $sql = "
						SELECT count(d.docid) AS total
						FROM workflow.documento d
						INNER JOIN sase.situacaoassessoramento s ON d.esdid = s.esdid AND s.stacod = " . $value['stacod'] . "
						INNER JOIN sase.assessoramento a ON a.docid = d.docid ";
                    $total = $db->pegaUm( $sql );

                    echo "<li ><table><tr><td style=\"width: 47px;\"><span style='background:" . $value['stacor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($total!='')?$total:'0') . "</b>&nbsp;&nbsp;</td><td>" . $value['stadsc'] . "</td></tr></table></li>";
                }
                ?>
            </ul>
        </div>
    <?php
    }

	/**
	 * Monta legenda de acordo com os estados selecionados
	 *
	 * @author Victor Martins Machado
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegendaEstado( Array $estuf ){
		global $db;
		?>
			<div id="legendaMapaContainer">
			<div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
				<ul>
					<?php
					$sql = "
						SELECT 
							s.stacod,
							s.stadsc,
							s.stacor,
							count(a.aseid) as total 
						FROM sase.situacaoassessoramento s
						LEFT JOIN sase.assessoramentoestado a on a.stacod = s.stacod
						WHERE stastatus = 'A'
						GROUP BY 1,2,3
						ORDER BY stacod ASC ";
					$lista = $db->carregar( $sql );
					foreach ($lista as $key => $value) {
	
						$sql = "
							SELECT count(d.docid) AS total
							FROM workflow.documento d
							INNER JOIN sase.situacaoassessoramento s ON d.esdid = s.esdid AND s.stacod = " . $value['stacod'] . "
							INNER JOIN sase.assessoramentoestado a ON a.docid = d.docid
							" . (($estuf!=''&&count($estuf)>0)?" WHERE a.estuf in ( '". (implode( "','", $estuf )) ."' ) ":"") . " ";
						$total = $db->pegaUm( $sql );
	
						echo "<li ><table><tr><td><span style='background:" . $value['stacor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($value['total']!='')?$value['total']:'0') . "</b>&nbsp;&nbsp;</td><td>" . $value['stadsc'] . "</td></tr></table></li>";
					}
					?>
				</ul>
			</div>
			<?php
		}
	
	/**
	 * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação - macrocategoria
	 *
	 * @author Sávio Resende
	 * @param array $estuf
	 * @return html
	 * @todo otimizar atraves da query
	 */
	public function montaLegendaMacroCategoria( Array $estuf ){
		global $db;
		?>
		<div id="chamadaLegenda" style="display:none"><a onclick="jQuery('#legendaConteudo').css({display:'block'});jQuery('#chamadaLegenda').css({display:'none'})" title="Mostrar Legenda"><img src="../imagens/sase/menu.png" style="width:15px;margin-top:4px;" /></a></div>
		<div id="legendaConteudo" style="display:block;width:150px;" class="topo_bootstrap_off">
			<div id="tituloLegenda">
				<div style="float:left;"><h5>&nbsp;Legenda:</h5></div>
				<div style="float:right;"><a onclick="jQuery('#legendaConteudo').css({display:'none'});jQuery('#chamadaLegenda').css({display:'block'})" title="Fechar Legenda"><img style="width:19px;"  src="../imagens/sase/sair.png" /></a></div>
				<div style="clear:both;height:1px;">&nbsp;</div>
			</div>
			<ul>
				<?php
				$sql = "
					select 
						--m.maccod,
						--m.macdsc,
						--m.maccor,
						s.stacod as maccod,
						s.stadsc as macdsc,
						s.stacor as maccor,
						s.stacod
					from sase.macrocategoria m
					left join sase.situacaoassessoramento s on s.maccod = m.maccod
					left join sase.assessoramento a on a.stacod = s.stacod
					group by 1,2,3,4
					order by stacod ASC ";
				$lista = $db->carregar( $sql );
				// ver($lista,d);

				$arrayApresentacao = array();
				$rotacao = array();
				foreach ($lista as $key => $value) {

					// dbg($_POST['params']['estados'],d);
					$sql = "
						select s.stacod, count(a.assid) as total
						from workflow.documento d
						inner join sase.assessoramento a on a.docid = d.docid
						inner join sase.situacaoassessoramento s on s.esdid = d.esdid and s.stacod = " . ( ($value['stacod'])?$value['stacod']:0 ) . "
						inner join sase.macrocategoria m on m.maccod = s.maccod
						where 
							" . (($estuf!=''&&count($estuf)>0)?"a.estuf in ( '". (implode( "','", $estuf )) ."' ) ":"") . " 
						group by 1 ";
					// exit($sql);
					$total = $db->carregar( $sql );

					// dbg($value['stacod']);
					if( array_search($value['maccod'],$rotacao) === false ){
						array_push( $arrayApresentacao, array('maccor' => $value['maccor'],'macdsc'=>$value['macdsc'],'total'=>$total[0]['total']));
						array_push($rotacao, $value['maccod']);
					}else{
						$arrayApresentacao[count($arrayApresentacao)-1]['total'] = $arrayApresentacao[count($arrayApresentacao)-1]['total'] + $total[0]['total'];
					}
				}
				// ver($arrayApresentacao,d);

				foreach ($arrayApresentacao as $key => $value) {
					echo "<li ><table><tr><td><span style='background:" . $value['maccor'] . ";' class='elementoCor'>&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($value['total']!='')?$value['total']:'0') . "</b>&nbsp;&nbsp;</td><td class='dsc'>" . $value['macdsc'] . "</td></tr></table></li>";
				}
				?>
			</ul>
			<div style="font-size:10px;font-color:;padding-left:10px;">É possível <b>arrastar</b> e fazer <b>zoom</b>.</div>
		</div>
		<?php
	}

	/**
	 * Monta legenda para o Brasil com a situação - macrocategoria
	 *
	 * @author Sávio Resende
	 * @return html
	 * @todo otimizar atraves da query
	 */
	public function montaLegendaMacroCategoriaPais(){
		global $db;
		?>
		<!-- <div id="chamadaLegenda" style="display:none;float:right;"><a onclick="jQuery('#legendaConteudo').css({display:'block'});jQuery('#chamadaLegenda').css({display:'none'})" title="Mostrar Legenda"><img src="../imagens/sase/menu.png" style="width:15px;margin-top:4px;" /></a></div> -->
		<div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
			<div id="tituloLegenda">
				<div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5></div>
				<div style="float:right;"></div>
				<div style="clear:both;height:1px;">&nbsp;</div>
			</div>
			<ul>
				<?php
				$sql = "
						SELECT 
							s.stacod,
							s.stadsc,
							s.stacor,
							count(a.aseid) as total 
						FROM sase.situacaoassessoramento s
						left join sase.assessoramentoEstado a on a.stacod = s.stacod
						WHERE stastatus = 'A'
						GROUP BY 1,2,3
						ORDER BY stacod ASC ";
				$lista = $db->carregar( $sql );
				// ver($lista,d);

				$arrayApresentacao = array();
				$rotacao = array();
				foreach ($lista as $key => $value) {

					// dbg($_POST['params']['estados'],d);
					$sql = "
						select s.stacod, count(a.aseid) as total
						from sase.situacaoassessoramento s
						join sase.assessoramentoestado a on a.stacod = s.stacod
						where 
							s.stacod = " . ( ($value['stacod'])?$value['stacod']:0 ) . "
						group by 1 ";
					// exit($sql);
					//ver($sql);
					$total = $db->carregar( $sql );

					// dbg($value['stacod']);
					if( array_search($value['stacod'],$rotacao) === false ){
						array_push( $arrayApresentacao, array('stacor' => $value['stacor'],'stadsc'=>$value['stadsc'],'total'=>$total[0]['total']));
						array_push($rotacao, $value['stacod']);
					}else{
						$arrayApresentacao[count($arrayApresentacao)-1]['total'] = $arrayApresentacao[count($arrayApresentacao)-1]['total'] + $total[0]['total'];
					}
				}
				// ver($arrayApresentacao,d);

				foreach ($arrayApresentacao as $key => $value) {
					echo "<li ><table><tr><td><span style='background:" . $value['stacor'] . ";' class='elementoCor'>&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($value['total']!='')?$value['total']:'0') . "</b>&nbsp;&nbsp;</td><td class='dsc'>" . $value['stadsc'] . "</td></tr></table></li>";
				}
				?>
			</ul>
		</div>
	<?php }
}