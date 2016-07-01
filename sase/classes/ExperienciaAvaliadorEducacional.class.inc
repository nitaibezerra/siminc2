<?php

require_once APPRAIZ . 'includes/library/simec/Listagem.php';
/**
 * Classe de Avaliador Educacional
 * 
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 * 
 * Objeto de Modelo de Avaliador Educacional
 */

class ExperienciaAvaliadorEducacional extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.experienciaavaliadoreducacional";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('eaeid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
							  'eaeid' => null,
							  'aveid' => null,
							  'eaeorgaoquetrabalha' => null,
							  'eaediretoriacoordenacaosetor' => null,
							  'eaecargo' => null,
							  'eaefuncao' => null,
							  'eaecargoqueaposentou' => null
	);
	
	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributosStr = array(
			'eaeorgaoquetrabalha'
	);

	/**
	 * @name $arCampos
	 * @var array
	 * @access protected
	 * Campos da Tabela
	*/
	protected $arCampos = array(
							  'eaeid',
							  'aveid',
							  'eaeorgaoquetrabalha',
							  'eaediretoriacoordenacaosetor',
							  'eaecargo',
							  'eaefuncao',
							  'eaecargoqueaposentou'
							);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	*/
	protected $arAtributosObrigatorios = array(
							  'eaeid',
							  'aveid',
							  'eaeorgaoquetrabalha',
							  'eaediretoriacoordenacaosetor',
							  'eaecargo',
							  'eaefuncao',
							  'eaecargoqueaposentou'
							);
	
	public $eaeid;

	/**
	 * Atualiza Situacao de Assessoramento
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizar(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			if ($this->commit()){
				return true;
			} else {
				$this->rollback();
				return false;
			}
		}
	
		return false;
	}
	
	/**
	 * Exclui logicamente a situacao de assessoramento
	 *
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluir( $eaeid ){
		$this->carregarPorId( $eaeid );
		$this->arAtributos['eaeid'] = $eaeid;
		$this->arAtributos['eaestatus'] = 'I';
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
			$this->eaeid = $this->inserir();
			if ($this->commit()){
				return true;
			} else {
				$this->rollback();
				return false;
			}
		}
		return false;
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
	
	public function getAposentou($campo, $valor = null){
		$dados = array (
				array (
						'codigo' => 'f',
						'descricao' => 'Não'
				),
				array (
						'codigo' => 't',
						'descricao' => 'Sim'
				)
		);
		return $this->getOptions ( $dados, array (), $campo, null, $valor );
	}
	
	public function getQuantidadeExperienciaDoAvaliador(){
		$sql = "SELECT COUNT(*) AS cont FROM sase.experienciaavaliadoreducacional WHERE aveid = {$this->aveid}";
		$res = $this->carregar($sql);
		$res = $res ? $res[0]['cont'] : 1;
		
		return $res;
	}
	
	public function getPrimeiraExperienciaDoAvaliador(){
		$sql = "SELECT eaeid, aveid, eaeorgaoquetrabalha, eaediretoriacoordenacaosetor, eaecargo, eaefuncao, eaecargoqueaposentou, eaestatus FROM sase.experienciaavaliadoreducacional WHERE aveid = {$this->aveid} ORDER BY eaeid LIMIT 1";
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
				
		return $dados[0];
	}
	
	public function montarTabelaListagem($dados = array()) {
	
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
							'Órgão',
							'Setor',
							'Cargo',
							'Função'
					) );
					$listagem->esconderColunas (array('eaeid'));
					$listagem->addAcao ( 'edit', array(
						'func' => 'editaExperiencia',
						'extra-params' => array (
								'eaeid'
						)
					) );
					$listagem->addAcao ( 'delete', array (
							'func' => 'apagaExperiencia',
							'extra-params' => array (
									'eaeid' 
							) 
					) );
					$listagem->setDados ( $dados );
					$listagem->render ();
				?>
				</div>
				<?php
		}
			
	public function getFormExperienciaPorAvaliador() {
		$sql = "SELECT eaeid, eaeorgaoquetrabalha, eaediretoriacoordenacaosetor, eaecargo, eaefuncao FROM sase.experienciaavaliadoreducacional WHERE aveid = {$this->aveid} AND eaestatus = 'A' ORDER BY eaeid";
		$dados = $this->carregar($sql);
		
		if ($dados){
			$this->montarTabelaListagem($dados);
		}
		
		/*foreach ( $dados as $dado ) {
			if ($i > 1) {
				?>
					<div><div id="divSeparador" class="divSeparador"><br/></div></div>
					<div class="form-group tipo-judicial">
						<input name="eaeid<?php echo $i ?>" id="eaeid<?php echo $campo.$i ?>" type="hidden"/>
						<label for="eaeorgaoquetrabalha<?php echo $i ?>" class="col-lg-4 col-md-4 control-label">Órgão:</label>
					
					    <div class="col-lg-8 col-md-8 ">
					        <input id="eaeorgaoquetrabalha<?php echo $i ?>" name="eaeorgaoquetrabalha<?php echo $i ?>" type="text"
					               class="form-control" placeholder="" maxlength="50" value="<?php echo $dado['eaeorgaoquetrabalha'] ?>">
					    </div>
					</div>
					<div class="form-group tipo-judicial">
					    <label for="eaediretoriacoordenacaosetor<?php echo $i ?>" class="col-lg-4 col-md-4 control-label">Diretoria / Coordenação / Setor:</label>
					
					    <div class="col-lg-8 col-md-8 ">
					        <input id="eaediretoriacoordenacaosetor<?php echo $i ?>" name="eaediretoriacoordenacaosetor<?php echo $i ?>" type="text"
					               class="form-control" placeholder="" maxlength="50" value="<?php echo $dado['eaediretoriacoordenacaosetor'] ?>">
					    </div>
					</div>
					<div class="form-group tipo-judicial">
					    <label for="eaecargo<?php echo $i ?>" class="col-lg-4 col-md-4 control-label">Cargo:</label>
					
					    <div class="col-lg-8 col-md-8 ">
					        <input id="eaecargo<?php echo $i ?>" name="eaecargo<?php echo $i ?>" type="text"
					               class="form-control" placeholder="" maxlength="50" value="<?php echo $dado['eaecargo'] ?>">
					    </div>
					</div>
					<div class="form-group tipo-judicial">
					    <label for="eaefuncao<?php echo $i ?>" class="col-lg-4 col-md-4 control-label">Função:</label>
					
					    <div class="col-lg-8 col-md-8 ">
					        <input id="eaefuncao<?php echo $i ?>" name="eaefuncao<?php echo $i ?>" type="text"
					               class="form-control" placeholder="" maxlength="50" value="<?php echo $dado['eaefuncao'] ?>">
					    </div>
					</div>
					<div class="form-group tipo-judicial">
					    <label for="dmdmandatoseguranca" class="col-lg-4 col-md-4 control-label">Foi o cargo que aposentou?</label>
					
					    <div class="col-lg-8 col-md-8">
					        <div class="btn-group " data-toggle="buttons">
					            <label class="btn btn-default <?= ($dado['eaecargoqueaposentou'] == 't' ? 'active' : ''); ?>">
					                <input type="radio" name="eaecargoqueaposentou" id="eaecargoqueaposentou1" value="t" <?= ($dado['eaecargoqueaposentou'] == 't' ? 'checked="checked"' : ''); ?>> Sim
					            </label>
					            <label class="btn btn-default <?= ($dado['eaecargoqueaposentou'] == 'f' ? 'active' : ''); ?>">
					                <input type="radio" name="eaecargoqueaposentou" id="eaecargoqueaposentou2" value="f" <?= ($dado['eaecargoqueaposentou'] == 'f' ? 'checked="checked"' : ''); ?>> Não
					            </label>
					        </div>
					    </div>
					</div>
				<?php 
			}
			$i++;
		}*/
	}
		
}
?>