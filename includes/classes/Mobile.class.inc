<?php
/*
 * Classe Mobile
 * Classe para trabalhar com o SIMEC Mobile
 * @author Juliano Meinen
 * @since 12/04/2011
 */
class Mobile extends cls_banco {

	public function __construct( $id = null ){
		
		parent::__construct();
		
		if( $id ){
			$this->carregarPorId( $id );
		}
	}
	
	public function monta_lista($sql)
	{
		$arrDados = $this->carregar($sql);
		if($arrDados){
			?> <ul data-role="listview" data-inset="true" data-filter="true" > <?php
			foreach($arrDados as $dado){ ?>
				<li>
					<a data-transition="flip" href="<?php echo $dado['link'] ? $dado['link'] : "#" ?>"><?php echo $dado['descricao'] ?>
				 		<?php if($dado['total']): ?>
				 			<span class="ui-li-count"><?php echo $dado['total'] ?></span>
				 		<?php endif; ?>
				 	</a>
				 </li>
			<?php }
			?> </ul> <?php
		}else{ ?>
			<p class="info_usuario" >Não existem registros.</p>
		<?php }
	}

}