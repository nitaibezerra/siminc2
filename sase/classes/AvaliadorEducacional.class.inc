<?php

require_once APPRAIZ . 'includes/library/simec/Listagem.php';
/**
 * Classe de Avaliador Educacional
 * 
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 * 
 * Objeto de Modelo de Avaliador Educacional
 */

class AvaliadorEducacional extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.avaliadoreducacional";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('aveid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
							  'aveid' => null,
							  'dsamatricula' => null,
							  'usucpf' => null,
							  'estuf' => null,
							  'taeid' => null,
							  'orvid' => null,
							  'lotid' => null,
							  'avenumdocindicacao' => null,
							  'avedataindicacao' => null,
							  'avenome' => null,
							  'aveapelido' => null,
							  'avesexo' => null,
							  'avedatanascimento' => null,
							  'avenumcpf' => null,
							  'aveidentidade' => null,
							  'aveorgaoexpedidor' => null,
							  'avedataexpedicao' => null,
							  'avelotacao' => null,
							  'avedtnascimento' => null,
							  'aveemail' => null,
							  'aveskype' => null,
							  'aveendereco' => null,
							  'muncod' => null,
							  'avecep' => null,
							  'avelogradouro' => null,
							  'avenumero' => null,
							  'avecomplemento' => null,
							  'avebairro' => null,
							  'avedddtelcomercial' => null,
							  'avetelcomercial' => null,
							  'avedddtelcelular' => null,
							  'avetelcelular' => null,
							  'avedddtelresidencial' => null,
							  'avetelresidencial' => null,
							  'avedeficiencia' => null,
							  'avequaltipodeficiencia' => null,
							  'avecargoqueaposentou' => null,
							  'avebanco' => null,
							  'aveagencia' => null,
							  'aveconta' => null,
							  'avedtinclusao' => null,
							  'avestatus' => null,
                              'avediretoria' => null
	);

	public $aveid;
	
	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributosStr = array(
			'avenome'
	);

	/**
	 * @name $arCampos
	 * @var array
	 * @access protected
	 * Campos da Tabela
	*/
	protected $arCampos = array(
							  'aveid',
							  'dsamatricula',
							  'usucpf',
							  'estuf',
							  'taeid',
							  'orvid',
							  'lotid',
							  'avenumdocindicacao',
							  'avedataindicacao',
							  'avenome',
							  'aveapelido',
							  'avesexo',
							  'avedatanascimento',
							  'avenumcpf',
							  'aveidentidade',
							  'aveorgaoexpedidor',
							  'avedataexpedicao',
							  'avelotacao',
							  'avedtnascimento',
							  'aveemail',
							  'aveskype',
							  'aveendereco',
							  'muncod',
							  'avecep',
							  'avelogradouro',
							  'avenumero',
							  'avecomplemento',
							  'avebairro',
							  'avedddtelcomercial',
							  'avetelcomercial',
							  'avedddtelcelular',
							  'avetelcelular',
							  'avedddtelresidencial',
							  'avetelresidencial',
							  'avedeficiencia',
							  'avequaltipodeficiencia',
							  'avecargoqueaposentou',
							  'avebanco',
							  'aveagencia',
							  'aveconta',
							  'avedtinclusao',
							  'avestatus',
                              'avediretoria'
				);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	*/
	protected $arAtributosObrigatorios = array(
							  'usucpf',
							  'estuf',
							  'taeid',
							  'orvid',
							  'avenome',
							  'avesexo',
							  'avedatanascimento',
							  'avenumcpf',
							  'aveidentidade',
							  'aveorgaoexpedidor',
							  'aveemail',
							  'aveskype',
							  'aveendereco',
							  'muncod',
							  'avecep',
							  'avebanco',
							  'aveagencia',
							  'aveconta'
				);

	/**
	 * Atualiza Situacao de Assessoramento
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizar(){
		if( $this->validaCamposObrigatorios() ){
			
			if ($this->arAtributos['avenumdocindicacao'] == ''){
				$this->arAtributos['avenumdocindicacao'] = null;
			}
			
			$tel = explode(' ', $this->arAtributos['avetelcomercial']);
			$this->arAtributos['avedddtelcomercial'] = str_replace('(','',str_replace(')','',$tel[0]));
			$this->arAtributos['avetelcomercial']    = $tel[1];

			$tel = explode(' ', $this->arAtributos['avetelcelular']);
			$this->arAtributos['avedddtelcelular'] = str_replace('(','',str_replace(')','',$tel[0]));
			$this->arAtributos['avetelcelular']    = $tel[1];

			$tel = explode(' ', $this->arAtributos['avetelresidencial']);
			$this->arAtributos['avedddtelresidencial'] = str_replace('(','',str_replace(')','',$tel[0]));
 			$this->arAtributos['avetelresidencial']    = $tel[1];
			 			
 			$this->alterar(array('dsamatricula'));
			if ($this->commit()){
				//ver($this->arAtributos['aveid'],d);
				//$this->cadastrarExperiencia($this->arAtributos['aveid']);
				return true;
			} else {
				$this->rollback();
				return false;
			}
		}
	
		return false;
	}
	
	/**
	 * Função que valida o CPF do avaliador
	 * @return multitype:number string boolean
	 */
	public function validaCpfAvaliador(){
		global $db;
        $avediretoria = $this->arAtributos['avediretoria'] != '' ? $this->arAtributos['avediretoria'] : $_REQUEST['avediretoria'];
		$ret = array(
				'cod' => 0,
				'msg' => '',
				'bool' => true
		);
		
		if (strlen($this->arAtributos['usucpf']) < 11){
			$ret = array(
					'cod' => 1,
					'msg' => 'Cpf inválido.',
					'bool' => false
			);
			return $ret;
		}
		
		$sql = "SELECT COUNT(*) as cont FROM sase.avaliadoreducacional WHERE avenumcpf = '{$this->arAtributos['avenumcpf']}' AND avediretoria = {$avediretoria}";
		
		$res = $db->carregar($sql);
		$res = $res[0];

		if ($res['cont'] > 0){
			$ret = array(
					'cod' => 2,
					'msg' => 'Avaliador Educacional já cadastrado.',
					'bool' => false
			);
			return $ret;
		}

        $sql2 = "SELECT COUNT(*) as cont FROM seguranca.usuario where usucpf = '{$this->arAtributos['avenumcpf']}'";

        $res2 = $db->carregar($sql2);
        $res2 = $res2[0];

        if ($res2['cont'] == 0){
            $ret = array(
                'cod' => 3,
                'msg' => 'Avaliador não está cadastrado como usuário.',
                'bool' => false
            );
            return $ret;
        }

		return $ret;
	}
	
	/**
	 * Exclui logicamente a situacao de assessoramento
	 *
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluir( $aveid ){
		$this->carregarPorId( $aveid );
		$this->arAtributos['aveid'] = $aveid;
		$this->arAtributos['avestatus'] = 'I';
		$this->alterar();
		if ($this->commit()){
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function popula( Array $arDados ){
		$this->arAtributos[$this->arChavePrimaria[0]] = $arDados[$this->arChavePrimaria[0]];
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}
	
	public function cadastraGraduacao($grdid){
		$sql = "INSERT INTO sase.itemgraduacao (aveid, grdid) VALUES ({$this->aveid}, {$grdid}) returning grdid";
		$res = $this->executar($sql);
		if ($this->commit()){
			return true;
		} else {
			return false;
		}
	}
	
	public function cadastraCurso(Array $dados){
		$sql = "INSERT INTO sase.itemcursopos (sbcid, aveid, icptipo) VALUES ({$dados['sbcid']}, {$this->aveid}, '{$dados['icptipo']}') returning icpid";
			$res = $this->executar($sql);
		if ($this->commit()){
			return true;
		} else {
			return false;
		}
	}
	
	public function apagaGraduacao($grdid){
		$sql = "DELETE FROM sase.itemgraduacao WHERE grdid = {$grdid} AND aveid = {$this->aveid}";
			$res = $this->executar($sql);
		if ($this->commit()){
			return true;
		} else {
			return false;
		}
	} 

	public function apagaCurso($icpid){
		$sql = "DELETE FROM sase.itemcursopos WHERE icpid = {$icpid} AND aveid = {$this->aveid}";
		$res = $this->executar($sql);
		if ($this->commit()){
			return true;
		} else {
			return false;
		}
	}
		
	public function montarTabelaGraduacao($dados = array()) {

		?>
			<script type="text/javascript">
				jQuery(document).ready(function(){
				    jQuery.expr[':'].contains = function(a, i, m) {
			    		console.log(jQuery(a).text());
				        return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
				    };
				
				    $("#textFind").keyup(function()
				    {
				        $('#listagem table.table tbody tr td').removeClass('marcado');
				        $('#listagem table.table tbody tr').removeClass('remover');
				        stringPesquisa = $("#textFind").val();
				        if (stringPesquisa) {
				            $('#listagem table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
				            $('#listagem table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
				        }
				    });
				});
			</script>
			<div id="listagem" style="margin-top: 20px; background: #ffffff !important;">
			<?php
			
				$listagem = new Simec_Listagem();
				$listagem->setTamanhoPagina(50);
				//$listagem->addCallbackDeCampo ( array ('objnome', 'autor','esddsc'), 'alinhaParaEsquerda' );
		        //$listagem->addCallbackDeCampo ( array ('dmdmandatoseguranca' ), 'tratarDemandaJudicialLista' );
				$listagem->setCabecalho ( array (
						'Graduação'
				) );
				$listagem->esconderColunas (array('grdid'));
				/*$listagem->addAcao ( 'edit', array(
					'func' => 'editaExperiencia',
					'extra-params' => array (
							'eaeid'
					)
				) );*/
				$listagem->addAcao ( 'delete', array (
						'func' => 'apagaGraduacao',
						'extra-params' => array (
								'grdid' 
						) 
				) );
				$listagem->setDados ( $dados );
				$listagem->setFormOff();
				$listagem->render ();
			?>
			</div>
			<?php
	}
			
	public function listarGraduacao(){
		$sql = "SELECT
					grd.grdid,
					grd.grddsc
				FROM sase.itemgraduacao igrd
				INNER JOIN sase.graduacao grd
					ON (igrd.grdid = grd.grdid)
				WHERE igrd.aveid = {$this->aveid}";
		$dados = $this->carregar($sql);
		
		if ($dados){
			$this->montarTabelaGraduacao($dados);
		}
	}

	public function montarTabelaCurso($dados = array()) {
	
		?>
				<div id="listagem" style="margin-top: 20px; background: #ffffff !important;">
				<?php
				
					$listagem = new Simec_Listagem();
					$listagem->setTamanhoPagina(50);
					//$listagem->addCallbackDeCampo ( array ('objnome', 'autor','esddsc'), 'alinhaParaEsquerda' );
			        //$listagem->addCallbackDeCampo ( array ('dmdmandatoseguranca' ), 'tratarDemandaJudicialLista' );
					$listagem->setCabecalho ( array (
							'Sub-Área',
							'Tipo'
					) );
					$listagem->esconderColunas (array('icpid'));
					/*$listagem->addAcao ( 'edit', array(
						'func' => 'editaExperiencia',
						'extra-params' => array (
								'eaeid'
						)
					) );*/
					$listagem->addAcao ( 'delete', array (
							'func' => 'apagaCurso',
							'extra-params' => array (
									'icpid' 
							) 
					) );
					$listagem->setDados ( $dados );
					$listagem->setFormOff();
					$listagem->render ();
				?>
				</div>
				<?php
		}

		public function listarCurso(){
			$sql = "
					SELECT
						icp.icpid,
						sbc.sbcdsc,
						CASE 
							WHEN icp.icptipo = 'D' THEN 'Doutorado'
							WHEN icp.icptipo = 'E' THEN 'Especialização'
							WHEN icp.icptipo = 'M' THEN 'Mestrado'
						END AS icptipo
					FROM sase.itemcursopos icp
					INNER JOIN sase.subareacurso sbc
						ON (icp.sbcid = sbc.sbcid)
					WHERE icp.aveid = {$this->aveid}";
			$dados = $this->carregar($sql);
		
			if ($dados){
			$this->montarTabelaCurso($dados);
			}
		}
				
	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Victor Martins Machado
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributos as $chave => $valor)
		if( isset($this->arAtributosObrigatorios[$chave])) {
			if (empty($this->arAtributos[$chave]) ){
				ver($chave);
				return false;
			}
		}
	
		return true;
	}
	
	/**
	 * Cadastra Situacao Assessoramento Montada no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrar(){
		if( $this->validaCamposObrigatorios() ){
						
			$tel = explode(' ', $this->arAtributos['avetelcomercial']);
			$this->arAtributos['avedddtelcomercial'] = str_replace('(','',str_replace(')','',$tel[0]));
			$this->arAtributos['avetelcomercial']    = $tel[1];

			$tel = explode(' ', $this->arAtributos['avetelcelular']);
			$this->arAtributos['avedddtelcelular'] = str_replace('(','',str_replace(')','',$tel[0]));
			$this->arAtributos['avetelcelular']    = $tel[1];

			$tel = explode(' ', $this->arAtributos['avetelresidencial']);
			$this->arAtributos['avedddtelresidencial'] = str_replace('(','',str_replace(')','',$tel[0]));
 			$this->arAtributos['avetelresidencial']    = $tel[1];

			$this->aveid = $this->inserir(array('dsamatricula'));
			if ($this->commit()){
				//$this->cadastrarExperiencia($this->aveid);
				return true;
			} else {
				$this->rollback();
				return false;
			}
		}
		return false;
	}
	
	public function cadastrarExperiencia($aveid){
		$i = 1;
		$m = $_REQUEST['hidCont'];
		
		while ( $i <= $m ){
			if ($_REQUEST['eaeid'.$i] == ''){
				$experienciaavaliadoreducacional = new ExperienciaAvaliadorEducacional();
				$arAtributos = array(
						'eaeid' => null,
						'aveid' => $aveid,
						'eaeorgaoquetrabalha' => $_REQUEST['eaeorgaoquetrabalha'.$i],
						'eaediretoriacoordenacaosetor' => $_REQUEST['eaediretoriacoordenacaosetor'.$i],
						'eaecargo' => $_REQUEST['eaecargo'.$i],
						'eaefuncao' => $_REQUEST['eaefuncao'.$i],
						'eaecargoqueaposentou' => $_REQUEST['eaecargoqueaposentou'.$i]
				);
								
				$experienciaavaliadoreducacional = $experienciaavaliadoreducacional->popula($arAtributos);
				$experienciaavaliadoreducacional->cadastrar();
				$i++;
			} else {
				$experienciaavaliadoreducacional = new ExperienciaAvaliadorEducacional();
				$arAtributos = array(
						'eaeid' => $_REQUEST['eaeid'.$i],
						'aveid' => $aveid,
						'eaeorgaoquetrabalha' => $_REQUEST['eaeorgaoquetrabalha'.$i],
						'eaediretoriacoordenacaosetor' => $_REQUEST['eaediretoriacoordenacaosetor'.$i],
						'eaecargo' => $_REQUEST['eaecargo'.$i],
						'eaefuncao' => $_REQUEST['eaefuncao'.$i],
						'eaecargoqueaposentou' => $_REQUEST['eaecargoqueaposentou'.$i]
				);
				
				$experienciaavaliadoreducacional = $experienciaavaliadoreducacional->popula($arAtributos);
				$experienciaavaliadoreducacional->atualizar();
				$i++;
			}
		}
	}
	
	public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null, $funcao = null, $value = null) {
		$html = '';
		$selected = '';
		
		$html .= "<select class=\"form-control chosen\" id=\"{$idCampo}\" name=\"{$idCampo}\"";
		if ($funcao != null){
			$html .= "onchange=\"{$funcao}\"";
		}
		$html .= ">";
		
		if (isset ( $htmlOptions ['prompt'] )) {
			$html .= '<option value="">' . strtr ( $htmlOptions ['prompt'], array (
					'<' => '&lt;',
					'>' => '&gt;'
			) ) . "</option>\n";
		}
	
		if ($dados) {
			foreach ( $dados as $data ) {
				if ($idCampo) {
					$selected = ($data ['codigo'] === trim($this->arAtributos[$idCampo]) ? "selected='true' " : "");
				}
				if ($value != null){
					$selected = ($data ['codigo'] === $value ? "selected='true' " : "");
				}
				$html .= "<option {$selected}  title=\"{$data['descricao']}\" value= " . $data ['codigo'] . ">  " . simec_htmlentities ( $data ['descricao'] ) . " </option> ";
			}
		}
		
		$html .= '</select>';
		
		return $html;
	}
	
	public function getAreaCursos($funcao = null){
		$sql = 'SELECT arcid AS codigo, arcdsc AS descricao FROM sase.areacurso';
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), 'arcid', $funcao );
	}
	
	public function getSubAreaCursos($arcid = ''){
		$sql = $arcid != '' ? "SELECT sbcid AS codigo, sbcdsc AS descricao FROM sase.subareacurso WHERE arcid = {$arcid}" : "SELECT sbcid AS codigo, sbcdsc AS descricao FROM sase.subareacurso";
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), 'sbcid' );
	}
	
	public function getTipoCursos(){
		$dados = array (
				array (
						'codigo' => 'D',
						'descricao' => 'Doutorado'
				),
				array (
						'codigo' => 'E',
						'descricao' => 'Especialização'
				),
				array (
						'codigo' => 'M',
						'descricao' => 'Mestrado'
				)
		);
		
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), 'icptipo' );
	}
	
	public function getTipos($funcao = null, $value = null){
		$sql = 'SELECT taeid as codigo, taedsc as descricao  FROM sase.tipoavaleducacional';
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => 'Todos'), 'taeid', $funcao, $value );
	}
	
	public function getUfs($campo, $funcao = null, $value = null){
		$sql = 'SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado ORDER BY estuf';
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => 'Todos'), $campo, $funcao, $value );
	}
	
	public function getMunicipios($campo, $estuf='', $funcao = null, $value = null){
		$sql = $estuf != '' ? "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf = '{$estuf}'": 'SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio';
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), $campo, $funcao, $value );
	}
	
	public function getOrgaosVinculacao($funcao = null, $value = null){
		$sql = 'SELECT orvid as codigo, orvdsc as descricao FROM sase.orgaovinculado WHERE orvstatus = \'A\'';
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), 'orvid', $funcao, $value );
	}
	
	public function getGraduacoes(){
		$sql = 'SELECT grdid AS codigo, grddsc AS descricao FROM sase.graduacao';
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), 'grdid');
	}
	
	public function getLotacoes(){
		$sql = "SELECT lotid AS codigo, lotdsc AS descricao from sase.lotacao WHERE lotstatus = 'A'";
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), 'lotid');
	}
	
	public function gerComboAvaliadores(){
        $avediretoria = $this->arAtributos['avediretoria'] != '' ? $this->arAtributos['avediretoria'] : $_REQUEST['avediretoria'];
		$sql = "select aveid as codigo, avenome as descricao from sase.avaliadoreducacional where avestatus = 'A' and avediretoria = {$avediretoria}";
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		
		return $this->getOptions( $dados, array('prompt' => 'Todos'), 'aveid');
	}
	
	public function getStatus($value = null) {
		$dados = array (
				array (
						'codigo' => 'A',
						'descricao' => 'Ativo'
				),
				array (
						'codigo' => 'I',
						'descricao' => 'Inativo'
				)
		);
				
		return $this->getOptions( $dados, array('prompt' => ' Selecione...'), 'avestatus' );
	}
	
	public function getSexo(){
		$dados = array (
				array (
						'codigo' => 'M',
						'descricao' => 'Masculino'
				),
				array (
						'codigo' => 'F',
						'descricao' => 'Feminino'
				)
		);
		return $this->getOptions ( $dados, array (), 'avesexo' );
	}
	
	public function getSqlListagem($tipo = 'rel'){
		$sql = "SELECT ";
		$avediretoria = $this->arAtributos['avediretoria'] != '' ? $this->arAtributos['avediretoria'] : $_REQUEST['avediretoria'];

		if ($tipo == 'rel'){
			$sql .=    "ave.aveid,
						SUBSTR(ave.avenumcpf, 1, 3) || '.' || SUBSTR(ave.avenumcpf, 4, 3) || '.' || SUBSTR(ave.avenumcpf, 7, 3) || '-' || SUBSTR(ave.avenumcpf, 10) as cpf,
						ave.avenumcpf,
						upper(ave.avenome) avenome,
						'(' || ave.avedddtelcelular || ') ' || ave.avetelcelular as avetelcelular, 
						upper(ave.aveemail) aveemail, 
						ave.estuf, 
		        		tae.taeid,
						upper(tae.taedsc) taedsc,
						upper(orv.orvdsc) orvdsc";
		} else {
			$sql .=    "SUBSTR(ave.avenumcpf, 1, 3) || '.' || SUBSTR(ave.avenumcpf, 4, 3) || '.' || SUBSTR(ave.avenumcpf, 7, 3) || '-' || SUBSTR(ave.avenumcpf, 10) as cpf,
						upper(ave.avenome) avenome,
						'(' || ave.avedddtelcelular || ') ' || ave.avetelcelular as avetelcelular, 
						upper(ave.aveemail) aveemail,
						ave.estuf, 
						upper(tae.taedsc) taedsc,
						upper(orv.orvdsc) orvdsc";
		}
				        		
        $sql .= " FROM sase.avaliadoreducacional ave
				INNER JOIN sase.tipoavaleducacional tae
					ON (ave.taeid = tae.taeid)
				INNER JOIN sase.orgaovinculado orv
					ON (ave.orvid = orv.orvid)
                WHERE avediretoria = {$avediretoria}";
		
		return $sql;
	}
	
	public function montarTabelaListagem($dados = array()) {
		$pfls = arrayPerfil();
		?>
			<script type="text/javascript">
				jQuery(document).ready(function(){
				    jQuery.expr[':'].contains = function(a, i, m) {
			    		console.log(jQuery(a).text());
				        return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
				    };
				
				    $("#textFind").keyup(function()
				    {
				        $('#listagem table.table tbody tr td').removeClass('marcado');
				        $('#listagem table.table tbody tr').removeClass('remover');
				        stringPesquisa = $("#textFind").val();
				        if (stringPesquisa) {
				            $('#listagem table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
				            $('#listagem table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
				        }
				    });
				});
			</script>
			<div id="listagem">
			<?php
			
				$listagem = new Simec_Listagem();
				$listagem->setTamanhoPagina(50);
				//$listagem->addCallbackDeCampo ( array ('objnome', 'autor','esddsc'), 'alinhaParaEsquerda' );
		        //$listagem->addCallbackDeCampo ( array ('dmdmandatoseguranca' ), 'tratarDemandaJudicialLista' );
				$listagem->setCabecalho ( array (
						'CPF',
						'NOME',
						'CELULAR',
						'E-MAIL',
						'UF',
						'Perfil',
						'Órgão'
				) );
				$listagem->esconderColunas (array('aveid', 'taeid', 'avenumcpf'));
				$listagem->addAcao ( 'edit', array(
					'func' => 'listaAvaliadorEducacional',
					'extra-params' => array (
							'aveid'
					)
				) );

				if (in_array(PFLCOD_SASE_ADMINISTRADOR, $pfls) ||
					in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls)) {
					$listagem->addAcao ( 'delete', array (
							'func' => 'deleteAvaliadorEducacional',
							'extra-params' => array (
									'aveid'
							)
					) );
				}	
				
				$listagem->addAcao ( 'view', array(
						'func' => 'showresponsabilidade',
						'titulo' => 'Exibir vinculação atribuída.',
						'extra-params' => array(
							'avenumcpf',
							'taeid',
							'cpf',
							'avenome'
						)
				) );
				
				$listagem->addAcao ( 'download', array(
						'func' => 'geraFichaCadastral',
						'titulo' => 'Download da ficha cadastral.',
						'extra-params' => array (
								'aveid'
						)
				) );

				$listagem->setDados ( $dados );
				$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
				//$listagem->setFormOff();
				$listagem->render ();
			?>
			</div>
			<?php
	}
			
	public function recuperarListagem($tipo = 'rel'){
        global $db;
		$sql = $this->getSqlListagem($tipo);
		
		$where = array();
		
		if ($_POST ['acao'] == 'pesquisa') {
			$this->popularDadosObjeto();
			
			
			// Dados Pessoais
			if ( $this->avenome ) {
				$where [] = "ave.avenome ilike '%{$this->avenome}%'";
			}
			
			if ( $this->estuf ){
				$where [] = "ave.estuf = '{$this->estuf}'";
			}
			
			if ( $this->taeid ){
				$where [] = "ave.taeid = {$this->taeid}";
			}
			
			if ( $this->aveapelido ) {
				$where [] = "ave.aveapelido = '{$this->aveapelido}'";
			}
			
			if ( $this->avesexo ) {
				$where [] = "ave.avesexo = '{$this->avesexo}'";
			}
			
			if ( $this->avedatanascimento ) {
				$where [] = "to_char(ave.avedatanascimento, 'dd/mm/yyyy') = '{$this->avedatanascimento}'";
			}
			
			if ( $this->avenumcpf ) {
				$where [] = "ave.avenumcpf = '{$this->avenumcpf}'";
			}
			
			if ( $this->aveidentidade) {
				$where [] = "ave.aveidentidade = '{$this->aveidentidade}'";
			}
			
			if ( $this->aveorgaoexpedidor ) {
				$where [] = "ave.aveorgaoexpedidor = '{$this->aveorgaoexpedidor}'";
			}
			
			if ( $this->avedataexpedicao ) {
				$where [] = "to_char(ave.avedataexpedicao, 'dd/mm/yyyy') = '{$this->avedataexpedicao}'";
			}
			
			if ( trim($this->aveemail) ) {
				$where [] = "ave.aveemail ilike '%{$this->aveemail}%'";
			}
			
			if ( trim($this->aveskype) ) {
				$where [] = "ave.aveskype ilike '%{$this->aveskype}%'";
			}

			if ( $this->aveendereco ) {
				$where [] = "ave.aveendereco ilike '%{$this->aveendereco}%'";
			}

			if ( $this->muncod ) {
				$where [] = "ave.muncod = '{$this->muncod}'";
			}

			if ( $this->avecep ) {
				$where [] = "ave.avecep ilike '%{$this->avecep}%'";
			}

			if ( $this->avelogradouro ) {
				$where [] = "ave.avelogradouro ilike '%{$this->avelogradouro}%'";
			}

			if ( $this->avenumero ) {
				$where [] = "ave.avenumero ilike '%{$this->avenumero}%'";
			}

			if ( $this->avecomplemento ) {
				$where [] = "ave.avecomplemento ilike '%{$this->avecomplemento}%'";
			}

			if ( $this->avebairro ) {
				$where [] = "ave.avebairro ilike '%{$this->avebairro}%'";
			}

			if ( $this->avetelcomercial ) {
				$where [] = "ave.avebairro ilike '%{$this->avebairro}%'";
			}
			
			if ( $this->avedeficiencia ) {
				$where [] = "ave.avedeficiencia = '{$this->avedeficiencia}'";
			}
			
			if ( trim($this->avequaltipodeficiencia) ) {
				$where [] = "ave.avequaltipodeficiencia ilike '%{$this->avequaltipodeficiencia}%'";
			}

			if ( $this->avebanco ) {
				$where [] = "ave.avebanco ilike '%{$this->avebanco}%'";
			}
			
			if ( $this->lotid ){
				$where [] = "ave.lotid = {$this->lotid}";
			}
			
			$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

            if (in_array(PFLCOD_SASE_EXECUTIVO, $perfis)) {
                $sqlUf = "select regcod from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'";
                $uf = $db->pegaUm($sqlUf);
                $sql .= " AND  ave.estuf = '{$uf}'";
            } else {
                if (!in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) && !in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis)) {
                    $sql .= " AND ave.avenumcpf = '{$_SESSION['usucpf']}'";
                }
            }

			
			if ( $this->avestatus ) {
				$s = $this->avestatus;
			} else {
				$s = 'A';
			}
							
            $sql .= " AND ave.avestatus = '{$s}' ";

			if (! empty ( $where ) > 0) {
				$where = implode ( ' AND ', $where );
				// $sql .= implode(' ',join);
				$sql .= ' AND ' . $where;
			}
		} else {
			$sql .= " AND ave.avestatus = 'A' ";
			
			$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

            if (is_array($perfis)) {
                if (in_array(PFLCOD_SASE_EXECUTIVO, $perfis)) {
                    $sqlUf = "select regcod from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'";
                    $uf = $db->pegaUm($sqlUf);
                    $sql .= " AND  ave.estuf = '{$uf}'";
                } else {
                    if (!in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) && !in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis)) {
                        $sql .= " AND ave.avenumcpf = '{$_SESSION['usucpf']}'";
                    }
                }
            }
				
		}
		$sql .= ' ORDER BY ave.avenome';

		
		if ($tipo == 'xls'){
			$this->montaListaXls($sql);
		} else {
			$dados = $this->carregar ( $sql );
			$dados = $dados ? $dados : array ();
			$this->montarTabelaListagem ( $dados );
		}
	}
	
	public function montaListaXls($sql){
		global $db;
	
		$cabecalho = array("CPF","Nome","Perfil","Órgão");
		$alinhamento = array('left','left','left','left');
		$larguras = array('5%','85%','5%', '5%');
	
		ob_clean();
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=simec_sase_avaliadoreducacional_" . date("Ymdhis") . ".xls");
		header("Content-Disposition: attachment; filename=simec_sase_avaliadoreducacional_" . date("Ymdhis") . ".xls");
		header("Content-Description: MID Gera excel");
	
		$db->monta_lista($sql,$cabecalho,100000,5,'N','','N','listaAvaliadorEducacional',$larguras,$alinhamento);
		//$db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%', '');
	}
	
	public function montarTabelaArquivo($dados = array()) {

		?>
			<div id="listagem" style="margin-top: 20px; background: #ffffff !important;">
			<?php
			
				$listagem = new Simec_Listagem();
				$listagem->setTamanhoPagina(50);
				$listagem->setCabecalho ( array (
						'Arquivo'
				) );
				$listagem->addAcao ( 'download', array(
					'func' => 'baixarArquivo',
					'extra-params' => array (
							'arqid'
					)
				) );
				$listagem->addAcao ( 'delete', array(
					'func' => 'inativarArquivo',
					'extra-params' => array (
							'aidid'
					)
				) );
				$listagem->esconderColunas (array('aidid', 'arqid'));
				$listagem->setDados ( $dados );
				$listagem->setFormOff();
				$listagem->render ();
			?>
			</div>
			<?php
	}

	public function listarArquivo(){
		$sql = "
				SELECT
					aidid,
					arqid,
					aiddsc
				FROM sase.arquivoindicacao
				WHERE aveid = {$this->aveid}
				AND aidstatus = 'A'";
		
		$dados = $this->carregar($sql);
	
		if ($dados){
			$this->montarTabelaArquivo($dados);
		}
	}	
	
	public function inativarArquivo($aidid){
		$sql = "UPDATE sase.arquivoindicacao SET aidstatus = 'I' WHERE aidid = {$aidid}";
		$res = $this->executar($sql);
		if ($this->commit()){
			return true;
		} else {
			return false;
		}
	}
	
	public function listaPerfil($usucpf, $taeid){
		global $db;
		
		$sql = "select
					u.usucpf,
					u.usunome,
					pu.pflcod,
					p.pfldsc
				from  seguranca.usuario u
				inner join seguranca.perfilusuario pu on (u.usucpf = pu.usucpf)
				inner join seguranca.perfil p on (pu.pflcod = p.pflcod)
				inner join sase.tprperfil tpr on (p.pflcod = tpr.pflcod)
				where p.sisid = 183
				and u.usucpf = '{$usucpf}'";
		
		$dados = $db->carregar($sql);
		
		if (!is_array($dados)){
			$dados = array();
		}
		
		$listagem = new Simec_Listagem();
		$listagem->setTamanhoPagina(50);
		$listagem->addCallbackDeCampo(array('pfldsc'), 'alinhaParaEsquerda');
		$listagem->setCabecalho ( array (
				'Perfil'
		) );
		$listagem->addAcao ( 'plus', array(
				'func' => 'detalhaResponsabilidade',
				'extra-params' => array (
						'pflcod'
				)
		) );
		$listagem->esconderColunas (array('usucpf', 'usunome', 'pflcod'));
		$listagem->setDados ( $dados );
		$listagem->setFormOff();
		$listagem->render (SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
	}
	
	public function listaResponsabilidade($usucpf, $pflcod){
		global $db;
		
		$sqlResponsabilidadesPerfil = "SELECT tr.*
							   FROM sase.tprperfil p
							   INNER JOIN sase.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
							   WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
							   ORDER BY tr.tprdsc";
		
		$queryR = sprintf($sqlResponsabilidadesPerfil, $pflcod);
		
		$rp = $db->carregar($queryR);
		$rp = $rp[0];
		
		$sqlRespUsuario = "";
		switch ($rp["tprsigla"]) {
			case "C": // Estados
				$aca_prg = "Coordenador Local Associado";
				$sqlRespUsuario = "SELECT CASE WHEN m.muncod IS NOT NULL THEN m.muncod
											   WHEN e.estuf  IS NOT NULL THEN e.estuf END as codigo,
										  CASE WHEN m.muncod IS NOT NULL THEN m.estuf || ' - ' || m.mundescricao
											   WHEN e.estuf  IS NOT NULL THEN e.estuf || ' - ' || e.estdescricao END as descricao,
										  ur.rpustatus AS status
					FROM sase.usuarioresponsabilidade ur
					LEFT JOIN territorios.municipio m ON m.muncod = ur.muncod
					LEFT JOIN territorios.estado e ON e.estuf = ur.estuf
					WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			case "E": // Estados
				$aca_prg = "Estados Associados";
				$sqlRespUsuario = "SELECT e.estuf AS codigo, e.estdescricao AS descricao, ur.rpustatus AS status
					FROM sase.usuarioresponsabilidade ur
					INNER JOIN territorios.estado e ON e.estuf = ur.estuf
					WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			case "M": // Municípios
				$aca_prg = "Municípios Associados";
				$sqlRespUsuario = "SELECT
									m.muncod as codigo,
									m.estuf || ' - ' || m.mundescricao as descricao,
							  		s.stadsc as situacao,
									ur.rpustatus aS status
								   from sase.usuarioresponsabilidade ur
								   inner join territorios.municipio m on m.muncod = ur.muncod
								   left join sase.assessoramento a on ur.muncod = a.muncod
								   left join sase.situacaoassessoramento s on a.stacod = s.stacod
							  		where
									ur.usucpf = '%s' and
									ur.pflcod = '%s' and
									ur.rpustatus = 'A'
								   order by
									m.estuf,
									m.mundescricao
				";
				break;
			case "T": // Municípios a partir dos técnicos
				$aca_prg = "Municípios Associados";
				$sqlRespUsuario = "select distinct
										ur.cpftecnico,
										SUBSTR(ur.cpftecnico, 1, 3) || '.' || SUBSTR(ur.cpftecnico, 4, 3) || '.' || SUBSTR(ur.cpftecnico, 7, 3) || '-' || SUBSTR(ur.cpftecnico, 10) as codigo,
							  			u.usunome as descricao,
							  			'".PFLCOD_SASE_TECNICO."' as pfltecnico
									from sase.usuarioresponsabilidade ur
									inner join seguranca.usuario u on ur.cpftecnico = u.usucpf
	 								where
										ur.usucpf = '%s' and
										ur.pflcod = '%s' and
										ur.rpustatus = 'A'";
		
				$sqlTecRespUsuario = "SELECT
									m.muncod as codigo,
									m.estuf || ' - ' || m.mundescricao as descricao,
									s.stadsc as situacao,
									ur.rpustatus aS status
								   from sase.usuarioresponsabilidade ur
								   inner join territorios.municipio m on m.muncod = ur.muncod
								   left join sase.assessoramento a on ur.muncod = a.muncod
								   left join sase.situacaoassessoramento s on a.stacod = s.stacod
									where
									ur.usucpf = '%s' and
									ur.pflcod = '%s' and
									ur.rpustatus = 'A' and
									ur.cpftecnico = '%s'
								   order by
									m.estuf,
									m.mundescricao
											";
				break;
		}
		
		if(!$sqlRespUsuario) continue;
		
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		//ver($query, d);
		
		$respUsuario = $db->carregar($query);
		
		//ver($respUsuario, d);
		
		if (!is_array($respUsuario)){
			$respUsuario = array();
		}
		
		$listagem = new Simec_Listagem();
		$listagem->setTamanhoPagina(50);
		if ($rp["tprsigla"] == 'M'){
			$listagem->addCallbackDeCampo(array('descricao', 'situacao'), 'alinhaParaEsquerda');
			$listagem->setCabecalho ( array (
					'Código',
					'Descrição',
					'Situação'
			) );
		} else {
			$listagem->addCallbackDeCampo(array('descricao'), 'alinhaParaEsquerda');
			$listagem->setCabecalho ( array (
					'Código',
					'Descrição'
			) );
		}
		
		if ($rp["tprsigla"] == 'T'){
			$listagem->addAcao ( 'plus', array(
			 'func' => 'detalhaResponsabilidade',
					'extra-params' => array (
							'pfltecnico'
					)
			) );
		}
		
		$listagem->esconderColunas (array('status', 'cpftecnico', 'pfltecnico'));
		$listagem->setDados ( $respUsuario );
		$listagem->setFormOff();
		$listagem->render (SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
		
	}
	
	/*
	 * Victor Martins Machado
	 * Função que executa o download das informações básicas do avaliador educacional no formato PDF
	 * */
	public function fichaCadastral($aveid){
		global $db;
		
		$ficha = "";
        $avediretoria = $this->arAtributos['avediretoria'] != '' ? $this->arAtributos['avediretoria'] : $_REQUEST['avediretoria'];

		$sql = "select
					aveid,
					avenome,
					case 
						when avesexo = 'F' then 'FEMININO'
						when avesexo = 'M' then 'MASCULINO'
					end as avesexo,
					to_char(avedatanascimento, 'dd/mm/yyyy') as avedatanascimento,
					SUBSTR(avenumcpf, 1, 3) || '.' || SUBSTR(avenumcpf, 4, 3) || '.' || SUBSTR(avenumcpf, 7, 3) || '-' || SUBSTR(avenumcpf, 10) as avenumcpf,
					aveidentidade,
					aveorgaoexpedidor,
					to_char(avedataexpedicao, 'dd/mm/yyyy') as avedataexpedicao,
					aveendereco,
					avebairro,
					mun.mundescricao || '/' || mun.estuf as avecidadeuf,
					SUBSTR(avecep, 1, 2)||'.'||SUBSTR(avecep, 3, 3)||'-'||SUBSTR(avecep, 6, 3),
					
					'('||avedddtelcomercial||') '||avetelcomercial as avetelcomercimal,
					'('||avedddtelcelular||') '||avetelcelular as avetelcelular,
					aveemail,
				
					avebanco,
					aveagencia,
					aveconta,
				
					(SELECT
						grddsc
					FROM sase.itemgraduacao igrd
					INNER JOIN sase.graduacao grd
						ON (igrd.grdid = grd.grdid)
					where aveid = ave.aveid
					order by grd.grdid desc
					limit 1) as grddsc,
				
					(SELECT
						sbc.sbcdsc
					FROM sase.itemcursopos icp
					INNER JOIN sase.subareacurso sbc
						ON (icp.sbcid = sbc.sbcid)
					WHERE icp.aveid = ave.aveid
					and icp.icptipo = 'E'
					order by icp.icpid desc
					limit 1) as especializacao,
				
					(SELECT
						sbc.sbcdsc
					FROM sase.itemcursopos icp
					INNER JOIN sase.subareacurso sbc
						ON (icp.sbcid = sbc.sbcid)
					WHERE icp.aveid = ave.aveid
					and icp.icptipo = 'M'
					order by icp.icpid desc
					limit 1) as mestrado,
				
					(SELECT
						sbc.sbcdsc
					FROM sase.itemcursopos icp
					INNER JOIN sase.subareacurso sbc
						ON (icp.sbcid = sbc.sbcid)
					WHERE icp.aveid = ave.aveid
					and icp.icptipo = 'D'
					order by icp.icpid desc
					limit 1) as doutorado
												
				from sase.avaliadoreducacional ave
				inner join territorios.municipio mun on ave.muncod = mun.muncod
				where aveid = ".$aveid."
				AND ave.avediretoria = {$avediretoria}";
		
		$dados = $db->carregar($sql);
		
		/*
		$ficha .= "	<tr>";
		$ficha .= "	</tr>";
		
		$ficha .= "		<td>";
		$ficha .= "		</td>";
		
		$ficha .= "";
		
		$ficha .= 			$dados[0][''];

		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= "		</td>";
		$ficha .= "	</tr>";

		*
		*/
		
		$ficha .= "<style>";
		$ficha .= "		.tituloFicha {";
		$ficha .= "			font-weight: bold;";
		$ficha .= "			text-align: center;";
		$ficha .= "		}";
		$ficha .= "		.tituloTab {";
		$ficha .= "			font-weight: bold;";
		$ficha .= "		}";
		$ficha .= "		.tituloCmp {";
		$ficha .= "			font-weight: bold;";
		$ficha .= "			text-align: right;";
		$ficha .= "		}";
		$ficha .= "		.tituloSemReg {";
		$ficha .= "			text-align: center;";
		$ficha .= "		}";
        $ficha .= "     .upper{";
        $ficha .= "         text-transform: uppercase;";
        $ficha .= "     }";
		$ficha .= "</style>";
		
		$ficha .= "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"710\" class=\"col-md-12 table-condensed table-hover table-responsive upper\" >";
		$ficha .= "	<tr>";
		$ficha .= "		<td colspan=\"6\" class=\"tituloFicha\">";
		$ficha .= "			FICHA CADASTRAL";
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td colspan=\"6\" class=\"tituloTab\">";
		$ficha .= "			1. DADOS PESSOAIS";
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			NOME COMPLETO:";
		$ficha .= "		</td>";
		$ficha .= "		<td colspan=\"5\">";
		$ficha .= 			strtolower($dados[0]['avenome']);
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			GÊNERO:";
		$ficha .= "		</td>";
		$ficha .= "		<td colspan=\"2\">";
		$ficha .= 			strtolower($dados[0]['avesexo']);
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\" colspan=\"2\">";
		$ficha .= "			DATA DE NASCIMENTO:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			strtolower($dados[0]['avedatanascimento']);
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			CPF:";
		$ficha .= "		</td>";
		$ficha .= "		<td colspan=\"5\">";
		$ficha .= 			$dados[0]['avenumcpf'];
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			RG:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			$dados[0]['aveidentidade'];
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			ÓRGÃO EXPEDIDOR:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			strtolower($dados[0]['aveorgaoexpedidor']);
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			DATA EXPEDIÇÃO:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			strtolower($dados[0]['avedataexpedicao']);
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			END. RES. COMPLEMENTO:";
		$ficha .= "		</td>";
		$ficha .= "		<td colspan=\"3\">";
		$ficha .= 			strtolower($dados[0]['aveendereco']);
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			BAIRRO:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			strtolower($dados[0]['avebairro']);
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			CIDADE/UF:";
		$ficha .= "		</td>";
		$ficha .= "		<td colspan=\"3\">";
		$ficha .= 			strtolower($dados[0]['avecidadeuf']);
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			CEP:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			$dados[0]['avecep'];
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "</table>";
		$ficha .= "<br />";
		
		$ficha .= "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"710\" class=\"col-md-12 table-condensed table-hover table-responsive upper\" >";
		$ficha .= "	<tr>";
		$ficha .= "		<td colspan=\"4\" class=\"tituloTab\">";
		$ficha .= "			2. CONTATOS";
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			TEL. INSTITUCIONAL:";
		$ficha .= "		</td>";
		$ficha .= "		<td width=\"170\">";
		$ficha .= 			$dados[0]['avetelcomercimal'];
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			CELULAR:";
		$ficha .= "		</td>";
		$ficha .= "		<td width=\"170\">";
		$ficha .= 			$dados[0]['avetelcelular'];
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			E-MAIL INSTITUCIONAL:";
		$ficha .= "		</td>";
		$ficha .= "		<td colspan=\"3\">";
		$ficha .= 			strtolower($dados[0]['aveemail']);
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "</table>";
		$ficha .= "<br />";
		
		$ficha .= "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"710\" class=\"col-md-12 table-condensed table-hover table-responsive upper\" >";
		$ficha .= "	<tr>";
		$ficha .= "		<td colspan=\"6\" class=\"tituloTab\">";
		$ficha .= "			3. DADOS BANCÁRIOS:";
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			BANCO:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			strtolower($dados[0]['avebanco']);
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			AGÊNCIA:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			strtolower($dados[0]['aveagencia']);
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\">";
		$ficha .= "			CONTA:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		$ficha .= 			strtolower($dados[0]['aveconta']);
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		$ficha .= "</table>";
		$ficha .= "<br />";
		
		$ficha .= "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"710\" class=\"col-md-12 table-condensed table-hover table-responsive upper\" >";
		$ficha .= "	<tr>";
		$ficha .= "		<td colspan=\"3\" class=\"tituloTab\">";
		$ficha .= "			4. ESCOLARIDADE/FORMAÇÃO:";
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\" width=\"350\" style=\"text-align: left !important;\">";
		$ficha .= "			GRADUAÇÃO:";
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\" width=\"30\">";
		$ficha .= "			ÁREA:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		if ($dados[0]['grddsc'] != ''){
			$ficha .= 			strtolower($dados[0]['grddsc']);
		} else {
			$ficha .= "			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		}
		$ficha .= "		</td>";
		$ficha .= "	</tr>";

		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\" style=\"text-align: left !important;\">";
		$ficha .= "			ESPECIALIZAÇÃO/PÓS: COMPLETA (&nbsp&nbsp&nbsp&nbsp) INCOMPLETA (&nbsp&nbsp&nbsp&nbsp) ";
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\" width=\"30\">";
		$ficha .= "			ÁREA:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		if ($dados[0]['especializacao'] != ''){
			$ficha .= 			strtolower($dados[0]['especializacao']);
		} else {
			$ficha .= "			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		}
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		
		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\" style=\"text-align: left !important;\">";
		$ficha .= "			MESTRADO:";
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\" width=\"30\">";
		$ficha .= "			ÁREA:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		if ($dados[0]['mestrado'] != ''){
			$ficha .= 			strtolower($dados[0]['mestrado']);
		} else {
			$ficha .= "			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		}
		$ficha .= "		</td>";
		$ficha .= "	</tr>";

		$ficha .= "	<tr>";
		$ficha .= "		<td class=\"tituloCmp\" style=\"text-align: left !important;\">";
		$ficha .= "			DOUTORADO:";
		$ficha .= "		</td>";
		$ficha .= "		<td class=\"tituloCmp\" width=\"30\">";
		$ficha .= "			ÁREA:";
		$ficha .= "		</td>";
		$ficha .= "		<td>";
		if ($dados[0]['doutorado'] != ''){
			$ficha .= 			strtolower($dados[0]['doutorado']);
		} else {
			$ficha .= "			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		}
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
				
		$ficha .= "</table>";
		$ficha .= "<br />";

		$sqlE = "select
					lower(eaeorgaoquetrabalha),
					lower(eaediretoriacoordenacaosetor),
					lower(eaecargo),
					lower(eaefuncao)
				from sase.experienciaavaliadoreducacional
				where eaestatus = 'A'
 				and aveid = " . $aveid ."
				order by eaeid desc
				limit 1";
		
		$dadosE = $db->carregar($sqlE);
		
		$ficha .= "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"710\" class=\"col-md-12 table-condensed table-hover table-responsive upper\" >";
		$ficha .= "	<tr>";
		$ficha .= "		<td colspan=\"4\" class=\"tituloTab\">";
		$ficha .= "			5. DADOS PROFISSIONAIS:";
		$ficha .= "		</td>";
		$ficha .= "	</tr>";
		
		if (is_array($dadosE)){ 
			foreach ($dadosE as $d){
				$ficha .= "	<tr>";
				$ficha .= "		<td class=\"tituloCmp\" width=\"100\">";
				$ficha .= "			ÓRGÃO QUE TRABALHA:";
				$ficha .= "		</td>";
				$ficha .= "		<td>";
				$ficha .= 			$d['eaeorgaoquetrabalha'];
				$ficha .= "		</td>";
				$ficha .= "		<td class=\"tituloCmp\" width=\"100\">";
				$ficha .= "			DIRETORIA/COORDENAÇÃO/SETOR:";
				$ficha .= "		</td>";
				$ficha .= "		<td>";
				$ficha .= 			$d['eaediretoriacoordenacaosetor'];
				$ficha .= "		</td>";
				$ficha .= "	</tr>";
				
				$ficha .= "	<tr>";
				$ficha .= "		<td class=\"tituloCmp\">";
				$ficha .= "			CARGO:";
				$ficha .= "		</td>";
				$ficha .= "		<td>";
				$ficha .= 			$d['eaecargo'];
				$ficha .= "		</td>";
				$ficha .= "		<td class=\"tituloCmp\">";
				$ficha .= "			FUNÇÃO:";
				$ficha .= "		</td>";
				$ficha .= "		<td>";
				$ficha .= 			$d['eaefuncao'];
				$ficha .= "		</td>";
				$ficha .= "	</tr>";
			}
		} else {
			$ficha .= "	<tr>";
			$ficha .= "		<td class=\"tituloCmp\" width=\"100\">";
			$ficha .= "			ÓRGÃO QUE TRABALHA:";
			$ficha .= "		</td>";
			$ficha .= "		<td>";
			$ficha .= "			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			$ficha .= "		</td>";
			$ficha .= "		<td class=\"tituloCmp\" width=\"100\">";
			$ficha .= "			DIRETORIA/COORDENAÇÃO/SETOR:";
			$ficha .= "		</td>";
			$ficha .= "		<td>";
			$ficha .= "			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			$ficha .= "		</td>";
			$ficha .= "	</tr>";
			
			$ficha .= "	<tr>";
			$ficha .= "		<td class=\"tituloCmp\">";
			$ficha .= "			CARGO:";
			$ficha .= "		</td>";
			$ficha .= "		<td>";
			$ficha .= "			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			$ficha .= "		</td>";
			$ficha .= "		<td class=\"tituloCmp\">";
			$ficha .= "			FUNÇÃO:";
			$ficha .= "		</td>";
			$ficha .= "		<td>";
			$ficha .= "			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			$ficha .= "		</td>";
			$ficha .= "	</tr>";
		}
		
		$ficha .= "</table>";
				
		return $ficha;
	}
}























